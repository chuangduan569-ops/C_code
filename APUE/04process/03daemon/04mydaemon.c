#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 128

static int mydaemon(void)
{
	pid_t pid;//存储子进程的标识
	int fd = 0; //存储打开“/dev/null”文件的文件描述符	

	pid = fork();//创建子进程
	if(pid < 0)
	{
		perror("fork()");
		exit(6);
	}

	if(pid > 0)//父进程的操作
		_exit(0);//终止父进程(不需要调用终止处理函数)

	if(setsid() == (pid_t) - 1) //判断创建一个人新的会话是否失败
	{
		perror("setsid()");
		exit(7);
	}

	//执行到此，当前子进程就是新的会话的进程组的组长进程
	//此时 子进程的PID == PGID == SID  而且脱离了控制终端
	//往下可以继续做 [4] [5] [6] 的操作
	//[4]mask
	umask(0);

	//[5]
	if(chdir("/") == -1)//判断当前工作目录切换到根目录是否失败
	{
		perror("chdir()");
		exit(8);
	}

	//[6]将文件描述符 0 1 2 重定向到 "/dev/null"
	fd = open("/dev/null", O_RDWR);//以读写的形式打开"/dev/null"
	if(fd < 0)
	{
		perror("open()");
		exit(9);
	}
	dup2(fd, 0);//把文件描述符0作为fd的重定向
	dup2(fd, 1);//把文件描述符1作为fd的重定向
	dup2(fd, 2);//把文件描述符2作为fd的重定向
	if(fd > 2)
		close(fd);

	return 0;
}

int main(void)
{
	FILE *fp = NULL;//fp指针指向要打开的文件
	time_t tm;	//存储时间戳
	struct tm *time_st = NULL;//存储格式化时间的结构体
	char buf[BUFSIZE] = {0};//存储格式化时间的字符串

	if(mydaemon() < 0) //调用我们自己封装的方法
	{
		fprintf(stderr, "Mydaemon Failed!\n");
		exit(5);
	
	}


	fp = fopen("/tmp/out", "w");//打开文件
	if(fp == NULL)
	{
		perror("fopen()");
		exit(4);
	}

	while(1)
	{
		if(time(&tm) == (time_t) -1)//判断获取当前时间戳是否失败
		{
			perror("time()");
			fclose(fp);
			exit(2);
		}
			
		time_st = localtime(&tm);
		if(time_st == NULL)//判断把时间戳转换成格式化时间是否失败
		{
			perror("localtime()");
			fclose(fp);
			exit(3);
		}

		memset(buf, 0, BUFSIZE);
		strftime(buf, BUFSIZE, "%Y年%m月%d日 %H:%M:%S\n", time_st);
		//把格式化时间转换为字符串
		fputs(buf,fp);//把buf存储的字符串写入fp指针指向的文件流中
		fflush(NULL);//刷新缓冲区
		sleep(1);//睡1s
	}

	fp = NULL;//野指针
	return 0;
}


