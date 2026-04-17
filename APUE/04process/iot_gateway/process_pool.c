#include "iot_gateway.h"
#include "process_pool.h"
#include "log.h"
#include <sys/wait.h>
#include <signal.h> 
static pid_t *g_worker_pids = NULL;//工作进程PID数组指针，动态分配
static int g_worker_count = 0;//工作进程数量，由process_pool_init设置 
static int *g_fifo_fds = NULL;//FIFO文件描述符数组(父进程端)，用于向子进程写命令 
//static int g_running = 1; //进程池运行标志: 1=运行中, 0=已停止 

static void sigchld_handler(int none)
{
	pid_t pid;//退出的子进程PID 
	int status;//子进程退出状态
	//waitpid(-1, &status, WNOHANG) 等待任意子进程，不阻塞
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		//WIFEXITED(status) 判断子进程是否正常退出
		if(WIFEXITED(status))
			LOG_INFO("Child process %d terminated normally, exit code=%d",pid, WEXITSTATUS(status));//WEXITSTATUS获取退出码
		else if(WIFSIGNALED(status))//WIFSIGNALED(status) 判断子进程是否被信号终止
			LOG_INFO("Child process %d terminated by signal %d",pid, WTERMSIG(status));//WTERMSIG获取终止信号编号
	}
}

int process_pool_init(int worker_count)
{
	int i = 0;
	char fifo_path[256];// FIFO完整路径缓冲区 
	struct sigaction sa;//信号动作结构体，用于设置SIGCHLD处理
	//参数合法性检查：工作进程数量应在1到100之间
	if (worker_count <= 0 || worker_count > 100) 
	{
        LOG_ERROR("process_pool_init: invalid worker_count=%d", worker_count);
        return -1;
    }
	g_worker_count = worker_count;//保存工作进程数量到全局变量
	//分配内存: 存储子进程PID数组
	g_worker_pids = (pid_t *)calloc(worker_count, sizeof(pid_t));
	//分配内存: 存储FIFO文件描述符数组
	g_fifo_fds = (int *)calloc(worker_count, sizeof(int));

	if(g_worker_pids == NULL || g_fifo_fds == NULL)//判断是否分配成功
	{
		LOG_ERROR("process_pool_init: failed to allocate memory");
		if (g_worker_pids) 
			free(g_worker_pids);//释放已分配的内存
		if (g_fifo_fds) 
			free(g_fifo_fds);//释放已分配的内存
		return -2;
	}
	//设置SIGCHLD信号处理，自动回收子进程，防止僵尸进程
	sa.sa_handler = sigchld_handler;//信号处理函数指针
	sa.sa_flags = SA_RESTART;//SA_RESTART: 被信号中断的系统调用自动重启
	sigaction(SIGCHLD, &sa, NULL);//册SIGCHLD信号处理函数
	//创建worker_count个工作进程
	for(i = 0; i < worker_count; i++)
	{
		//构造FIFO路径: /tmp/iot_gateway_fifo_0, /tmp/iot_gateway_fifo_1, ...
		snprintf(fifo_path, sizeof(fifo_path), FIFO_PATH, i);
		unlink(fifo_path);//删除可能存在的旧FIFO文件，避免冲突
		//创建命名管道(FIFO)
		//mkfifo()创建FIFO特殊文件，0666权限: 所有者/组/其他都可读写
		if(mkfifo(fifo_path, 0666) < 0)
		{
			LOG_ERROR("process_pool_init: failed to create FIFO %s: %s",fifo_path, strerror(errno));
			return -3;
		}
		//创建子进程
		g_worker_pids[i] = fork();
		if(g_worker_pids[i] < 0)//创建失败
		{
			LOG_ERROR("process_pool_init: fork failed: %s", strerror(errno));
			return -4;
		}
		if(g_worker_pids[i] == 0)//子进程操作
		{
			char buf[CMD_BUFFER_SIZE];//命令缓冲区
			int fd;//FIFO文件描述符
			ssize_t n;//取的字节数
			int j = 0;//循环变量
			//关闭父进程中已打开的其他FIFO写端
			for(j = 0; j < i; j++)
			{
				if(g_fifo_fds[j] > 0)
					close(g_fifo_fds[j]);
			}
			//打开FIFO用于读取父进程发送的命令
			fd = open(fifo_path, O_RDONLY);
			if(fd < 0)
			{
				LOG_ERROR("Worker %d: failed to open FIFO for reading", i);
				exit(1);
			}

			//子进程主循环: 持续读取命令并处理 
			while(g_running)
			{
				//从FIFO读取命令，阻塞直到有数据可读
				n = read(fd, buf, sizeof(buf) - 1);
				if(n > 0)//读到数据
				{
					buf[n] = '\0';
					//解析并执行命令
					if(strstr(buf, "CMD:PROCESS") != NULL)
					{
						//处理任务: 模拟执行，休眠1秒
						sleep(1);
						LOG_INFO("Worker %d: task processed", i);
					}
					else if(strstr(buf, "CMD:STOP") != NULL)
					{
						//收到停止命令，退出循环
						LOG_INFO("Worker %d: received stop command", i);
						break;
					}
				}
				else if(n == 0)
				{
					//read返回0表示FIFO被关闭，父进程已退出
					LOG_INFO("Worker %d: FIFO closed, exiting", i);
					break;
				}
				else if(n < 0 && errno != EINTR)
				{
					//read出错且不是被信号中断
					LOG_ERROR("Worker %d: read error: %s", i, strerror(errno));
					break;
				}
			}
			close(fd);//关闭fifo
			exit(0);
		}
		else  //父进程操作
		{
			//打开FIFO用于向子进程写入命令
			g_fifo_fds[i] = open(fifo_path, O_WRONLY);
			if(g_fifo_fds[i] < 0)
			{
				LOG_ERROR("process_pool_init: failed to open FIFO for writing: %s",strerror(errno));
				return -5;
			}
		}
	}
	return 0;
}

int process_pool_send_task(int worker_id, const char *cmd, const char *data)
{
	char buffer[CMD_BUFFER_SIZE];//命令缓冲区 
	ssize_t written; //写入的字节数 

	//参数合法性检查
	if (worker_id < 0 || worker_id >= g_worker_count)
		return -1;
	if (cmd == NULL)
		return -2;
	//格式化命令: 如果有data则格式为"CMD:xxx DATA:xxx\n"，否则为"CMD:xxx\n"
	if (data != NULL)
		snprintf(buffer, sizeof(buffer), "CMD:%s DATA:%s\n", cmd, data);
	else
		snprintf(buffer, sizeof(buffer), "CMD:%s\n", cmd);

	written = write(g_fifo_fds[worker_id], buffer, strlen(buffer));//写入FIFO，子进程会读取并处理
	if(written < 0)
	{
		LOG_ERROR("process_pool_send_task: write to worker %d failed: %s",worker_id, strerror(errno));
		return -1;
	}
	return 0;
}

void process_pool_destroy(void)
{
	int i = 0;
	char fifo_path[256];//FIFO路径缓冲区
	g_running = 0;//通知子进程退出
	//终止所有工作进程
	for(i = 0; i < g_worker_count; i++)
	{
		if(g_worker_pids != NULL && g_worker_pids[i] > 0)
		{
			process_pool_send_task(i, "STOP", NULL);//发送STOP命令
			usleep(100000);//等待100毫秒，给子进程退出时间
			//如果还没退出，强制终止
			if(kill(g_worker_pids[i], SIGTERM) == 0)
				LOG_INFO("Sent SIGTERM to worker %d (PID=%d)", i, g_worker_pids[i]);
		}
		//关闭FIFO文件描述符
		if(g_fifo_fds != NULL && g_fifo_fds[i] > 0)
			close(g_fifo_fds[i]);//关闭写端 
		snprintf(fifo_path, sizeof(fifo_path), FIFO_PATH, i);//删除FIFO文件
		unlink(fifo_path);//删除FIFO特殊文件
	}
	sleep(1);//等待子进程退出
	//释放动态分配的内存
	if(g_worker_pids != NULL)
	{
		free(g_worker_pids);//释放PID数组
		g_worker_pids = NULL;
	}
	if(g_fifo_fds != NULL)
	{
		free(g_fifo_fds);//释放FIFO描述符数组 
		g_fifo_fds = NULL;
	}
	LOG_INFO("Process pool destroyed");//记录销毁完成日志
}

pid_t process_pool_get_pid(int worker_id)
{
	return g_worker_pids[worker_id];//返回指定工作进程的PID
}


