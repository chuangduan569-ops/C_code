#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void close_file(void)
{
	printf("善后[1] ：关闭打开的文件\n");
}

void free_mem(void)
{
	printf("善后[2] ：释放开辟的空间\n");
}

void exit_begin(void)
{
	printf("善后[3] ：开始进行终止处理程序\n");
}

int main(void)
{
	if(atexit(close_file) != 0)
	{
    fprintf(stderr,"close_file is failed!\n");
    exit(1);
	}

	if(atexit(free_mem) != 0)
	{
		fprintf(stderr,"free_mem is failed!\n");
		exit(2);
	}

	if(atexit(exit_begin) != 0)
	{
    fprintf(stderr,"exit_begin is failed!\n");
    exit(3);
	}

	printf("整个程序的业务逻辑，执行中...\n");

	exit(0);//可以替换为return 0
}





