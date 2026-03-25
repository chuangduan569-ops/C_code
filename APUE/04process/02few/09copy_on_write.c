/*copy on write(写时复制)*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int i = 10010;	//存储特定值
	pid_t pid;	//存储子进程的标识
	
	pid = fork();	//创建子进程
	if(pid < 0) //判断子进程是否创建失败
	{
		perror("fork()");
		exit(1);
	}

	if(pid == 0) //子进程的操作
	{	
		printf("child.i = %p i = %d\n", &i, i);
		i = 10086;
		printf("child.i = %p i = %d\n", &i, i);
		exit(0); //终止子进程
	}
	else //父进程的操作
	{
		wait(NULL); //父进程等待子进程结束
		printf("parent.i = %p i = %d\n", &i, i);
		i = 12306;
		printf("parent.i = %p i = %d\n", &i, i);
	}




	return 0;
}



