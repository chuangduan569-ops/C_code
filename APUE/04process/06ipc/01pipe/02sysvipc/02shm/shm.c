#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int shm_id;//存储共享内存的标识符
	pid_t pid;//存储子进程ID号
	void *ptr = NULL;//存储映射到虚拟空间的地址
	int ret = 0;//存储错误玛


	//[1]创建共享内存 shmget(2)
	shm_id = shmget(IPC_PRIVATE, 1024, IPC_CREAT | IPC_EXCL | 0600);//创建共享内存
	if(shm_id == -1)//判断共享内存创建是否失败
	{
		if(errno == EEXIST)
		{
			shm_id = shmget(IPC_PRIVATE, 1024, 0);//获取已存在的共享内存标识
		}
		else{
		perror("shmget()");
		ret = -1;
		goto ERR_1;
		}
	}

	//[2]创建子进程
	pid = fork();
	if(pid == -1)
	{
		perror("fork()");
		ret = -2;
		goto ERR_2;
	}
	//[3]父子进程分别将共享内存映射到自己进程的虚拟地址空间 shmat(2)
	if(pid == 0)//子进程的操作
	{
		ptr = shmat(shm_id, NULL, 0);//子进程将共享内存映射到虚拟空间地址
		if(ptr == (void *)-1)//判断共享内存映射到虚拟地址空间是否失败
		{
			perror("shmat()");
		//	ret = -3;
		//	goto ERR_2;
			exit(1);//终止子进程
		}
	memcpy(ptr, "hello world!", 12);//往共享内存写入"hello world!"
	shmdt(ptr);//子进程接触映射
	exit(0);//终止子进程
	}

	//父进程的操作
	wait(NULL);//等待子进程结束(收尸)
	ptr = shmat(shm_id, NULL, 0);//父进程将共享内存映射到虚拟空间地址
	if(ptr == (void *)-1)//判断共享内存映射到虚拟地址空间是否失败
	{
		perror("shmat()");
		ret = -3;
		goto ERR_2;
	}
	puts(ptr);//把共享内存中的数据打印输出到标准输出中
	shmdt(ptr);//解除映射
	

ERR_2 :
	shmctl(shm_id, IPC_RMID, NULL);//销毁共享内存
ERR_1 :
	return ret;
}



