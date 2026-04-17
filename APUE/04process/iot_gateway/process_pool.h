#ifndef __PROCESS_POOL_H 
#define __PROCESS_POOL_H 

#include <sys/types.h>

#define PROCESS_POOL_SIZE 4 //进程池大小：同时运行的工作进程数量
#define FIFO_PATH "/tmp/iot_gateway_fifo_%d" //命名管道路径模板，%d会被worker_id替换，如/tmp/iot_gateway_fifo_0
#define CMD_BUFFER_SIZE 1024 //命令缓冲区大小(字节)，用于父子进程间传递命令

//进程任务结构体
//用于封装发送给工作进程的命令和数据
typedef struct
{
	int id; //任务ID，用于标识和追踪任务
	char cmd[256];//命令类型: PROCESS(处理任务), STOP(停止), STATUS(状态查询)等
	char data[768];//命令附带的数据，最大768字节
}process_task_t; //定义进程任务类型为process_task_t

/*
功能   初始化进程池
参数  worker_count - 工作进程数量
 返回值 ： 成功返回0   失败返回-1
*/
int process_pool_init(int worker_count);
/*
功能 发送任务给指定工作进程
参数 worker_id - 工作进程ID
	cmd - 命令字符串
	data - 命令数据，可以为NULL
 返回值 成功返回0   失败返回-1
*/
int process_pool_send_task(int worker_id, const char *cmd, const char *data);

/*
功能 销毁进程池
终止所有工作进程，关闭FIFO，释放资源
*/
void process_pool_destroy(void);

/*
功能 获取指定工作进程的PID
参数 worker_id - 工作进程ID
返回值  进程PID，失败返回-1
*/
pid_t process_pool_get_pid(int worker_id);





#endif
