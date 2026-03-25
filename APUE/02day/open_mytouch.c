/*通过open(2) close(2) 实现mytouch的功能*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc,char *argv[])
{
	int fd = 0;	//创建文件描述符

	if(argc < 2)
	{
		fprintf(stderr,"Usage : %s + filename>\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
	}

	for(int i = 1; i < argc; i++)
	{
	fd = open(argv[i],O_WRONLY | O_CREAT, 0666);	//打开文件，并设置权限
	if(fd < 0)	//打开失败
	{
		perror("open()");	//打印错误信息
		return -2;
	}
		close(fd);	//关闭文件
	}


	return 0;
}


