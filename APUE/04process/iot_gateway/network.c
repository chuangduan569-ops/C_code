#include "iot_gateway.h"
#include "network.h"
#include "log.h" 

//设置文件描述符为非阻塞模式
//非阻塞模式下，IO操作立即返回，不会阻塞进程
int set_nonblocking(int fd)
{
	int flags;//存储文件状态标志，用于fcntl操作
	//F_GETFL: 获取当前文件状态标志 
	flags = fcntl(fd, F_GETFL, 0);
	if(flags == -1)
	{
		LOG_ERROR("fcntl F_GETFL failed: %s", strerror(errno));
		return -1;
	}
	//F_SETFL: 设置文件状态标志，添加O_NONBLOCK非阻塞标志
	if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		LOG_ERROR("fcntl F_SETFL failed: %s", strerror(errno));
		return -2;
	}
	return 0;
}

//设置文件描述符的FD_CLOEXEC标志
int set_fd_cloexec(int fd)
{
	int flags;//存储文件描述符标志

	// F_GETFD: 获取文件描述符标志
	flags = fcntl(fd, F_GETFD, 0);
	if(flags == -1) 
	{
        LOG_ERROR("fcntl F_GETFD failed: %s", strerror(errno));
        return -1;
    }
	//F_SETFD: 设置文件描述符标志，添加FD_CLOEXEC
	//执行exec时自动关闭文件描述符，防止子进程继承
	if(fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
	{
		LOG_ERROR("fcntl F_SETFD failed: %s", strerror(errno));
		return -1;
	}
	return 0;
}

//初始化TCP服务器
int init_tcp_server(int port)
{
	int fd;//socket文件描述符
	struct sockaddr_in addr;//IPv4地址结构体，用于bind()绑定地址
	int opt = 1; //socket选项值，1表示启用该选项
	//创建TCP socket: AF_INET=IPv4地址族, SOCK_STREAM=流式套接字(TCP协议)
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)//判断是否失败
	{
		LOG_ERROR("TCP socket creation failed: %s", strerror(errno));
		return -1;
	}
	//SO_REUSEADDR: 端口重用选项
	//设置套接字上的选项
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		LOG_ERROR("setsockopt SO_REUSEADDR failed: %s", strerror(errno));
		close(fd);
		return -2;
	}
	//绑定地址和端口
	addr.sin_family = AF_INET; //IPv4地址
	addr.sin_addr.s_addr = INADDR_ANY; //监听所有网络接口
	addr.sin_port = htons(port); //端口号，转换为网络字节序
	
	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)//bind失败返回-1 
	{
		LOG_ERROR("TCP bind failed on port %d: %s", port, strerror(errno));
		close(fd);
		return -3;
	}
	//开始监听: SOMAXCONN是系统定义的最大待处理连接数
	if(listen(fd, SOMAXCONN) < 0)
	{
		LOG_ERROR("TCP listen failed: %s", strerror(errno));
		close(fd);
		return -4;
	}
	//设置非阻塞模式
	set_nonblocking(fd);
	//设置FD_CLOEXEC标志，防止子进程继承此socket
	set_fd_cloexec(fd);

	return fd;//返回监听socket文件描述符
}

//初始化UDP服务器
int init_udp_server(int port)
{
	int fd;//socket文件描述符 
	struct sockaddr_in addr;//IPv4地址结构体
	//创建UDP socket: SOCK_DGRAM=数据报套接字(UDP协议)
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0)
	{
		LOG_ERROR("UDP socket creation failed: %s", strerror(errno));
		return -1;
	}
	//绑定地址和端口
	addr.sin_family = AF_INET; //IPv4地址
	addr.sin_addr.s_addr = INADDR_ANY;//监听所有网络接口
	addr.sin_port = htons(port); //端口号，转换为网络字节序
	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		LOG_ERROR("UDP bind failed on port %d: %s", port, strerror(errno));
		close(fd);
		return -2;
	}
	//设置非阻塞模式
	set_nonblocking(fd);
	//设置FD_CLOEXEC标志
	set_fd_cloexec(fd);

	return fd;//返回UDP socket文件描述符 
}

// ========== 新增：初始化UDP广播发送socket ==========
/*
 * 功能: 初始化UDP广播发送socket
 * 参数: port - 广播端口
 * 返回值: 成功返回socket fd，失败返回-1
 */
int init_udp_broadcast(int port)
{
    int fd;                 // socket文件描述符
    int broadcast = 1;      // 广播选项值，1表示允许广播
    struct sockaddr_in addr;// 地址结构体
    
    // 创建UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        LOG_ERROR("UDP broadcast socket creation failed: %s", strerror(errno));
        return -1;
    }
    
    // 设置SO_BROADCAST选项，允许发送广播数据包
    // 这是广播功能的关键设置，没有这个选项无法发送广播
    if(setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        LOG_ERROR("setsockopt SO_BROADCAST failed: %s", strerror(errno));
        close(fd);
        return -2;
    }
    
    // 可选：绑定到指定端口，用于接收广播响应
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网络接口
    addr.sin_port = htons(port);
    
    // bind失败不影响广播发送，只影响接收
    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        LOG_WARN("UDP broadcast bind failed: %s (broadcast send only)", strerror(errno));
        // 广播发送不需要bind也能正常工作
    }
    
    // 设置非阻塞模式
    set_nonblocking(fd);
    // 设置FD_CLOEXEC标志
    set_fd_cloexec(fd);
    
    LOG_INFO("UDP broadcast initialized on port %d", port);
    return fd;
}

// ========== 新增：发送UDP广播数据 ==========
/*
 * 功能: 发送UDP广播数据到网络中的所有设备
 * 参数: fd - socket文件描述符
 *       data - 要发送的数据
 *       len - 数据长度
 *       port - 目标端口
 * 返回值: 成功返回发送字节数，失败返回-1
 */
int send_udp_broadcast(int fd, const char *data, int len, int port)
{
    struct sockaddr_in broadcast_addr;  // 广播地址结构体
    int ret;                            // 发送字节数
    
    // 设置广播地址结构体
    broadcast_addr.sin_family = AF_INET;                    // IPv4地址族
    broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;      // 广播地址 255.255.255.255
    broadcast_addr.sin_port = htons(port);                  // 目标端口
    
    // 发送广播数据
    ret = sendto(fd, data, len, 0, 
                 (struct sockaddr *)&broadcast_addr, 
                 sizeof(broadcast_addr));
    if(ret < 0)
    {
        LOG_ERROR("UDP broadcast send failed: %s", strerror(errno));
        return -1;
    }
    
    LOG_DEBUG("UDP broadcast sent %d bytes to %s:%d", ret, "255.255.255.255", port);
    return ret;
}










