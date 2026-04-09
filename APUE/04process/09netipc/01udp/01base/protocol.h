#ifndef __PROTOCOL_H
#define __PROTOCOL_H

//约定地址 IP + PORT
#define SERVER_IP "10.11.17.155"
#define SERVER_PORT	5202

//信息大小
#define MSGSIZE	128

//约定双方对话格式
struct data_st
{
	int8_t id;
	char msg[MSGSIZE];
}__attribute((packed));//单字节对齐





#endif
