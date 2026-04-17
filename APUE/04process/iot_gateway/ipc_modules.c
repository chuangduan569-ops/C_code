#include "iot_gateway.h"
#include "ipc_modules.h"
#include "log.h"

//消息队列的实现
int msgq_create(key_t key)
{
	int msqid;//消息队列ID
	// msgget()创建消息队列，IPC_CREAT表示不存在则创建，0666权限 
	msqid = msgget(key, IPC_CREAT | 0666);
	if(msqid < 0)
	{
		LOG_ERROR("msgget failed for key 0x%x: %s", key, strerror(errno));
		return -1;
	}
	return msqid;//返回消息队列ID
}

int msgq_send(int msqid, const void *msg, size_t size, long type)
{
	// msgsnd()发送消息，flags=0表示阻塞发送直到消息被接收
	if(msgsnd(msqid, msg, size, 0) < 0)
	{
		LOG_ERROR("msgsnd failed: %s", strerror(errno));
		return -2;
	}
	return 0;
}

int msgq_recv(int msqid, void *msg, size_t size, long type)
{
	size_t ret;//接收到的消息长度
	//msgrcv()接收消息，flags=0表示阻塞接收
	ret = msgrcv(msqid, msg, size, type, 0);
	if(ret == -1)//判断接收是否失败
	{
		if(errno != EINTR)//如果不是被信号中断
		{
			LOG_ERROR("msgrcv failed: %s", strerror(errno));
		}
		return -1;
	}
	return (int)ret;//返回接收到的字节数
}

int msgq_destroy(int msqid)
{
	//msgctl()控制消息队列，IPC_RMID命令删除消息队列
	if(msgctl(msqid, IPC_RMID, NULL) < 0)
	{
		LOG_ERROR("msgctl IPC_RMID failed: %s", strerror(errno));
		return -1;
	}
	return 0;
}

//共享内存实现 

int shm_create(key_t key, size_t size)
{
	int shmid;//共享内存ID
	//创建新的共享内存	
	shmid = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
	if(shmid < 0)
	{
		LOG_ERROR("shmget failed for key 0x%x: %s", key, strerror(errno));
		return -1;
	}
	return shmid;//返回共享内存ID
}

//映射地址
void *shm_attach(int shmid)
{
	void *addr;//映射后的内存地址
	//shmat()附加共享内存，第二个参数为NULL表示系统自动选择地址
	addr = shmat(shmid, NULL, 0);
	if(addr == (void *)-1)
	{
		LOG_ERROR("shmat failed for shmid=%d: %s", shmid, strerror(errno));
		return NULL;
	}
	return addr;//返回映射地址
}

//解除共享内存与当前进程的映射
int shm_detach(void *addr)
{
	//shmdt()解除共享内存映射
	if(shmdt(addr) < 0)
	{
		LOG_ERROR("shmdt failed for addr=%p: %s", addr, strerror(errno));
		return -1;
	}
	return 0;
}

//销毁共享内存
int shm_destroy(int shmid)
{
	//shmctl()控制共享内存，IPC_RMID命令删除共享内存
	if(shmctl(shmid, IPC_RMID, NULL) < 0)
	{
		LOG_ERROR("shmctl IPC_RMID failed for shmid=%d: %s", shmid, strerror(errno));
		return -1;
	}
	return 0;
}

//信号量实现 
//创建或获取信号量数组
int sem_create(key_t key, int nsems, unsigned short *init_val)
{
	int semid;//信号量ID
	int i = 0;
	//创建新的信号量集合 
	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);
	if(semid < 0)
	{
		semid = semget(key, nsems, 0666);//已存在，获取已存在的信号量
		if(semid < 0)
		{
			LOG_ERROR("semget failed for key 0x%x: %s", key, strerror(errno));
			return -1;
		}
	}
	//新创建的信号量，设置初始值
	for(i = 0; i < nsems; i++)
	{
		//semctl()设置信号量值，SETVAL命令设置单个信号量的值 
		if(semctl(semid, i, SETVAL, init_val[i]) < 0)
		{
			LOG_ERROR("semctl SETVAL failed for sem %d: %s", i, strerror(errno));
			semctl(semid, 0, IPC_RMID);//删除刚创建的信号量
			return -2;
		}
	}
	return semid;//返回信号量ID
}
//P操作
int sem_wait(int semid, int sem_num)
{
	struct sembuf sb;//信号量操作结构体
	
	sb.sem_num = sem_num;//要操作的信号量索引
	sb.sem_op = -1;//P操作: 信号量值减1
	sb.sem_flg = SEM_UNDO;//SEM_UNDO: 进程退出时自动恢复信号量
	
	//semop()执行信号量操作
	if(semop(semid, &sb, 1) < 0)
	{
		LOG_ERROR("semop wait failed for semid=%d, sem_num=%d: %s",semid, sem_num, strerror(errno));
		return -1;
	}
	return 0;
}
//V操作(解锁/释放资源)
int sem_signal(int semid, int sem_num)
{
	struct sembuf sb;//信号量操作结构体

	sb.sem_num = sem_num;//要操作的信号量索引
	sb.sem_op = 1;//V操作: 信号量值加1
	sb.sem_flg = SEM_UNDO;//SEM_UNDO: 进程退出时自动恢复信号量

	//semop()执行信号量操作
	if(semop(semid, &sb, 1) < 0)
	{
		LOG_ERROR("semop signal failed for semid=%d, sem_num=%d: %s",semid, sem_num, strerror(errno));
		return -1;
	}
	return 0;
}
//销毁信号量
int sem_destroy(int semid)
{
	//semctl()控制信号量，IPC_RMID命令删除信号量集合
	if(semctl(semid, 0, IPC_RMID) < 0)
	{
		LOG_ERROR("semctl IPC_RMID failed for semid=%d: %s", semid, strerror(errno));
		return -1;
	}
	return 0;
}
















