#include <stdio.h>

#define BUFSIZE 32

int mycat(const char *pathname)
{
	FILE *fp = NULL;//fp指针指向打开的文件
	int ch = 0;//存储读取出的数据
	char buf[BUFSIZE] = {0};//存储读取出的一串数据

	fp = fopec(pathname, "r");//通过fopen以r的模式打开pathname文件
	if(fp == NULL)//判断打开文件是否失败
	{
		perror("fopen()");
		return -1;
	}

	while(1)
	{

	}


}



int main(int argc[],char *argv[])
{
	

	if(argc < 2)
	{
		fprintf(stderr,"< %s + filename>\n",argv[0]);//打印使用说明
		return -1;//由于命令行参数不够,结束程序,并且返回-1
	}

	mycat(argv[1]);
	return 0;
}




