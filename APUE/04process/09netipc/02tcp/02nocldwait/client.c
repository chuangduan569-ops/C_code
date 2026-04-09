#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "protocol.h"

int main(void)
{
	int tcp_socket;//存储创建成功的流式套接字
	struct sockaddr_in raddr;//存储对端地址
	char msg[MSG_SIZE] = {0};//存储对端传输的数据

	//[1] 创建流式套接字 socket(2)
	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_socket == -1)//判断创建流式套接字是否失败
	{
		perror("socket()");
		return -1;
	}
	//[2] 和对端请求连接 connect(2)
	raddr.sin_family = AF_INET;//指定IPv4协议
	inet_aton(SERVER_IP, &raddr.sin_addr);//转换对端地址
	raddr.sin_port = htons(SERVER_PORT);//转换对端端口号
	if(connect(tcp_socket, (struct sockaddr *)&raddr, sizeof(raddr)) == -1)
	{
		perror("connect()");
		close(tcp_socket);//关闭流式套接字
		return -2;
	}
	//[3] 一旦请求成功,进行I/O操作   read(2)/writre(2)
	read(tcp_socket, msg, MSG_SIZE);//读取对端发送过来的数据
	puts(msg);//把读取到的数据打印到标准输出中

	//[4] 关闭流式套接字 close(2)
	close(tcp_socket);//关闭流式套接字
	return 0;
}





