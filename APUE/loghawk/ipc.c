#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include "ipc.h"

int sem_create(int key)
{
	int semid;//信号量ID
	union semun
	{
		int val;//信号量的初始值
		struct semid_ds *buf; //信号量的状态
		short *array;//信号量数组
	}arg;
	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);//创建1个信号量
	if(semid >= 0)//创建成功
	{
		arg.val = 1;//初始值设为1(互斥锁)
		semctl(semid, 0, SETVAL, arg);//SETVAL命令设置信号量值 
		return semid;
	}
	//如果创建失败(信号量已存在)
	semid = semget(key, 1, 0666);
	return semid;
}

//P操作
void sem_wait(int semid)
{
	struct sembuf sb;//信号量操作结构体
	sb.sem_num = 0;//操作第0个信号量
	sb.sem_op = -1;//-1表示P操作，申请一个资源
	sb.sem_flg = SEM_UNDO;//进程退出时自动恢复信号量
	semop(semid, &sb, 1);//执行P操作 信号量ID，操作数组，操作数量
}
//V操作
void sem_signal(int semid)
{
	struct sembuf sb;//信号量操作结构体
	sb.sem_num = 0;//操作第0个信号量
	sb.sem_op = 1;//1表示V操作，释放一个资源
	sb.sem_flg = SEM_UNDO;//进程退出时自动恢复信号量
	semop(semid, &sb, 1);//执行V操作 信号量ID，操作数组，操作数量
}

void sem_destroy(int semid)
{
	if(semid >= 0)
		semctl(semid, 0, IPC_RMID);//销毁信号量
}

int shm_create(int key, size_t size)//创建共享内存
{
	int shmid;//共享内存ID
	shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);//创建共享内存
	if(shmid >= 0)
		return shmid;//创建成功,返回ID

	//创建失败(已存在),获取已存在的共享内存
	shmid = shmget(key, size, 0666);
	return shmid;//返回已存在的共享内存ID
}
//映射共享内存到进程地址空间
void *shm_attach(int shmid)
{
	void *addr;//映射后的内存地址
	addr = shmat(shmid, NULL, 0);//映射共享内存
	if(addr == (void *)-1)//映射失败
		return NULL;
	return addr;//返回映射地址
}
//解除共享内存映射
void shm_detach(void *addr)
{
	if(addr)
	shmdt(addr);//解除共享内存映射
}
//销毁共享内存
void shm_destroy(int shmid)
{
	if(shmid >= 0)
	shmctl(shmid, IPC_RMID, NULL);//销毁共享内存
}




















