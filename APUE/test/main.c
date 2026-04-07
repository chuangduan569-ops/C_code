// 主程序  单实例守护进程的创建 创建IPC资源 启动子进程
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "ipc.h"
#include "msgq.h"

static int g_running = 1;//运行标志 1运行 0退出
static pid_t g_collector_pid = 0;//采集进程的PID
static pid_t g_processor_pid = 0;//处理进程的PID
static pid_t g_outputer_pid = 0;//输出进程的PID

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

//守护进程
/*
 daemonize 将程序变成单实力守护进程
功能 使程序在后台运行 脱离终端控制
步骤 fork→setsid→fork→chdir→umask→重定向
 */
static void daemonize(void)
{
	pid_t pid;//进程ID
	int fd;
	//[1] 第一次fork 脱离终端
	pid = fork();
	if(pid < 0)
		exit(1);
	if(pid > 0)
		exit(0);//父进程退出,子进程继续
	//[2] 创建会话 成为会话组长
	setsid();
	//[3] 第二次 fork 确定不会意外获得终端
	pid = fork();
	if(pid < 0)
		exit(1);
	if(pid > 0)
		exit(0);
	//[4] 改变工作目录到根目录
	chdir("/");
	//[5] 设置文件掩码
	umask(0);
	 //[6]将文件描述符 0 1 2 重定向到 "/dev/null"
    fd = open("/dev/null", O_RDWR);//以读写的形式打开"/dev/null"
    if(fd < 0)//判断打开文件是否失败
    {
        perror("open()");
        exit(2);
    }
    dup2(fd, 0);//把文件描述符0重定向到fd文件
    dup2(fd, 1);//把文件描述符1重定向到fd文件
    dup2(fd, 2);//把文件描述符2重定向到fd文件
    if(fd > 2)
        close(fd);
}	

/*
 create_ipc  创建ipc
 功能 创建共享内存 信号量 消息队列 并初始化共享内存
 */
static void create_ipc(void)
{
	int shmid;//共享内存ID
	struct shm_buf *shm;//共享内存指针
	shmid = shm_create(SHM_KEY, sizeof(struct shm_buf));//创建共享内存
	shm = (struct shm_buf *)shm_attach(shmid);//映射共享内存
	//初始化共享内存 写指针=0 读指针=0 数据区清0
	shm->write_pos = 0;
	shm->read_pos = 0;
	memset(shm->data, 0, SHM_SIZE);
	shm_detach(shm);//解除映射
	sem_create(SEM_KEY);//创建信号量(互斥量)
	msgq_create(MSG_KEY);//创建消息队列
	log_msg("IPC资源创建成功");
}
	//子进程管理
/*
 start_children 启动子进程
 功能 fork出collector、processor、outputer三个进程
 */
static void start_children(void)
{
	//启动采集进程
	g_collector_pid = fork();
	if(g_collector_pid == 0)//子进程操作
	{
		execl("./collector", "collector", NULL);//执行collector程序
		exit(1);
	}
	//启动处理模块
	g_processor_pid = fork();
	if(g_processor_pid == 0)
	{
		execl("./processor", "processor", NULL); 
		exit(1);
	}
	//启动输出模块
	g_outputer_pid = fork();
	if(g_outputer_pid == 0)
	{
		execl("./outputer", "outputer", NULL);
		exit(1);
	}
	log_msg("子进程启动成功");
}
/*
 stop_childern 停止所有子进程
 功能 发送SIGTERM信号给子进程 等待退出
 */
static void stop_children(void)
{
	//通知SIGTERM信号 通知子进程退出
	if(g_collector_pid > 0)
		kill(g_collector_pid, SIGTERM);
	if(g_processor_pid > 0)
		kill(g_processor_pid, SIGTERM);
	if(g_outputer_pid > 0)
		kill(g_outputer_pid, SIGTERM);
	//等待500毫秒 子进程清理资源
	usleep(500000);
	//等待子进程退出 回收资源
	if(g_collector_pid > 0)
		waitpid(g_collector_pid, NULL, WNOHANG);
	if(g_processor_pid > 0)
		waitpid(g_processor_pid, NULL, WNOHANG);
	if(g_outputer_pid > 0)
		waitpid(g_outputer_pid, NULL, WNOHANG);
}
/*
 cleanup 清理所有资源
 功能 停止子进程 销毁IPC资源
 */
static void cleanup(void)
{
	log_msg("正在清理资源...");
	//停止所有子进程
	stop_children();
	int shmid = shm_create(SHM_KEY, 0);//获取共享内存ID
	if(shmid >= 0)
		shm_destroy(shmid);//销毁共享内存
	int semid = sem_create(SEM_KEY);//获取信号量ID
	if(semid >= 0)
		sem_destroy(semid);//销毁
	int msqid = msgq_create(MSG_KEY);//获取消息队列ID
	if(msqid >= 0)
		msgq_destroy(msqid);//销毁
	log_msg("清理完成");
}

//信号处理
/*
 signal_handler 信号处理函数
 参数 signo  信号编号
 功能 收到退出信号时设置退出标志
 */
static void signal_handler(int signo)
{
	g_running = 0;//设置运行标志为0 通知主循环退出
}

int main(int argc, char *argv[])
{
	int daemon_mode = 0;//守护进程模式标志 0前台 1后台
	//解析命令行参数：如果带 -d 参数，则以后台守护进程模式运行 
	if(argc >= 2 && strcmp(argv[1], "-d") == 0)
		daemon_mode = 1;
	//[1] 创建IPC资源
	create_ipc();
	//[2] 单实例进程守护
	if(daemon_mode)
		daemonize();//后台运行
	//[3] 设置信号处理
	signal(SIGTERM, signal_handler);//终止信号
	signal(SIGINT, signal_handler);//ctrl + c中断信号
	signal(SIGCHLD, SIG_IGN);//忽略子进程退出信号 由wait处理
	//[4] 启动子进程
	start_children();
	
	//打印启动成功日志
	log_msg("LogHawk 启动成功");
	if(daemon_mode)
		log_msg("守护进程模式运行中...");
	else
		log_msg("前台模式运行中，按 Ctrl+C 退出");
	
	//[5]主循环等待退出信号
	while(g_running)
		sleep(1);//每秒检查一次退出标志
	//[6] 清理资源并退出
	cleanup();
	log_msg("LogHawk 已停止");
	return 0;
}




