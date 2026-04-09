#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <string.h>
#include "protocol.h"
#include <unistd.h>

//把argv[1] 当作 ID
//把argv[2] 当作 MSG

int main(int argc, char *argv[])
{
	int sd = 0;//存储创建成功的套接字描述符
	struct data_st buf;//存储要发送的数据
	struct sockaddr_in sever_addr;//存储对端的地址

	if(argc < 3)//判断命令行参数的个数是否少于3个
	{
		fprintf(stderr, "Usage : %s + ID + MSG\n", argv[0]);//打印使用说明
		return -1;
	}
	//[1] 创建套接字
	sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字	
	if(sd == -1)//判断创建报式是否失败
	{
		perror("socket()");
		return -2;
	}
	//[2] 发送数据报
	buf.id = atoi(argv[1]);//把argv[1]转换为整形的ID
	strncpy(buf.msg, argv[2], MSGSIZE);//把argv[2]拷贝到msg
	sever_addr.sin_family = AF_INET;//指定IPv4协议

	inet_aton(SERVER_IP, &sever_addr.sin_addr);//转换成对端IP地址
	//把点分十进制地址字符串转换成地址结构体
	sever_addr.sin_port = htons(SERVER_PORT);//转换对端端口号
	//将本地字节序的端口号转换为网络字节序
 
	if(sendto(sd, &buf, sizeof(buf.id)+strlen(buf.msg)+1, 0, \
		  (struct sockaddr *)&sever_addr, sizeof(sever_addr)) == -1)
	{
		perror("sendto()");
		close(sd);//关闭网络套接字文件
		return -3;
	}
	////关闭网络套接字文件
	
	close(sd);

	return 0;
}



