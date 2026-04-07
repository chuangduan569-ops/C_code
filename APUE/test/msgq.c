#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include "msgq.h"
#include <stdio.h>
#include <errno.h>

int msgq_create(int key)
{
	int msqid;//消息队列ID

	msqid = msgget(key, IPC_CREAT | 0666);
	if(msqid < 0)
	{
		perror("msgget()");
		return -1;
	}
	return msqid;
}

void msgq_send(int msqid, const char *text, long type)
{
	struct msgbuf msg;//消息结构体
	int len;//消息长度
	if(msqid < 0 || !text)
		return ;
	len = strlen(text);
	if(len >= MSG_MAX)
		len = MSG_MAX - 1;//保留最后一个字节结束符

	msg.mtype = type;//设置消息类型
	memcpy(msg.mtext, text, len);//复制消息内容
	msg.mtext[len] = '\0';//添加字符串结束符

	if(msgsnd(msqid, &msg, len + 1, 0) < 0)//发送消息失败
		perror("msgsnd()");//打印错误信息

}
//接收消息(阻塞)
int msgq_recv(int msqid, char *buf, long type)
{
	struct msgbuf msg;//接收消息的缓冲区
	int ret;//接收到的消息长度
	if(msqid < 0 || !buf)
		return -1;

	ret = msgrcv(msqid, &msg, MSG_MAX, type, 0);//接收消息
	if(ret < 0)
	{
		if(errno != EINTR)//如果不是被信号中断
			perror("msgrcv()");
	return -1;
	}
	//复制消息内容到输出缓冲区
	int copy_len = (ret < MSG_MAX) ? ret : MSG_MAX -1;
	memcpy(buf, msg.mtext, copy_len);
	buf[copy_len] = '\0';

	return copy_len;//返回消息长度
}

//接收消息(非阻塞)
int msgq_recv_nowait(int msqid, char *buf, long type)
{
    struct msgbuf msg;//接收消息的缓冲区
    int ret;//接收到的消息长度
    if(msqid < 0 || !buf)
        return -1;

    ret = msgrcv(msqid, &msg, MSG_MAX, type, IPC_NOWAIT);//接收消息
    if(ret < 0)
    {
    	if(errno == ENOMSG)
    	    return 0;           // 无消息，返回0
        if(errno != EINTR)//如果不是被信号中断
            perror("msgrcv()");
    	return -1;
    }
    //复制消息内容到输出缓冲区
    int copy_len = (ret < MSG_MAX) ? ret : MSG_MAX -1;
    memcpy(buf, msg.mtext, copy_len);
    buf[copy_len] = '\0';

    return copy_len;//返回消息长度

}

void msgq_destroy(int msqid)
{
	if(msqid >= 0)
		msgctl(msqid, IPC_RMID, NULL);//删除消息队列
}




