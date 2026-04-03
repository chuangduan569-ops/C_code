#ifndef __MSGQ_H
#define __MSGQ_H

//消息最大长度：1024字节
//每条JSON格式的日志消息的最大长度
#define MSG_MAX	1024
//创建消息队列的key值
#define MSG_KEY 0x9ABC

struct msgbuf//消息结构体
{
	long mtype;//官方定义消息类型
	char mtext[MSG_MAX];//消息内容：json格式的日志
};

int msgq_create(int key);//创建消息队列

//发送消息 消息队列ID 发送的内容 消息类型(>0)
void msgq_send(int msqid, const char *text, long type);

//接收消息(阻塞) 消息队列ID 接收缓冲区 消息类型(0表示接收任意类型)
int msgq_recv(int msqid, char *buf, long type);

//接收消息(非阻塞) 消息队列ID 接收缓冲区 消息类型(0表示接收任意类型)
int msgq_recv_nowait(int msqid, char *buf, long type);

//销毁消息队列
void msgq_destroy(int msqid);


#endif




