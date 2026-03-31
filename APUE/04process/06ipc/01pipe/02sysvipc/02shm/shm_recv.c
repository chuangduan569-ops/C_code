#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"


int main(void)
{
	key_t key;
	int shm_id = 0;
	struct shm_st *recv_buf;
	char oldmsg[STRSIZE] = {0};//用于和下一次数据做对比

	//生成key值
	key = ftok(PATH, PROJ_ID);
	if(key == -1)
	{
		perror("ftok()");
		return -1;
	}
	//获取共享内存标识符
	shm_id = shmget(key, sizeof(struct shm_st), IPC_CREAT | 0600);
	if(shm_id == -1)
	{
		perror("shmget()");
		return -2;
	}
	//映射内存共享
	recv_buf = shmat(shm_id, NULL, 0);
	if(recv_buf == (void *)-1)
	{
		perror("shmat()");
		return -3;
	}
	//接收数据
	while(1)
	{
		if(recv_buf->str[0] != '\0' && strcmp(oldmsg, recv_buf->str) != 0)
		{
			// 直接打印结构体中的字符串成员
			puts(recv_buf->str);
		// 备份当前数据，防止重复触发打印
		strcpy(oldmsg, recv_buf->str);
		}
//	memset(recv_buf->str, 0, STRSIZE);
//		puts(recv_buf->str);

	usleep(100000);  // 添加这一行，休眠0.1秒   CPU 占用率可能过高
	
	}
	
	//解除映射(实际上由于while(1)通常由Ctrl+C终止，此行在手动结束时运行)
	shmdt(recv_buf);

	return 0;
}


