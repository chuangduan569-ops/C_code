#include <stdio.h>
#include "protocol.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	key_t key;//存储生成的key值
	int msg_id = 0;//存储创建出来的消息队列的标识符	
	struct msg_st send_buf;//存储发送数据
	ssize_t count = 0;//count存储成功接收到数据的字节数


	//我们将来执行程序时需要3个参数 ./a.out id msg
	if(argc < 3)
	{
		fprintf(stderr, "Usage : %s + id + msg\n", argv[0]);
		return -1;
	}

	//[1]通过ftok(3)生成key值
	key = ftok(PATH, PROJ_ID);//根据协议中指定的路径和ID生成key值
	if(key == -1)//判断生成key是否失败
	{
		perror("ftok()");
		return -2;
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
			return -3;
		}
	}
//以上的操作和recv.c一模一样
	//[3]发送数据
	memset(&send_buf, 0, sizeof(send_buf));
	send_buf.mtype = atoi(argv[1]);//把命令行参数id通过atoi(3)转换成整型存到send_buf
	strncpy(send_buf.str, argv[2], STRSIZE);//把命令行参数的数据转存到send_buf
	count = msgsnd(msg_id, &send_buf, strlen(send_buf.str), 0);//往消息队列发送消息
	 if(count == -1)//判断发送是否失败
 {
     perror("msgsnd()");
    // msgctl(msg_id, IPC_RMID, NULL);//销毁消息队列
     return -4;
 }
//[4] 销毁消息队列
//可以销毁也可以不销毁，是灵活的
//如果销毁了消息队列，接收端会受到影响
//msgctl(msg_id, IPC_RMID, NULL);


	return 0;
}




