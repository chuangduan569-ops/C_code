	/*使用read write实现简单的mycp*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

static int mycp(const char *src, const char *dest)
{
	int fds = 0;	//存储打开源文件的文件描述符
	int fdd = 0;	//存储打开目标文件的文件描述符
	int ret = 0;
	char buf[BUFSIZE] = {0};	//存储读取到的数据
	size_t count = 0;	//存储成功读取到的字节数

	fds = open(src, O_RDONLY);	//通过open()以只读的形式打开src文件
	if(fds < 0)//判断打开文件是否失败
	{
		perror("open()");//打印错误信息
		ret = -1;//由于打开文件失败,给ret赋值-1
		goto ERR_1;//跳转到ERR_1的标识符位置
	}
	
	fdd = open(dest, O_WRONLY | O_CREAT |O_TRUNC, 0666);//通过open打开目标文件
	if(fdd < 0)//判断打开文件是否失败
	{
		perror("open()");//打印错误信息
		ret = -2;//由于打开文件失败,给ret赋值-2
		goto ERR_2;//跳转到ERR_2的标识符位置
	}

	while(1)
	{
		memset(buf, 0, BUFSIZE);
		count = read(fds, buf, BUFSIZE);//从文件中读取数据
		if(count == 0)//判断是否读到了文件结尾
			break;//跳出死循环
		else if(count < 0)//判断是否读取错误
		{
			perror("read()");//打印错误信息
			ret = -3;//由于读取错误,,给ret赋值-3
			goto ERR_3;//跳转到ERR_3的标识符位置
		}
		write(fdd, buf, count);	//把buf存储的数据写入到目标文件中
    }
	


ERR_3 :
	close(fdd);//关闭目标文件
ERR_2 :
	close(fds);//关闭源文件
ERR_1 :
	return ret;
}



int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"Usage : <%s + srcfile + destfile>\n", argv[0]);
		return -1;
	}
	
	mycp(argv[1], argv[2]); //调用自己的mycp函数

	return 0;
}




