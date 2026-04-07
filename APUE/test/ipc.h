#ifndef __IPC_H
#define __IPC_H

//共享内存的大小 ：4MB
#define SHM_SIZE	(1024 * 1024 *4)

//信号量key值
#define SEM_KEY	0x1234

//共享内存key值
#define SHM_KEY	0x5678


struct shm_buf//共享内存缓冲区
{
	int write_pos;//写指针
	int read_pos;//读指针
	char data[SHM_SIZE];//数据区
};

int sem_create(int key);//创建或获取信号量,成功返回信号量ID

/*
P操作(加锁) 
semid 信号量ID
*/
void sem_wait(int semid);
/*
V操作(解锁) 
semid 信号量ID
*/
void sem_signal(int semid);

void sem_destroy(int semid);//销毁信号量

int shm_create(int key, size_t size);//创建共享内存

void *shm_attach(int shmid);//映射共享内存到进程的地址空间

void shm_detach(void *addr);//解除共享内存的映射 addr共享内存映射的地址

void shm_destroy(int shmid);//销毁共享内存

#endif




