#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;//存储子进程的标识

	printf("----parent ----\n");
	printf("PID : %d PPID : %d PGID : %d SID : %d\n", \
		  getpid(), getppid(), getpgid(0), getsid(0));//打印父进程的信息
	printf("-----------------\n");

	pid = fork();
	if(pid < 0)//判断创建子进程是否失败
	{
		perror("fork()");
		exit(1);
	}

	if(pid == 0)//子进程的操作
	{
		printf("----child ----\n");
		printf("PID : %d PPID : %d PGID : %d SID : %d\n", \
				getpid(), getppid(), getpgid(0), getsid(0));//打印子进程的信息
		printf("-----------------\n");
		exit(0);
	}
	wait(NULL); //父进程等待子进程结束

	return 0;
}




