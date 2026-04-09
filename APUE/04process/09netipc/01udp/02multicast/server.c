#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include "protocol.h"
#include <net/if.h>

int main(int argc, char *argv[])
{
	int sd = 0;//存储创建成功的套接字描述符
	struct data_st buf;//存储接收的数据
	
	struct sockaddr_in remote_addr;//存储对端的地址	
	
	struct ip_mreqn imr;//存储组播选项的结构

	//[1] 创建套接字
	sd = socket(AF_INET, SOCK_DGRAM, 0);//创建报式套接字
	if(sd == -1)//判断创建报式套接字是否失败
	{
		perror("socket()");//打印错误信息
		return -1;
	}
	//bind忽略  由内核绑定

	//[2] 使能组播选项
	inet_aton(MULTICAST_IP, &imr.imr_multiaddr);//转换组播地址
	inet_aton(LOCAL_IP, &imr.imr_address);//转换本地地址
	imr.imr_ifindex = if_nametoindex(NETCARD_NAME);//转换网卡号
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, \
			&imr, sizeof(imr)) == -1)//判断使能多播组是否失败
	{
		perror("setsockopt()");
		close(sd);//关闭套接字文件
		return -2;
	}
	
	//[3] 往多播组中发消息
	remote_addr.sin_family = AF_INET;//指定IPv4协议
	inet_aton(MULTICAST_IP, &remote_addr.sin_addr);//填入组播地址
	remote_addr.sin_port = htons(RECV_PORT);//填入端口号
	
	while(1)
	{
		//往多播组中发送数据
		sendto(sd, "hello", 5, 0, (struct sockaddr *) &remote_addr, \
			  sizeof(remote_addr));
		sleep(1);//睡1s
	}

	//[4] 关闭套接字文件
	close(sd);//关闭套接字文件
	return 0;
}



