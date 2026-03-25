#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define DUP2

#define BUFSIZE 1024

static int mycat(const char *pathname)
{
	int fd = 0;
	int fdo = 0;	//fd变量用来保存输出文件的描述副
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
	fdo = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);	
	if(fdo < 0)
	{
		perror("open()");
		ret = -2;
		goto ERR_2;
	}

#if defined(DUP)
	//不是原子操作
	close(1);	//关闭1号文件描述符
	dup(fdo);	//把当前可用最小的文件描述符(1号)作为fdo的复制

#elif defined(DUP2)
	//原子操作
	dup2(fdo, 1);	//手动指定1号文件描述符复制给fdo
#endif

	while(1)
	{
	memset(buf,0,BUFSIZE);	//清空buf空间
	count = read(fd, buf, BUFSIZE);	//从文件中读取数据
	if(count == 0)	//判断是否读到了文件结尾
		break;
	else if(count < 0)	//判断是否读取错误
	{
		perror("read()");
		ret = -3;
		goto ERR_3;
	}
	write(1, buf, count);	//把buf存储的数据写入到1号文件中	
	}
ERR_3 :
	close(fdo);
ERR_2:
	close(fd);
ERR_1:
	return ret;

}


int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"Usage : <%s + filename>\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
	}
	mycat(argv[1]);	//调用mycat实现cat的功能
	return 0;
}


