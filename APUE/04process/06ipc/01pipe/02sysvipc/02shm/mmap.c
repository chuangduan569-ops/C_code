#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define LENGTH 1024

int main(void)
{
	void *ptr = NULL;//指向映射的地址空间
	int ret = 0;	
	pid_t pid;//存储子进程的标识

	ptr = mmap(NULL, LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//映射共享内存
	if(ptr == MAP_FAILED)
	{
		perror("mmap()");
		ret = -1;
		goto ERR_1;
	}
	//创建子进程
	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}
	//父子进程通过共享内存进行通信
	if(pid == 0)//子进程的操作
	{
	memcpy(ptr, "Easthome", 8);//子进程往共享内存写入easthome
	munmap(ptr, LENGTH);//解除映射
	exit(0);//终止子进程
	}
	
	//父进程的操作
	wait(NULL);//等待子进程的结束
	puts(ptr);//把共享内存中的数据打印到标准输出



ERR_2 :
	munmap(ptr, LENGTH);//解除映射
ERR_1 :
	return ret;
}

