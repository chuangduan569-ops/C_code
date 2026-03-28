/*使用系统调用IO函数实现mycat*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "tbf.h"

#define BUFSIZE 16
#define CPS		BUFSIZE//速率(请求取令牌的速率也是积攒令牌的速率)
#define BURST	(CPS) * 20 //令牌桶中令牌数量的上限
//(上限制和速率尽量是整数倍的关系)

static int token = 0;//需要使用token表示令牌的个数

static int mycat(int fd, int td)
{
	char buf[BUFSIZE] = {0};//存储读取到的数据
	int count = 0;//存储成功读取到的字节数
	int n = 0;//要读取数据的字节数

	while(1)
	{
		n = tbf_fetch_token(td, BUFSIZE);//获取td令牌桶中的token
		memset(buf, 0, BUFSIZE);//清空脏数据
		count = read(fd, buf, n);//从文件中读取数据(按n的大小读取)
		if(count == 0)//判断是否读到了文件结尾的位置
			break;//跳出死循环
		else if(count < 0)//判断是否读取失败
		{
			perror("read()");//打印错误信息
			return -1;//由于读取失败,结束函数,并且返回-1
		}
		write(1, buf, count);//把数据写入到标准输出文件中
	}
}

int main(int argc, char *argv[])
{
	int fd = 0;//fd变量用来保存打开文件的文件描述符
	int td = 0;//存储令牌描述符

	if(argc < 2)//判断命令行参数个数是否少于2个
	{
		fprintf(stderr, "Usage : %s + filename\n", argv[0]);//打印使用说明
		return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
	}
	
	td = tbf_init(CPS, BURST);//初始化令牌桶
	if(td < 0)//判断初始化是否失败
	{
		fprintf(stderr, "tbf Init Is Failed!\n");//打印错误信息
		return -3;
	}

	fd = open(argv[1], O_RDONLY);//通过open(2)以只读的形式打开文件
	if(fd < 0)//判断打开文件是否失败
	{
		perror("open()");//打印错误信息
		return -2;//由于打开文件失败,结束程序,并且返回-2
	}

	mycat(fd, td);//调用自己实现的mycat()

	close(fd);//通过close(2)关闭文件

	tbf_destroy(td);
	return 0;
}
