/*父进程读argv[1]文件,发送给子进程,子进程接收到之后写到标准输出*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 128

static int cpfile(int rfd, int wfd)
{
	char buf[BUFSIZE] = {0};//存储读取出的数据
	int count = 0;//存储成功读取到的字节数
	
	while(1)
	{
		memset(buf, 0, BUFSIZE);//清空垃圾数据
		count = read(rfd, buf, BUFSIZE);//从rfd文件描述符中读取数据
		if(count == -1)//判断读取数据是否失败
		{
			perror("read()");
			return -1;
		}
		if(count == 0)//判断是否读取到文件结尾
			break;
		write(wfd, buf, count);//把buf中存储的数据写到wfd文件中
	}

	return 0;
}

int main(int argc, char *argv[])
{
    int fd = 0;//存储argv[1]文件的文件描述符
    int pfd[2];//存储pipe创建之后的读端和写端的文件描述符
    pid_t pid;//存储子进程的ID号

    if(argc < 2)//判断命令行参数的个数是否少于2个
    {
        printf("Usage : <%s + filename>\n", argv[0]);//打印使用说明
        goto ERR_1;//跳转到ERR_1的标识符
    }

    fd = open(argv[1], O_RDONLY);//以只读的形式打开argv[1]文件
    if(fd < 0)//判断打开文件是否失败
    {
        perror("open()");//打印错误信息
        goto ERR_1;//跳转到ERR_1的标识符
    }

    if(pipe(pfd) == -1)//判断创建pipe是否失败
    {
        perror("pipe()");//打印错误信息
        goto ERR_2;//跳转到ERR_2的标识符
    }

    pid = fork();//创建子进程
    if(pid < 0)//判断创建子进程是否失败
    {
        perror("fork()");//打印错误信息
        goto ERR_3;//跳转到ERR_3的标识符
    }

    if(pid == 0)//子进程的操作
    {
        close(pfd[1]);//子进程关闭写端的文件描述符
        cpfile(pfd[0], 1);//通过cpfile()完成读写操作,从pipe的读端读数据,写到1号文件描述符(标准输出)
        close(pfd[0]);//子进程关闭读端的文件描述符
        exit(0);//终止子进程
    }

    //父进程的操作
    close(pfd[0]);//父进程关闭读端的文件描述符
    cpfile(fd, pfd[1]);//读的是打开文件的文件描述符(argv[1]),写到pipe的写端
    close(pfd[1]);//父进程关闭写端的文件描述符
    wait(NULL);//等待子进程结束(收尸)

    close(fd);//关闭打开的文件

    exit(0);//终止父进程

ERR_3:
    close(pfd[0]);//关闭读端
    close(pfd[1]);//关闭写端
ERR_2:
    close(fd);//关闭打开的文件
ERR_1:
    exit(1);
}












