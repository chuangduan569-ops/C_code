#include "iot_gateway.h"
#include "io_multiplexing.h"
#include "network.h" 
#include "log.h"
#include "config.h" 
#include "token_bucket.h"


#include "token_bucket.h"
#include "sensor_parser.h"
#include "thread_pool.h"
#include "http_client.h"
#include "base64.h"

static client_t *g_clients = NULL; //客户端链表头指针，指向第一个客户端
static int g_client_count = 0; //当前客户端连接数，用于统计和限制
static int g_cloud_fd = -1; //ESLink云平台连接socket，-1表示未连接
static int g_cloud_connected = 0; //ESLink云平台连接状态: 1=已连接，0=未连接


// ========== 新增：线程池指针 ==========
static pool_t *g_thread_pool = NULL;  // 新增：线程池指针，用于异步处理UDP数据

static int g_udp_broadcast_fd = -1;  // UDP广播socket，用于发送广播消息

// ========== 新增：设置线程池函数 ==========
void set_io_thread_pool(pool_t *pool)  // 新增：供main.c调用，传入线程池指针
{
    g_thread_pool = pool;  // 新增：保存线程池指针到静态全局变量
}

// ========== 新增：设置广播socket的函数 ==========
void set_udp_broadcast_fd(int fd)  // 供main.c调用，传入广播socket
{
    g_udp_broadcast_fd = fd;
    LOG_INFO("UDP broadcast fd set to %d", fd);
}
/*
 * 功能: 广播网关状态到网络中的所有STM32设备
 * 参数: 无
 * 返回值: 成功0，失败-1
 */
static void broadcast_gateway_status(void)
{
    char broadcast_msg[256];
    
    // 如果广播socket未初始化，直接返回
    if(g_udp_broadcast_fd < 0)
    {
        LOG_DEBUG("UDP broadcast fd not available");
        return;
    }
    
    // 构造广播消息（JSON格式，便于STM32解析）
    snprintf(broadcast_msg, sizeof(broadcast_msg),
             "{\"type\":\"gateway_status\",\"connections\":%d,\"timestamp\":%ld}",
             g_client_count, time(NULL));
    
    // 发送广播到端口8082
    send_udp_broadcast(g_udp_broadcast_fd, broadcast_msg, strlen(broadcast_msg), 8082);
    
    LOG_DEBUG("Broadcast gateway status: %s", broadcast_msg);
}

// ========== 新增：UDP数据处理任务结构体 ==========
typedef struct   // 新增：用于在线程池中传递UDP数据
{
    char buffer[4096];              // 新增：接收到的原始数据缓冲区
    int len;                        // 新增：数据有效长度
    struct sockaddr_in client_addr; // 新增：客户端地址信息
    char client_ip[32];             // 新增：客户端IP字符串，用于日志记录
} udp_task_t;                       // 新增：UDP任务结构体类型

// ========== 新增：上报数据到云平台（使用HTTP API） ==========
static void upload_to_cloud(const sensor_data_t *sensor)  // 新增：将解析后的传感器数据上报到云平台
{
    // 新增：上报温度数据到云平台
    upload_sensor_data(CLOUD_IP, DEVICE_ID, SENSOR_TEMP, API_KEY, sensor->sht_temperature);
    LOG_INFO("上报温度: %.1f", sensor->sht_temperature);  // 新增：调试日志
    
    // 新增：上报湿度数据到云平台
    upload_sensor_data(CLOUD_IP, DEVICE_ID, SENSOR_HUMI, API_KEY, sensor->sht_humidity);
    LOG_INFO("上报湿度: %.1f", sensor->sht_humidity);  // 新增：调试日志
}

// ========== 新增：处理UDP数据（在线程池中执行） ==========
static void* process_udp_data(void *arg)  // 新增：线程池工作线程执行的函数
{
    udp_task_t *task = (udp_task_t *)arg;  // 新增：获取传递过来的UDP任务数据
    sensor_data_t sensor;  // 新增：传感器数据结构体，用于存储解析后的数据
    
    LOG_INFO("处理来自 %s 的UDP数据: %s", task->client_ip, task->buffer);  // 新增：记录处理日志
    
    // 新增：解析传感器数据
    if(parse_sensor_data(task->buffer, &sensor) == 0)  // 新增：调用解析函数
    {
        sensor.device_id = 1;  // 新增：设置设备ID（可根据源IP映射不同设备）
        
        // 新增：保存到共享内存，供其他进程读取
        save_to_shm(&sensor);  // 新增：保存传感器数据
        
        // 新增：上报到云平台
        upload_to_cloud(&sensor);  // 新增：调用上报函数
        
        LOG_INFO("设备[%s]数据已处理: 温度=%.1f, 湿度=%.1f, ADC=%d",  // 新增：处理完成日志
                 task->client_ip, sensor.sht_temperature, 
                 sensor.sht_humidity, sensor.adc_value);
    }
    else
    {
        LOG_WARN("解析UDP数据失败: %s", task->buffer);  // 新增：解析失败警告
    }
    
    free(task);  // 新增：释放任务结构体内存
    return NULL;  // 新增：线程函数返回
}


// 将客户端添加到链表头部
//参数: client - 要添加的客户端对象指针
static void add_client(client_t *client)
{
	client->next = g_clients;//新节点的next指针指向原头节点
	g_clients = client; //更新头指针指向新节点
	g_client_count++;//连接数加1
}
//从链表中移除客户端并销毁
//参数: client - 要移除的客户端对象指针
static void remove_client(client_t *client)
{
	client_t **p = &g_clients;//二级指针，用于遍历链表并修改指针
	//遍历链表找到要删除的节点 
	while(*p)//遍历直到链表末尾
	{
		if(*p == client)//找到目标节点
		{
			*p = client->next;//将前一个节点的next指向后一个节点
			break;
		}
		p = &(*p)->next;//移动到下一个节点
	}
	//关闭socket并释放内存 
	if(client->fd > 0)
	{
		close(client->fd);//关闭socket连接
	}
	free(client);//释放客户端对象内存
	g_client_count--;//连接数减1
}
//验证客户端认证令牌
//参数 client - 客户端对象指针
	//token - 客户端发送的令牌字符串
static int client_authenticate(client_t *client, const char *token)
{
	char clean_token[128];//清理后的令牌字符串缓冲区
	strncpy(clean_token, token, sizeof(clean_token) -1);//复制令牌并确保以'\0'结尾
	clean_token[sizeof(clean_token) - 1] = '\0';
	clean_token[strcspn(clean_token, "\n\r")] = '\0';//去除换行符和回车符

	//与配置中的认证令牌比较
	if(strcmp(clean_token, g_config.auth_token) == 0)//trcmp返回0表示相等
	{
		client->authenticated = 1;//标记为已认证
		return 1;//返回1表示认证成功
	}
	LOG_WARN("Client fd=%d authentication failed", client->fd);
	return 0;//返回0表示认证失败
}
//将响应数据放入写缓冲区
//参数: client - 客户端对象指针
	//response - 响应字符串
static int client_send_response(client_t *client, const char *response)
{
	int len = strlen(response);//获取响应字符串长度
	//检查写缓冲区是否有足够空间
	if(len < (int)(sizeof(client->wbuf) - client->wlen))
	{
		memcpy(client->wbuf + client->wlen, response, len);//将响应追加到写缓冲区末尾
		client->wlen += len;//更新待发送数据长度 
		return 0;//返回0表示成功
	}
	return -1;//缓冲区已满，返回-1表示失败 
}
//处理新的TCP连接请求 
//accept新连接，检查连接数限制，加入客户端链表
//参数: tcp_fd - TCP监听socket文件描述符
static int handle_tcp_accept(int tcp_fd)
{
	struct sockaddr_in client_addr;//客户端地址信息
	socklen_t addr_len = sizeof(client_addr);//地址结构体长度
	client_t *client;//新客户端对象指针
	int client_fd;  //新连接的socket文件描述符

	//accept新连接，获取客户端socket
	client_fd = accept(tcp_fd, (struct sockaddr *)&client_addr, &addr_len);
	if(client_fd < 0)//判断连接是否失败
	{
		if(errno != EAGAIN && errno != EWOULDBLOCK)
			LOG_ERROR("accept failed: %s", strerror(errno));
		return -1;
	}
	//检查是否超过最大连接数限制 
	if(g_client_count >= MAX_CONNECTIONS)
	{
		LOG_WARN("Max connections reached (%d), rejecting new connection", MAX_CONNECTIONS);
		close(client_fd);//关闭新连接的文件描述符
		return -1;
	}
	//设置新连接的socket为非阻塞模式
	set_nonblocking(client_fd);
	//分配客户端对象内存并初始化为0
	client = (client_t *)calloc(1, sizeof(client_t));
	if(client == NULL)
	{
		LOG_ERROR("Failed to allocate client memory");
		close(client_fd);
		return -1;
	}
	//初始化客户端对象成员
	client->fd = client_fd;//保存socket文件描述符
	client->last_active = time(NULL);//记录当前时间作为最后活跃时间
	client->authenticated = 0;//初始状态未认证
	client->rlen = 0;//读缓冲区长度初始为0
	client->wlen = 0;//写缓冲区长度初始为0

	//加入客户端链表
	add_client(client);

	return 0;
}
//处理TCP客户端数据读取
// 读取数据 → 认证(未认证时) → 业务处理(Echo服务)
//参数: client - 客户端对象指针
static int handle_tcp_read(client_t *client)
{
	char buffer[4096]; //临时读缓冲区
	int n; //读取的字节数 
	char response[8192]; //响应缓冲区
	//从socket读取数据 
	n = read(client->fd, buffer, sizeof(buffer) - 1);
	if(n < 0)
	{
		LOG_ERROR("Read from client fd=%d failed: %s", client->fd, strerror(errno));
		return -1;
	}
	if(n == 0)//read返回0表示对端关闭连接
	{
		LOG_INFO("Client fd=%d closed connection", client->fd);
		return -2;
	}
	buffer[n] = '\0';//添加字符串结束符
	client->last_active = time(NULL);//更新最后活跃时间
	//未认证状态：先进行认证
	if(!client->authenticated)
	{
		if(!client_authenticate(client, buffer))
		{
			client_send_response(client, "AUTH_FAILED\n");//发送认证失败响应 
			return -3;
		}
			client_send_response(client, "AUTH_OK\n");//发送认证成功响应
	}
	//已认证：实现Echo服务，回显收到的消息
	snprintf(response, sizeof(response), "ECHO: %s", buffer); //构造响应
	client_send_response(client, response);//发送响应
	LOG_DEBUG("Echo from fd=%d: %s", client->fd, buffer);//记录日志
	return 0;
}

/*
 功能 处理TCP客户端数据发送 将写缓冲区中的数据发送给客户端
 参数 client  客户端对象指针
 返回值 成功0 失败-1
 */
static int handle_tcp_write(client_t *client)
{
	int n;//实际发送的字节数 
	//发送数据
	n = write(client->fd, client->wbuf, client->wlen);
	if(n < 0)
	{
		if(errno == EAGAIN || errno == EWOULDBLOCK)//EAGAIN/EWOULDBLOCK表示发送缓冲区满，稍后重试 
			return 0;
		LOG_ERROR("Write to client fd=%d failed: %s", client->fd, strerror(errno));
			return -1;
	}
	//处理部分发送的情况
	if(n < client->wlen)
	{
		//将未发送的数据移到缓冲区开头
		memmove(client->wbuf, client->wbuf + n, client->wlen - n);
		client->wlen -= n;//更新待发送长度
	}
	else
		client->wlen = 0;//全部发送完成
	return 0;
}
/*
 功能 上报数据到东方瑞通ESLink云平台
将32开发板发送的传感器数据转发到云平台
参数 data 要上报的数据字符串  len 数据长度
 返回值 成功0  失败-1
 */
static int report_to_cloud(const char *data, int len)
{
	struct sockaddr_in server_addr;//服务器地址结构体
	char send_buf[8192];//发送缓冲区
	
	//检查并建立与ESLink云平台的TCP连接
	if(!g_cloud_connected)
	{
		g_cloud_fd = socket(AF_INET, SOCK_STREAM, 0);//创建socket
		if(g_cloud_fd < 0)
		{
			LOG_ERROR("Create cloud socket failed: %s", strerror(errno));
			return -1;
		}
		//设置服务器地址
		server_addr.sin_family = AF_INET;//IPv4地址
		server_addr.sin_port = htons(g_config.cloud_port);//端口号
		inet_aton(g_config.cloud_ip, &server_addr.sin_addr);//IP地址转换

		//连接ESLink云平台
		if(connect(g_cloud_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			LOG_ERROR("Connect to ESLink cloud failed: %s:%d - %s",\
					g_config.cloud_ip, g_config.cloud_port, strerror(errno));
			close(g_cloud_fd);
			return -1;
		}
		//设置非阻塞模式
		set_nonblocking(g_cloud_fd);
		g_cloud_connected = 1;//标记已连接
		}
		//构造JSON格式数据上报(ESLink云平台标准格式)
		snprintf(send_buf, sizeof(send_buf),
             "{\"device\":\"gateway\",\"type\":\"sensor_data\",\"data\":%.*s}\n",
             len, data);
		//发送数据到云平台
		if(write(g_cloud_fd, send_buf, strlen(send_buf)) < 0)
		{
			LOG_ERROR("Send to cloud failed: %s", strerror(errno));
			close(g_cloud_fd);
			g_cloud_connected = 0;
			return -2;
		}
		return 0;
}
/*
 功能 处理UDP数据报接收
令牌桶限流 → recvfrom接收 → 上报云平台 → 回复ACK
 参数 udp_fd - UDP socket文件描述符
 	token_bucket_id - 令牌桶ID，用于限流
返回值  成功0 失败-1
 */
static int handle_udp_read(int udp_fd, int token_bucket_id)
{
	char buffer[4096];//数据缓冲区
	struct sockaddr_in client_addr;//客户端地址(32开发板)
	socklen_t addr_len = sizeof(client_addr);//地址结构体长度
	int n;//接收的字节数
	int token_fetched;//获取的令牌数
	udp_task_t *task;           // 新增：UDP任务结构体指针
	task_t pool_task;           // 新增：线程池任务结构体

	  // 关键修改：改为 while 循环，读尽所有待处理的数据包
    while(1)  // 新增循环
    {
        addr_len = sizeof(client_addr);
        n = recvfrom(udp_fd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client_addr, &addr_len);
        
        if(n <= 0) {
            if(errno != EAGAIN && errno != EWOULDBLOCK && errno != 0) {
                LOG_ERROR("UDP recvfrom failed: %s", strerror(errno));
            }
            break;  // 没有更多数据了，退出循环
        }
        
        // 令牌桶限流（放在循环内，每个包单独检查）
        if(tbf_fetch_token(token_bucket_id, 1) <= 0) {
            LOG_WARN("UDP rate limit exceeded, dropping packet");
            continue;  // 跳过这个包，继续处理下一个
        }
        
        buffer[n] = '\0';
        LOG_INFO("Received from %s: %s", inet_ntoa(client_addr.sin_addr), buffer);
        
        // 立即回复ACK（保持快速响应）
        sendto(udp_fd, "ACK", 3, 0, (struct sockaddr *)&client_addr, addr_len);
        
        // 提交到线程池处理...
        if(g_thread_pool != NULL) {
            task = (udp_task_t *)malloc(sizeof(udp_task_t));
            if(task) {
                memcpy(task->buffer, buffer, n + 1);
                task->len = n;
                memcpy(&task->client_addr, &client_addr, sizeof(client_addr));
                snprintf(task->client_ip, sizeof(task->client_ip), "%s", 
                         inet_ntoa(client_addr.sin_addr));
                
                pool_task.job = process_udp_data;
                pool_task.arg = task;
                pool_add_task(g_thread_pool, &pool_task);
            }
        }
    }
    return 0;
}
/*
 功能 检查心跳超时连接
 参数 timeout_sec - 超时时间(秒)
 返回值 无
 */
static void check_heartbeat(int timeout_sec)
{
	client_t *client = g_clients;//指向链表头
	client_t *next;//临时保存下一个节点
	time_t now = time(NULL);//当前时间戳

	//遍历链表检查每个客户端
	while(client)
	{
		next = client->next;//先保存下一个节点，因为当前节点可能被删除 
		//检查最后活跃时间是否超时 
		if(now - client->last_active > timeout_sec)
		{
			LOG_INFO("Heartbeat timeout, closing fd=%d (inactive for %ld seconds)",
                     client->fd, (long)(now - client->last_active));
			remove_client(client);//超时，断开连接
		}
		client = next;//移动到下一个节点
	}
}
//获取当前连接数
int get_connection_count(void)
{
	return g_client_count;//返回全局连接计数 
}
/*
 功能 关闭所有连接
 参数 返回值 无
 */
void close_all_connections(void)
{
	client_t *client = g_clients;//指向链表头
	client_t *next;//临时保存下一个节点
	//遍历链表，关闭所有客户端连接
	while(client)
	{
		next = client->next;//先保存下一个节点
		if(client->fd > 0)
			close(client->fd);//先保存下一个节点
		free(client);
		client = next;//移动到下一个节点 
	}
	//关闭云平台连接
	if(g_cloud_fd > 0)
	{
		close(g_cloud_fd);
		g_cloud_connected = 0;
	}
	g_clients = NULL;//链表头置空
	g_client_count = 0;//连接数清零
}


//主事件循环核心，使用poll同时监听多个事件源
/*
 参数 tcp_fd - TCP监听socket
        udp_fd - UDP socket
        signal_fd - 信号管道读端
        token_bucket_id - 令牌桶ID
        max_connections - 最大连接数
        p_running - 程序运行标志指针
 */

int io_multiplexing_loop(int tcp_fd, int udp_fd, int signal_fd,
                          int token_bucket_id, int max_connections, int *p_running)
{
	struct pollfd *pollfds;//pollfd数组，动态分配 
	int nfds;//pollfd数组中的文件描述符数量
	int i;
	int ret;//poll返回值 
	client_t *client;//客户端对象指针
	time_t last_heartbeat = 0;//上次心跳检查时间
	int client_index;//客户端在pollfd数组中的起始索引

	//分配pollfd数组: 最多max_connections个客户端 + 3个固定事件源
	pollfds = (struct pollfd *)malloc((max_connections + 3) * sizeof(struct pollfd));
	if(pollfds == NULL)
	{
		LOG_ERROR("Failed to allocate pollfds array");
		return -1;
	}
	//主循环事件
	while(*p_running)
	{
		nfds = 0;//重置文件描述符数量
		//[事件源1] 信号管道 - 用于将信号转换为IO事件 
		pollfds[nfds].fd = signal_fd;//把文件描述符填写到epoll中
		pollfds[nfds].events |= POLLIN;//监听可读事件
		nfds++;
	
		//[事件源2] TCP监听socket - 接收新连接
		pollfds[nfds].fd = tcp_fd;
		pollfds[nfds].events |= POLLIN;//监听新连接事件
		nfds++;

		//[事件源3] UDP socket - 接收数据报 
		pollfds[nfds].fd = udp_fd;
		pollfds[nfds].events |= POLLIN;//监听数据到达事件 
		nfds++;

		//[事件源4] 所有TCP客户端连接 
		for(client = g_clients; client != NULL; client = client->next)
		{
			if(nfds >= max_connections + 3)//防止数组越界
			{
				LOG_WARN("Too many connections, some may not be monitored");
				break;
			}
			pollfds[nfds].fd = client->fd;
			pollfds[nfds].events |= POLLIN;//总是监听读事件
			if(client->wlen > 0)
				pollfds[nfds].events |= POLLOUT;//有数据待发送时监听写事件
			nfds++;
		}
		//调用poll等待IO事件
		ret = poll(pollfds, nfds, -1);
		if(ret < 0)//判断是否失败
		{
			if(errno == EINTR)//被信号中断
				continue;//继续循环
			LOG_ERROR("poll failed: %s", strerror(errno));
			break;
		}
		//处理信号管道事件
		if(pollfds[0].revents & POLLIN)//检查pollfds数组第0个（信号文件描述符）是否有数据可读
		{
			int signo;//用于存储接收到的信号编号
			read(signal_fd, &signo, sizeof(signo));//从信号管道读取信号值，存入signo变量
			if(signo == SIGTERM || signo == SIGINT)
			//// 判断信号是否为SIGTERM（kill命令默认信号）或SIGINT（Ctrl+C）
			{
				LOG_INFO("Received shutdown signal");//收到关闭信号
				break;
			}
		}
		//处理TCP新连接
		//检查pollfds数组第1个（TCP监听socket）是否有新连接到达
			if(pollfds[1].revents & POLLIN)
			{
				handle_tcp_accept(tcp_fd);//调用函数接受新连接，创建客户端结构体，加入客户端链表
				}
		//处理UDP数据
		//检查pollfds数组第2个（UDP socket）是否有数据可读
			if(pollfds[2].revents & POLLIN)
			{
			handle_udp_read(udp_fd, token_bucket_id);//调用函数读取UDP数据，并进行处理
			//token_bucket_id是令牌桶ID，用于流量控制
			}

		//处理TCP客户端
		client_index = 3;//客户端从索引3开始
		//只要client不为空且索引未越界就继续循环
		for(client = g_clients; client != NULL && client_index < nfds;  )
		{
			client_t *next = client->next;//保存下一个节点，因为当前节点可能被删除
			//读事件处理
			if(pollfds[client_index].revents & POLLIN)
			{
				if(handle_tcp_read(client) < 0)//检查当前客户端是否有数据可读
				{
					remove_client(client);//读取失败，断开连接
					client = next;
					client_index++;//索引加1，指向下一个pollfd位置
					continue;
				}
			}
			//写事件处理
			if(pollfds[client_index].revents & POLLOUT)
			{
				if(handle_tcp_write(client) < 0)//检查当前客户端是否可写
				{
					remove_client(client);//写入失败，断开连接 
					client = next;
					client_index++;
					continue;
				}
			}
			client = next;//当前客户端处理完毕，移动到下一个客户端
			client_index++;//索引加1，指向下一个pollfd位置
		}
		//心跳检测
		time_t now = time(NULL);//获取当前时间戳
		if(now - last_heartbeat > HEARTBEAT_INTERVAL)//判断距离上次心跳检测是否超过了检测间隔
		{
			check_heartbeat(HEARTBEAT_INTERVAL);//遍历所有客户端，检查最后活跃时间
			last_heartbeat = now;//更新上次心跳检测时间
			broadcast_gateway_status();  // 每10秒广播一次网关状态
			//输出状态监控信息
			LOG_INFO("Status: connections=%d, tokens=%d",
                     g_client_count, tbf_get_tokens(token_bucket_id));
		}
	}
	free(pollfds);//释放pollfd数组
	return 0;
}










