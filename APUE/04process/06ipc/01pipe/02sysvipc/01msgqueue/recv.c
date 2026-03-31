#include <stdio.h>
#include "protocol.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	key_t key;//存储生成的key值
	int msg_id = 0;//存储创建出来的消息队列的标识符	
	struct msg_st recv_buf;//存储接收到的数据
	ssize_t count = 0;//count存储成功接收到数据的字节数

	//[1]通过ftok(3)生成key值
	key = ftok(PATH, PROJ_ID);//根据协议中指定的路径和ID生成key值
	if(key == -1)//判断生成key是否失败
	{
		perror("ftok()");
		return -1;
	}

	//[2]创建消息队列  msgget(2)
	//IPC_EXCL|IPC_CREAT一起使用,表示如果要创建的消息队列已存在则返回错误
	//并且设置读写的权限
	msg_id = msgget(key, IPC_CREAT | IPC_EXCL | 0600);//创建消息队列
	if(msg_id == -1)//判断创建消息队列是否失败
	{
		if(errno == EEXIST)//判断消息队列是否已存在
		{
			msg_id = msgget(key, 0);//由于消息队列存在,直接获取消息队列的标识
		}
		else
		{
			perror("msgget()");
			return -2;
		}
	}
//以上的操作和send.c一模一样
	//[3]等待接收信息
	while(1)
	{
	memset(&recv_buf, 0, sizeof(recv_buf));//清空脏数据
	count = msgrcv(msg_id, &recv_buf, STRSIZE, 0, 0);//接收消息队列的数据
	if(count == -1)//判断接收是否失败
	{
		perror("msgrcv()");
		msgctl(msg_id, IPC_RMID, NULL);//销毁消息队列
		return -3;
	}
	puts(recv_buf.str);//把消息队列中接收到的消息打印出来
	}



	return 0;
}




