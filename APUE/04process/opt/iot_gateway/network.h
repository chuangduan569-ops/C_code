#ifndef __NETWORK_H             
#define __NETWORK_H            

#include "iot_gateway.h" 

/*==================== 网络配置常量 ====================*/
#define DEFAULT_TCP_PORT 8080      //TCP服务默认端口号，用于与云平台通信 
#define DEFAULT_UDP_PORT 8081      //UDP服务默认端口号，用于与32开发板通信 
#define MAX_CONNECTIONS 5000       //最大支持TCP并发连接
#define CLIENT_BUFFER_SIZE 8192    //客户端读写缓冲区大小(8KB)，足够存储一条消息 
#define HEARTBEAT_TIMEOUT 60       //心跳超时时间(秒)，超过此时间无数据则断开连接
#define HEARTBEAT_INTERVAL 10      //心跳检查间隔(秒)，每10秒检查一次超时连接 

/*==================== 云平台配置常量 ====================*/
#define DEFAULT_CLOUD_IP "119.29.98.16"    //东方瑞通ESLink云平台默认地址
#define DEFAULT_CLOUD_PORT 80              //ESLink云平台默认端口(MQTT协议)

/*==================== 应用层协议定义 ====================*/
#define PROTOCOL_AUTH 1        /* 协议类型: 认证消息，客户端首次连接需发送认证令牌 */
#define PROTOCOL_DATA 2        /* 协议类型: 业务数据消息，如传感器数据 */
#define PROTOCOL_HEARTBEAT 3   /* 协议类型: 心跳保活消息，维持长连接 */
#define PROTOCOL_RESPONSE 4    /* 协议类型: 服务器响应消息，如AUTH_OK/ECHO等 */

/*==================== 认证配置常量 ====================*/
#define DEFAULT_AUTH_TOKEN "iot_gateway_2024"  //默认认证令牌，客户端需发送此令牌进行认证

/*==================== 函数声明 ====================*/
/*
 功能描述: 初始化TCP服务器
 参数: port - 监听端口
 返回值: 成功返回监听socket，失败返回-1
 */
int init_tcp_server(int port); 

/*
功能描述: 初始化UDP服务器
 参数: port - 监听端口
 返回值: 成功返回UDP socket，失败返回-1
 */
int init_udp_server(int port);  

/*
 功能描述: 设置文件描述符为非阻塞模式
 参数: fd - 文件描述符
 返回值: 成功0，失败-1
 */
int set_nonblocking(int fd);  

/*
 功能描述: 设置文件描述符的FD_CLOEXEC标志
 参数: fd - 文件描述符
 返回值: 成功0，失败-1
 */
int set_fd_cloexec(int fd);
  
/*
功能描述: 初始化UDP广播发送socket（用于向网络中的设备广播消息）
 参数: port - 广播端口
 返回值: 成功返回UDP广播socket，失败返回-1
 */
int init_udp_broadcast(int port);  // 新增

/*
功能描述: 发送UDP广播数据
 参数: fd - socket文件描述符
       data - 要发送的数据
       len - 数据长度
       port - 目标端口
 返回值: 成功返回发送字节数，失败返回-1
 */
int send_udp_broadcast(int fd, const char *data, int len, int port);  // 新增s
  
  

#endif  
