
#ifndef __IOT_GATEWAY_H
#define __IOT_GATEWAY_H

/*==================== 系统头文件包含 ====================*/
#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <fcntl.h>      
#include <errno.h>      
#include <signal.h>    
#include <time.h>      
#include <sys/types.h>  
#include <sys/stat.h>   
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <poll.h>       
#include <pthread.h>    
#include <sys/wait.h>   
#include <stdarg.h>     /* 可变参数: va_list, va_start, va_arg, va_end */

/*==================== 全局宏定义 ====================*/
#define GATEWAY_VERSION "1.0.0"              /* 网关程序版本号 */

/*==================== 客户端连接结构体 ====================*/
/*
 * 客户端连接结构体
 * 用于管理每个TCP客户端连接的状态信息
 */
typedef struct client {
    int fd;                         /* 客户端socket文件描述符 */
    char rbuf[8192];                /* 读缓冲区: 存储从客户端接收的数据(8KB) */
    int rlen;                       /* 读缓冲区中有效数据长度 */
    char wbuf[8192];                /* 写缓冲区: 存储待发送给客户端的数据(8KB) */
    int wlen;                       /* 写缓冲区中待发送数据长度 */
    int authenticated;              /* 认证标志: 1=已认证, 0=未认证 */
    time_t last_active;             /* 最后活跃时间戳，用于心跳超时检测 */
    struct client *next;            /* 链表指针，指向下一个客户端节点 */
} client_t;

/*==================== 全局配置结构体 ====================*/
/*
 * 全局配置结构体
 * 存储从配置文件加载的所有配置项
 */
typedef struct {
    int tcp_port;               /* TCP服务监听端口，与云平台通信 */
    int udp_port;               /* UDP服务监听端口，与32开发板通信 */
    int udp_broadcast_port;     /* 新增：UDP广播端口 */
    int max_connections;        /* 最大允许的并发连接数 */
    int token_rate;             /* 令牌桶速率: 每秒生成的令牌数 */
    int token_burst;            /* 令牌桶容量: 最大突发流量上限 */
    char log_path[256];         /* 日志文件存储路径 */
    char auth_token[128];       /* 客户端认证令牌字符串 */
    char cloud_ip[64];          /* 东方瑞通ESLink云平台IP地址 */
    int cloud_port;             /* 云平台端口号 */
    int log_level;              /* 日志输出级别: 0=DEBUG,1=INFO,2=WARN,3=ERROR,4=FATAL */
} gateway_config_t;

/*==================== 全局变量声明 ====================*/
extern int g_running;               /* 程序运行标志: 1=运行中, 0=退出中 */
extern gateway_config_t g_config;   /* 全局配置对象，所有模块共享 */


#endif  /* __IOT_GATEWAY_H */
