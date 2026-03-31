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
	int fd = 0;//用来存储打开文件的文件描述符
	int ret = 0;	
	pid_t pid;//存储子进程的ID号
	char buf[BUFSIZE] = {0};//存储读取到的数据

	fd = open("./hello", O_RDWR | O_CREAT | O_TRUNC, 0666);//打开文件
	if(fd < 0)//判断文件打开是否失败
	{
		perror("open()");
		ret = -1;
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
		write(fd, "hello world!", 12);//往fd中写数据
		close(fd);
		exit(0);
	}
	//父进程操作
	wait(NULL);//等待子进程结束(收尸)	
	lseek(fd, SEEK_SET, 0);//把fd文件定位到起始位置
	read(fd, buf, BUFSIZE);//读取fd文件的数据
	printf("Parent : %s\n", buf);//把子进程写入的数据父进程打印输出



ERR_2 :
	close(fd);
ERR_1 :
	return ret;
}




