#include <stdio.h>
#include <unistd.h>

int main(void)
{

	printf("begin...\n");//打印开始

	execl("/usr/bin/ls", "ls", "-l", NULL);//使用ls -l 替换当前调用进程

	printf("End...\n");//打印结束

	return 0;
}




