#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(void)

{
	int i = 0;//循环变量
	int pfd[2];//存储pipe的读端和写端的文件描述符
	pid_t pid;//存储进程的ID号

	if(pipe(pfd) == -1)//判断创建是否失败
	{
		perror("pipe()");
		goto ERR_1;
	}

	for(i = 0; i < 2; i++)//循环创建子进程
	{
		pid = fork();//创建子进程
		if(pid < 0)//判断创建子进程是否失败
		{
			perror("fork()");
			goto ERR_2;
		}
		if(pid == 0)//子进程的操作
			break;
	}

	if(i == 0)//[1]号子进程的操作
	{
		close(pfd[0]);//关闭读端
		dup2(pfd[1], 1);//把1号文件描述符作为pfd[1]的复制
		execl("/usr/bin/ps", "ps", "ajx", NULL);//[1]号子进程替换进程
		close(pfd[1]);//[1]号子进程关闭写端
		exit(0);//终止[1]号子进程
	}

	if(i == 1)//[2]号子进程操作
	{
		close(pfd[1]);//关闭写端
		dup2(pfd[0], 0);//把0号文件描述符作为pfd[0]的赋值
		execl("/usr/bin/grep", "grep", "systemd", NULL);//[2]号进程替换进程
		close(pfd[0]);//关闭读端
		exit(0);//终止2号进程
	}

	//父进程的操作
	
	close(pfd[0]);
	close(pfd[1]);
	wait(NULL);
	wait(NULL);

	exit(0);

ERR_2:
    close(pfd[0]);//关闭pipe读端
    close(pfd[1]);//关闭pipe写端
ERR_1:
    exit(1);
	
}



