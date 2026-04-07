//日志处理模块
//从共享内存读取日志，解析系统日志格式，只保留WARN/ERROR，发送消息队列

#include <stdio.h>
#include <string.h>
#include <unistd.h>     
#include <sys/wait.h>   
#include <time.h>       // 时间函数：time, localtime, strftime
#include <ctype.h>      // 字符处理：tolower 
#include "ipc.h"       
#include "msgq.h"       
#include <stdlib.h>

#define NUM_WORKERS 4	//工作进程数量
#define LINE_MAX 1024	//单行日志最大长度（字节）

int shmid; //共享内存ID
int semid; //信号量ID 
struct shm_buf *shm; //共享内存指针
int msqid;	// 消息队列ID

//辅助打印函数
static void log_msg(const char *msg)
{
    time_t now;                 /* 当前时间戳 */
    struct tm *tm_info;         /* 时间结构体 */
    char time_str[32];          /* 时间字符串缓冲区 */
    
    time(&now);
    tm_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(stderr, "[%s] [INFO] %s\n", time_str, msg);
    fflush(stderr);
}

/*
 get_level 从消息内容中判断日志级别
 参数 msg日志消息内容
返回值 ERROR WARN INFO
 */
static const char *get_level(const char *msg)
{
	char lower[256];//存放小写版本的信息
	int i = 0;
	//将消息转换为小写 方便比较(不区分大小写)
	for(i = 0; msg[i] && i < 255; i++)
	{
		lower[i] = tolower(msg[i]);
	}
	lower[i] = '\0';
	//检查ERROR关键字
	if(strstr(lower, "error") || strstr(lower, "fail") || strstr(lower, "fatal") || strstr(lower, "critical"))
		return "ERROR";
	//检查WARN关键字
	if(strstr(lower, "warn") || strstr(lower, "warning"))
		return "WARN";
	return "INFO";
}

/*
 worker 工作进程函数
参数 id 工作进程编号
 功能 每个worker独立运行，从共享内存读取日志并处理
使用sem_wait/sem_signal进行同步
 使用msgq_send发送消息
 */
static void worker(int id)
{
	char line[LINE_MAX];//存储一行原始日志
	char message[512];//存储消息内容 用于判断级别
	char worker_name[32];//工作进程名称字符串
	int read_pos; //共享内存读位置
	int i = 0;
	const char *level;//日志级别
	char *p;//指针p 用来查找冒号
	
	snprintf(worker_name, sizeof(worker_name), "Worker %d 启动", id);
	log_msg(worker_name);//打印辅助信息

	while(1)
	{
	//等待数据 P操作 等待数据可读
	sem_wait(semid);
	//从共享内存读取一行 
	read_pos = shm->read_pos; //获取当前读位置
	i = 0;//重置索引
	//循环读取直到遇到换行符或缓冲区满 
	while(shm->data[read_pos] != '\n' && i < LINE_MAX -1)
	{
		line[i++] = shm->data[read_pos];
		read_pos = (read_pos + 1) % SHM_SIZE; //移动到对应的位置
	}
	line[i] = '\0';//添加结束符字符串
	shm->read_pos = (read_pos + 1) % SHM_SIZE; //跳过换行符
	// V操作 解锁
	sem_signal(semid);
	//提取消息内容来判断日志级别
	p = strchr(line, ':');//查找冒号
	if(p)
	{
		p++;//跳过冒号
		while (*p == ' ') p++;      //跳过空格
		strncpy(message, p, sizeof(message) - 1);
		message[sizeof(message) - 1] = '\0';
	}
	else
		strncpy(message, line, sizeof(message) - 1);
	//判断日志级别
	level = get_level(message);
	//只保留 WARN 和 ERROR（文档要求）
	if(strcmp(level, "WARN") == 0)
        {
            // WARN级别：发送到消息队列，类型为2
            msgq_send(msqid, line, 2);
        }
        else if(strcmp(level, "ERROR") == 0)
        {
            // ERROR级别：发送到消息队列，类型为3
            msgq_send(msqid, line, 3);
        }
	}
	}
	//主函数
/*
 main 处理模块接口
功能 连接IPC资源，创建多个工作进程，实现并行处理
返回值 成功 0 失败 1
 */
int main(void)
{
	int i = 0;
	pid_t pids[NUM_WORKERS];//存储子进程PID
	log_msg("Processor 启动");

	//[1]连接IPC资源
	shmid = shm_create(SHM_KEY, sizeof(struct shm_buf));//获取共享内存
	shm = (struct shm_buf *)shm_attach(shmid);//映射共享内存
	semid = sem_create(SEM_KEY);//获取信号量
	msqid = msgq_create(MSG_KEY);//获取消息队列
	//[2]创建4个子进程
	for(i = 0; i < NUM_WORKERS; i++)
	{
		pids[i] = fork();//创建子进程
		if(pids[i] < 0)
		{
		perror("fork()");
		return -1;
		}
		if(pids[i] == 0)//子进程
		{
			worker(i);//调用工作函数
			exit(0);
		}
	}
	//[3] 父进程等待子进程退出
	for (i = 0; i < NUM_WORKERS; i++)
		wait(NULL);//等待子进程（正常情况下不会执行）

	return 0;
}















