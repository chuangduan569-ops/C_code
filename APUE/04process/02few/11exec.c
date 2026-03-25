#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;	//
	char *str[] = {"ls", "-l", NULL}; //把选项参数合到一起

	printf("Begin...\n");

	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	
	if(pid == 0) //子进程的操作
	{
//		execl("/usr/bin/ls", "ls", "-l", NULL);	//使用ls-l替换子进程
	//	execlp("ls", "ls", "-l", NULL);
		execvp("ls", str);
		perror("execl()");
		exit(2);
	}

	wait(NULL); //等待子进程
	printf("End...\n");

	exit(0);
}


