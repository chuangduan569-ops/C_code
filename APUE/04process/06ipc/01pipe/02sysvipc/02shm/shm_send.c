#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"


int main(int argc, char *argv[])
{
	key_t key;//存储生成的key值
	int shm_id = 0;//存储创建出来的虚拟内存的标识符
	struct shm_st *send_buf;//存储发送的数据
	
	//校验参数 ./send + id + msg
	if(argc < 3)
	{
		fprintf(stderr, "Usage : %s + id + msg\n", argv[0]);
		return -1;
	}

	//生成key值
	key = ftok(PATH, PROJ_ID);
	if(key == -1)
	{
		perror("ftok()");
		return -2;
	}
	//创建或获取共享内存
	shm_id = shmget(key, sizeof(struct shm_st), IPC_CREAT | 0600);
	if(shm_id == -1)
	{
		perror("shmget()");
		return -3;
	}

	//映射共享内存
	send_buf = shmat(shm_id, NULL, 0);
	if(send_buf == (void *)-1)
	{
		perror("shmat()");
		return -4;
	}
	//发送数据
	send_buf->mtype = atol(argv[1]);	//ID
	memset(send_buf->str, 0, STRSIZE);	//清空
	strncpy(send_buf->str, argv[2], STRSIZE - 1);////把命令行参数的数据转存到send_buf

	//解除映射
	shmdt(send_buf);
	return 0;
}




