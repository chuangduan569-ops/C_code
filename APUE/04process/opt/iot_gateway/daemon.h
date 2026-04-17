
#ifndef __DAEMON_H
#define __DAEMON_H

/*==================== 守护进程配置常量 ====================*/
#define PID_FILE_PATH "/var/run/iot_gateway.pid"  /* PID文件路径，用于单实例控制 */

/*==================== 函数声明 ====================*/
/*
 * 函数名称: daemonize
 * 功能描述: 将当前进程变为守护进程(后台服务进程)
 *          实现标准守护进程化流程: fork → setsid → 二次fork → chdir → umask → 重定向
 * 文档要求: 实现标准守护进程化流程
 * 参数: 无
 * 返回值: 成功返回0，失败返回-1
 */
int daemonize(void);

/*
 * 函数名称: create_pid_file
 * 功能描述: 创建PID文件并加锁，防止程序多实例运行
 *          将当前进程的PID写入文件，使用O_EXCL标志确保唯一性
 * 文档要求: 生成PID文件并加锁，防止多实例运行
 * 参数: 无
 * 返回值: 成功返回0，失败返回-1
 */
int create_pid_file(void);

/*
 * 函数名称: remove_pid_file
 * 功能描述: 删除PID文件，程序退出时调用
 *          删除后允许下次启动新的实例
 * 参数: 无
 * 返回值: 无
 */
void remove_pid_file(void);

#endif  
