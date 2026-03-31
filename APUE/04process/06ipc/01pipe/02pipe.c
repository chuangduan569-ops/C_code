#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 32

int main(int argc, char *argv[])
{
	int pfd[2];//用来存储pipe的读端和写端
	int ret = 0;	
	pid_t pid;//存储子进程的ID号
	char buf[BUFSIZE] = {0};//存储读取到的数据

	if(pipe(pfd) == -1)//判断创建的匿名管道是否失败
	{
		perror("pipe()");
		goto ERR_1;
	}
	
	pid = fork();//创建子进程
	if(pid < 0)//判断创建子进程是否失败
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}

	if(pid == 0)//子进程操作
	{
		close(pfd[0]);//子进程关闭读端的文件描述符
		write(pfd[1], "hello world!", 12);//往pipe的写端写数据
		close(pfd[1]);//子进程关闭pipe的写端
		exit(0);

	}
	//父进程操作
	close(pfd[1]);//父进程关闭写端的文件描述符
	wait(NULL);//等待子进程结束(收尸)	
//	lseek(fd, SEEK_SET, 0);//把fd文件定位到起始位置
	read(pfd[0], buf, BUFSIZE);//从pipe的读端读取数据
	printf("Parent : %s\n", buf);//把子进程写入的数据父进程打印输出

	close(pfd[0]);//父进程关闭读端的文件描述符

	return 0;

ERR_2 :
	close(pfd[0]);//关闭pipe的读端
	close(pfd[1]);//关闭pipe的写端
ERR_1 :
	return ret;
}




