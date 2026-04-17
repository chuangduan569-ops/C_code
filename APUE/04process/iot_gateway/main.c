#include "iot_gateway.h"      
#include "log.h"               
#include "daemon.h"            
#include "config.h"             
#include "network.h"            
#include "io_multiplexing.h"    
#include "thread_pool.h"      
#include "process_pool.h"      
#include "token_bucket.h"     
#include "alarm_scheduler.h"   
#include "ipc_modules.h"        
#include "password_auth.h"      

int g_running = 1;//程序运行标志: 1=运行中, 0=退出中
gateway_config_t g_config;//全局配置对象，存储从配置文件加载的配置 

static int g_signal_pipe[2];//信号管道: fd[0]=读端, fd[1]=写端，用于将信号转换为IO事件
static int g_tcp_fd = -1;//TCP监听socket文件描述符
static int g_udp_fd = -1;//UDP socket文件描述符
static int g_token_bucket_id = -1;//令牌桶ID，用于UDP限流 
static pool_t *g_thread_pool = NULL;//线程池指针

//信号处理函数
static void signal_handler(int signo)
{
	write(g_signal_pipe[1], &signo, sizeof(signo));
	//将信号编号写入管道写端，主循环的poll会检测到读端有数据 
}

//注册信号处理函数
static void setup_signals(void)
{
	//创建信号管道
	if(pipe(g_signal_pipe) < 0)
	{
		LOG_ERROR("创建信号管道失败: %s", strerror(errno));
		exit(1);
	}
	//设置信号管道写端为非阻塞
	set_nonblocking(g_signal_pipe[1]);
	//注册信号处理函数
	signal(SIGTERM, signal_handler);//SIGTERM: 终止信号 
	signal(SIGINT, signal_handler);//SIGINT: 中断信号(Ctrl+C) 
	signal(SIGHUP, signal_handler);//SIGHUP: 挂起信号，用于配置热加载
	//忽略SIGPIPE，防止向已关闭socket写入时崩溃
	signal(SIGPIPE, SIG_IGN);
	//忽略SIGCHLD，子进程退出时自动回收
	signal(SIGCHLD, SIG_IGN);
}

//热加载配置文件
static void reload_config(void)
{
	config_load(DEFAULT_CONFIG_PATH, &g_config);//重新加载配置
	config_print(&g_config);//配置重新加载完成
}

//初始化IPC资源(共享内存、信号量、消息队列)
static int init_ipc_resources(void)
{
	int shmid, semid, msqid;//共享内存 信号量 消息队列
	shm_buffer_t *shm;
	unsigned short sem_init_val[2] = {1, 1};//unsigned short sem_init_val[2] = {1, 1};
	//信号量数组 比分开占用资源小

	//创建共享内存
	shmid = shm_create(IPC_KEY_SHM, sizeof(shm_buffer_t));
	if(shmid < 0)
	{
		LOG_ERROR("创建共享内存失败");
		return -1;
	}

	//初始化共享内存
	shm = (shm_buffer_t *)shm_attach(shmid);
	if(shm)//判断是否创建成功
	{
		memset(shm, 0, sizeof(shm_buffer_t));// 将整个共享内存区域清零
		shm_detach(shm);// 断开与共享内存的连接
	}
	//创建信号量
	semid = sem_create(IPC_KEY_SEM, 2, sem_init_val);
	//信号量集中包含 2 个独立的信号量
	if(semid < 0)
	{
		LOG_ERROR("创建信号量失败");
		return -1;
	}
	//创建消息队列
	msqid = msgq_create(IPC_KEY_MSGQ);
	if(msqid < 0)
	{
		LOG_ERROR("创建消息队列失败");
		return -1;
	}
	return 0;
}

//优雅退出，释放所有资源
static void graceful_shutdown(void)
{
	LOG_INFO("正在悠悠丫丫的退出呢...");
	g_running = 0;//设置退出标志
	close_all_connections();//关闭所有连接
	//关闭socket
	if (g_tcp_fd > 0) 
		close(g_tcp_fd);
	if (g_udp_fd > 0) 
		close(g_udp_fd);
	//销毁线程池
	if(g_thread_pool)
       	pool_destroy(g_thread_pool);//销毁线程池
    process_pool_destroy();//销毁进程池
	//销毁令牌桶
	if(g_token_bucket_id >= 0)
		tbf_destroy(g_token_bucket_id);//销毁令牌桶
	log_close();//关闭日志
	remove_pid_file();//删除PID文件
	LOG_INFO("悠悠丫丫的退出完毕--over");
}



//-h 打印帮助信息
//参数: prog - 程序名
static void print_usage(const char *prog)
{
    printf("物联网网关 v%s\n", GATEWAY_VERSION);
    printf("用法: %s [选项] [配置文件]\n", prog);
    printf("选项:\n");
    printf("  -d         以守护进程模式运行（后台运行）\n");
    printf("  -h         显示此帮助信息\n");
    printf("  --no-auth  跳过密码认证（仅调试模式使用）\n");
}

int main(int argc, char *argv[])
{
	const char *config_file = DEFAULT_CONFIG_PATH;//指定路径
	int daemon_mode = 0;//守护进程模式标志
	int need_auth = 1;//密码认证标志
	int i;
	static int g_udp_broadcast_fd = -1;  // 新增：UDP广播socket文件描述符
	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-d") == 0)
			daemon_mode = 1;//用户要求后台运行
		else if(strcmp(argv[i], "-h") == 0)
			{
				print_usage(argv[0]);//打印帮助信息
				return 0;
			}
		else if(strcmp(argv[i], "--no-auth") == 0)
			need_auth = 0;//用户跳过认证（调试用）
		else if(argv[i][0] != '-')
			config_file = argv[i];//用户指定了配置文件路径
	}
	//[2] 密码认证(守护进程化之前)
	if(need_auth)
	{
		printf("\n========================================\n");
	    printf("   物联网网关 v%s\n", GATEWAY_VERSION);
    	printf("   系统认证 : \n");
    	printf("========================================\n\n");

    	if (authenticate_user() != 0) 
		{
        	fprintf(stderr, "\n认证失败！程序退出...\n");
        		return 1;
    	}
    	printf("\n认证成功！正在启动网关...\n\n");
	}

	//[3] 加载配置
	if(config_load(config_file, &g_config) < 0)
	{
		fprintf(stderr, "加载配置文件失败: %s\n", config_file);
        return 1;
	}

	//[4] 初始化日志 
	log_init(g_config.log_path, g_config.log_level);

	//[5] 守护进程化
	if(daemon_mode)
	{
		if(daemonize() < 0)
		{
			LOG_ERROR("守护进程化失败");
			return -1;
		}
		log_init(g_config.log_path, g_config.log_level);
	}

	//[6]创建PID文件 
	if(create_pid_file() < 0)
		return -2;

	LOG_INFO("物联网网关 v%s 正在启动...", GATEWAY_VERSION);
	config_print(&g_config);

	//[7] 注册信号
	setup_signals();

	//[8] 初始化IPC
	if(init_ipc_resources() < 0)
	{
		LOG_ERROR("IPC资源初始化失败");
		graceful_shutdown();//优雅退出
		return -3;
	}

	//[9] 创建令牌桶
	g_token_bucket_id = tbf_init(g_config.token_rate, g_config.token_burst);
	if (g_token_bucket_id < 0) 
	{
		LOG_ERROR("令牌桶初始化失败");
		graceful_shutdown();
		return -4;
	}
	
	//[10] 创建线程池
	if(pool_init(&g_thread_pool, 10) < 0)
	{
		LOG_ERROR("线程池初始化失败");
		graceful_shutdown();
		return -5;
	}
 	LOG_INFO("线程池初始化成功");
 	//新增：将线程池设置到io_multiplexing模块
 	set_io_thread_pool(g_thread_pool);  // 只有这一行是新增的
	//[11] 创建进程池 
	if(process_pool_init(PROCESS_POOL_SIZE) < 0)
		LOG_WARN("进程池初始化失败，继续运行");
	//[12] 初始化网络
	g_tcp_fd = init_tcp_server(g_config.tcp_port);
	if(g_tcp_fd < 0)
	{
		LOG_ERROR("TCP服务器初始化失败");
		graceful_shutdown();
		return -6;
	}
	g_udp_fd = init_udp_server(g_config.udp_port);

	if(g_udp_fd < 0)
	{
		LOG_ERROR("UDP服务器初始化失败");
		graceful_shutdown();
		return -7;
	}
	// ========== 新增：初始化UDP广播socket ==========
    g_udp_broadcast_fd = init_udp_broadcast(g_config.udp_broadcast_port);
    if(g_udp_broadcast_fd < 0)
    {
        LOG_WARN("UDP广播初始化失败，将无法发送广播消息");
    }
    else
    {
    	set_udp_broadcast_fd(g_udp_broadcast_fd);
  	//新增：将广播socket传递给io_multiplexing模块
    }
	LOG_INFO("网关启动成功！");
	LOG_INFO("TCP端口: %d (云平台连接)", g_config.tcp_port);
	LOG_INFO("UDP端口: %d (32开发板)", g_config.udp_port);
	//[13] 主事件循环
	io_multiplexing_loop(g_tcp_fd, g_udp_fd, g_signal_pipe[0],
                          g_token_bucket_id, g_config.max_connections, &g_running);
	
	//[14] 优雅退出
	graceful_shutdown();

	return 0;
}




























