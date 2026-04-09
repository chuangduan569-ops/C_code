#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "protocol.h"


int main(void)
{
	int tcp_socket;//存储创建成功的流式套接字
	int new_socket;//存储accept(2)返回的描述符
	struct sockaddr_in laddr;//存储本地的地址	
	pid_t pid;//存储子进程的标识
	struct sigaction act;//存储信号的行为

	act.sa_handler = SIG_DFL;//默认行为
	act.sa_flags = SA_NOCLDWAIT;//加入要求(子进程不会变为僵尸进程)
	sigaction(SIGCHLD, &act, NULL);////注册信号行为


	//[1] 创建流式套接字
	tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_socket == -1)//判断创建流式套接字是否失败
	{
		perror("socket()");
		return -1;
	}

	// 设置 SO_REUSEADDR 选项  即使服务器进程退出后立即重启，也能成功绑定端口，不会出现"已绑定"的错误。
	int opt = 1;
	if (setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
	}

	//[2] 绑定地址 bind(2)
	laddr.sin_family = AF_INET;//指定IPv4的协议
							   //inet_aton("0.0.0.0", &laddr.sin_addr);//转换本地地址(也可以使用宏)
	laddr.sin_addr.s_addr = INADDR_ANY;//转换本地地址(也可以使用宏)
	laddr.sin_port = htons(SERVER_PORT);//转换本地端口
	if(bind(tcp_socket, (struct sockaddr *)&laddr, sizeof(laddr)) == -1)
	{
		perror("bind()");
		close(tcp_socket);
		return -2;
	}
	//[3] 套接字处于监听状态 listen(2) 最多支持连接请求20个
	if(listen(tcp_socket, 20) == -1)
	{
		perror("listen()");
		close(tcp_socket);
		return -3;
	}
	//[4] 接收连接请求 accept(2)
	while(1)
	{
		new_socket = accept(tcp_socket, NULL, NULL);//接收连接请求
		if(new_socket == -1)//判断连接请求是否失败
		{
			perror("accept");
			close(tcp_socket);
			return -4;
		}
		//new_socket用于数据的交互 tcp_socket用于接收连接请求的
		pid = fork();//创建子进程
		if(pid == -1)//判断子进程是否创建失败
		{
			perror("fork()");
			close(tcp_socket);
			return -5;
		}
		//[5]连接成功后进行IO
		if(pid == 0)//子进程的操作
		{
			write(new_socket, "hello dc", 8);//写数据
			close(new_socket);//关闭用于交互数据的套接字
			close(tcp_socket);//关闭流式套接字
			exit(0);//终止子进程
		}
		//父进程的操作
		close(new_socket);//关闭用于交互数据的套接字
	}
	
	//[6] 关闭流式套接字 close(2)
	close(tcp_socket);//关闭流式套接字

	return 0;
}



