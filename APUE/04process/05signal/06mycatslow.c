#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#define BUFSIZE 20

static int flag = 0;

static void sig_handler(int node)
{
	alarm(1);//设置1s的闹钟
	flag = 1;//设置flag的标记为1

}

static int mycat(const char *pathname)
{
	int fd = 0;
	char buf[BUFSIZE] = {0};
	size_t count = 0;
	int ret = 0;	//存储该函数的返回值

	fd = open(pathname, O_RDONLY);	//通过open()以只读的形式打开pathname文件	
	if(fd < 0)
	{
		perror("open()");
		ret = -1;
		goto ERR_1;
	}
	
	while(1)
	{
	while(!flag);//死等 等待法flag == 1
	flag = 0;//恢复标记的初始值
	memset(buf,0,BUFSIZE);	//清空buf空间
	count = read(fd, buf, BUFSIZE);	//从文件中读取数据
	if(count == 0)	//判断是否读到了文件结尾
		break;
	else if(count < 0)	//判断是否读取错误
	{
		perror("read()");
		ret = -2;
		goto ERR_2;
	}
	write(1, buf, count);	//把buf存储的数据写入到1号文件中	
	}
ERR_2:
	close(fd);
ERR_1:
	return ret;

}


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage : <%s + filename>\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
	}
	signal(SIGALRM, sig_handler); //给 SIGALRM 信号设置行为
	alarm(1);//设置1s的闹钟
	mycat(argv[1]);	//调用mycat实现cat的功能
	return 0;
}


