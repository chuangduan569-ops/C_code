#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define BUFSIZE 128

int main(void)
{
	FILE *fp = NULL;//fp指针指向要打开的文件
	time_t tm;	//存储时间戳
	struct tm *time_st = NULL;//存储格式化时间的结构体
	char buf[BUFSIZE] = {0};//存储格式化时间的字符串

	fp = fopen("/tmp/out", "w");//打开文件
	if(fp == NULL)
	{
		perror("fopen()");
		exit(1);
	}

	while(1)
	{
		if(time(&tm) == (time_t) -1)//判断获取当前时间戳是否失败
		{
			perror("time()");
			fclose(fp);
			exit(2);
		}
			
		time_st = localtime(&tm);
		if(time_st == NULL)//判断把时间戳转换成格式化时间是否失败
		{
			perror("localtime()");
			fclose(fp);
			exit(3);
		}

		memset(buf, 0, BUFSIZE);
		strftime(buf, BUFSIZE, "%Y年%m月%d日 %H:%M:%S\n", time_st);
		//把格式化时间转换为字符串
		fputs(buf,fp);//把buf存储的字符串写入fp指针指向的文件流中
		fflush(NULL);//刷新缓冲区
		sleep(1);//睡1s
	}

	fp = NULL;//野指针
	return 0;
}


