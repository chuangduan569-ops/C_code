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
#include <net/if.h>

//把argv[1] 当作 ID
//把argv[2] 当作 MSG

int main(int argc, char *argv[])
{
	int sd = 0;//存储创建成功的套接字描述符
	struct data_st buf;//存储要发送的数据
	struct sockaddr_in my_addr;//存储本地的地址
	struct ip_mreqn imr;//存储组播选项的结构

	//[1] 创建套接字
	sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字	
	
	if(sd == -1)//判断创建报式是否失败
	{
		perror("socket()");
		return -2;
	}
	//[2] 将本地地址与套接字绑定
	my_addr.sin_family = AF_INET;//指定IPv4协议
	inet_aton(LOCAL_IP, &my_addr.sin_addr);//转换本地IP
	my_addr.sin_port = htons(RECV_PORT);
	if(bind(sd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind()");
		close(sd);
		return -2;
	}
	//[3] 加入多播组
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);//转换组播地址
	inet_aton(LOCAL_IP, &imr.imr_address);//转换本地地址
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);//转换网卡号
	if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, \
        &imr, sizeof(imr)) == -1)//判断加入多播组是否失败
	{
    perror("setsockopt()");
    close(sd);//关闭套接字文件
    return -3;
	}
	//[4] 接收多播组的消息
	recvfrom(sd, buf.msg, MSGSIZE, 0, NULL, NULL);
	puts(buf.msg);
	
	//[5]关闭网络套接字文件
	close(sd);

	return 0;
}



