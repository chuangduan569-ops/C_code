
#ifndef __IO_MULTIPLEXING_H
#define __IO_MULTIPLEXING_H

#include "iot_gateway.h"  
#include "thread_pool.h" 
/*==================== IO多路复用配置常量 ====================*/
#define MAX_POLL_FDS 10000          //poll监听的最大文件描述符数量，支持5000+连接 
#define POLL_TIMEOUT_MS 1000        //poll超时时间(毫秒)，1秒 

/*==================== 函数声明 ====================*/
/*
 功能描述: 主事件循环，基于poll实现
           同时监听TCP监听socket、UDP socket、信号管道、所有客户端连接
  参数: tcp_fd - TCP监听socket
        udp_fd - UDP socket
        signal_fd - 信号管道读端
        token_bucket_id - 令牌桶ID，用于UDP限流
        max_connections - 最大连接数
        p_running - 程序运行标志指针
 * 返回值: 成功返回0，失败返回-1
 */
int io_multiplexing_loop(int tcp_fd, int udp_fd, int signal_fd,
                          int token_bucket_id, int max_connections, int *p_running);

/*
  功能描述: 获取当前活跃的客户端连接数
  参数: 无
  返回值: 当前连接数
 */
int get_connection_count(void);

/*
  功能描述: 关闭所有客户端连接
  参数: 无
  返回值: 无
 */
void close_all_connections(void);

void set_io_thread_pool(pool_t *pool);  // 新增：设置线程池的函数声明

void set_udp_broadcast_fd(int fd);  // 设置广播socket

#endif  
