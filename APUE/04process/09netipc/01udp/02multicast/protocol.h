#ifndef __PROTOCOL_H
#define __PROTOCOL_H

//不需要服务端的地址 需要多播组的地址
#define MULTICAST_IP	"234.2.3.4"
//在创建多播组的时候需要本地的ID
#define LOCAL_IP	"0.0.0.0"
//通过ifconfig查看当前系统的网卡名字
#define NETCARD_NAME	"ens33"
//需要接收端的端口
#define RECV_PORT	5202

//信息大小
#define MSGSIZE	128

//约定双方对话格式
struct data_st
{
	int8_t id;
	char msg[MSGSIZE];
}__attribute((packed));//单字节对齐





#endif
