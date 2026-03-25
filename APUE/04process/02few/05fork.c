#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
	int num = 10;	//定义整形的变量，为了验证父子进程的独立性
	pid_t pid;	//存储子进程的标识PID
	printf("调用fork(2)之前，只有父进程(PID : %d), num = %d\n", getpid(), num);

	pid = fork();//创建子进程
	if(pid < 0)	//判断创建子进程是否失败
	{
		perror("fork()");
		exit(1);
	}

	//===========fork(2)成功，父子进程从这里开始分岔执行===========

	if(pid == 0)	//子进程的操作
	{
		num += 5;	//子进程修改自己的num变量
		printf("I am child process (PID :%d, PPID : %d) num = %d\n", getpid(), getppid(), num);
	}
	else //父进程操作
	{
		sleep(30);
		num -= 5; //父进程修改自己的num变量
		printf("I am Parent process (PID :%d,child process PID : %d) num = %d\n", getpid(), pid, num);

//	wait(NULL); //父进程等待子进程执行结束(收尸)

	}

	//父子进程都会执行这行代码(验证:同一份代码,父子进程各自执行一份)
	printf("PID : %d game Over num = %d\n", getpid(), num);

	exit(0);
}


