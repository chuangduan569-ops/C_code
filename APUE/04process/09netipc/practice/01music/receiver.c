#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <signal.h>     
#include <sys/wait.h>   

#define MULTICAST_IP "224.0.0.1" //组播IP地址(与对端一致)
#define	PORT	5202	//组播通信端口(与对端一直)
#define	BUFFER_SIZE	1024	//接收缓冲区大小(1KB)

volatile int running = 1;//全局运行标志

void signal_handler(int sig)
{
	if(sig == SIGINT || sig == SIGTERM)
		running = 0;//设备运行标志为0 主循环退出
}
/*
函数实现的功能 子进程播放音频函数  参数 读端的文件描述符
 1 将标准输入重定向到管道读端
 2 启动音频播放器(mpg123或aplay)
 3 从标准输入读取数据并传给播放器
 */
 /*
void play_audio(int pipe_fd)
{
	FILE *player;//播放器进程的FILE指针
	char buffer[BUFFER_SIZE];//数据缓冲区
	ssize_t bytes_read;//实际读取的字节数
	//dup2：复制文件描述符
	//将标准输入（文件描述符0）重定向到管道读端（pipe_fd）
	//这样后续从stdin读取数据实际上就是从管道读取
	if(dup2(pipe_fd, STDIN_FILENO) == -1)//判断重定向是否失败
	{
		perror("dup2()");
		exit(1);
	}
	//关闭管道读端（已经通过dup2复制了，原来的可以关闭）
	close(pipe_fd);
	// 尝试使用mpg123播放MP3（这是最常用的MP3命令行播放器）
	// popen：创建一个管道连接到子进程的标准输入/输出
	// "mpg123 -" 表示从标准输入读取MP3数据并播放
	// "w" 表示以写入方式打开管道（父进程写入，子进程读取）
	player = popen("mpg123 - 2>/dev/null", "w");
	if(player == NULL)
	{
		// 如果mpg123不可用，尝试使用aplay（仅支持WAV格式）
		printf("[警告] mpg123不可用，尝试使用aplay...\n");
		printf("[提示] 可以安装mpg123: sudo apt-get install mpg123\n");
		player = popen("aplay -f cd -t wav 2>/dev/null", "w");
	}
	if(player == NULL)
	{
		// 两个播放器都无法启动，打印错误并退出
		fprintf(stderr, "[错误] 无法启动音频播放器\n");
		fprintf(stderr, "请安装: sudo apt-get install mpg123\n");
		exit(1);
	}
	printf("[子进程] 音频播放器已启动\n");
	//从标准输入（此时已经是管道读端）读取数据
	//读取到的数据直接写入播放器进程的标准输入
	while((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) >0)
	{
		// fwrite：将数据写入播放器进程
		fwrite(buffer,1, bytes_read, player);
		fflush(player);//刷新缓冲区，确保数据及时发送给播放器
    }
	//关闭播放器进程
	pclose(player);
	printf("播放结束\n");
	exit(0);
}
*/

int main(int argc, char *argv[])
{
	int sockfd;//socket文件描述符
	int pipe_fd[2];//匿名管道的两个文件描述符 0读段 1写端
	struct sockaddr_in local_addr;//本地地址结构体
	struct ip_mreq mreq;//组播请求结构体 用于加入/离开组播
	pid_t pid;//子进程ID
	char buffer[BUFFER_SIZE];//接收数据缓冲区
	ssize_t bytes_received;//实际接收的字节数
	int total_bytes = 0;//累积接收的总字节数
	int packet_count = 0;//接收的数据包数量
	
	//注册信号处理函数
	signal(SIGINT, signal_handler); //ctrl+c信号
	signal(SIGTERM, signal_handler); //终止信号
	signal(SIGPIPE, SIG_IGN);//忽略管道破裂信号（防止写入已关闭的管道时程序崩溃）

	printf("========================================\n");
	printf("    组播音频接收程序 :  \n");
	printf("========================================\n");
	//[1] 创建匿名管道 pipe_fd[0]用于读数据，pipe_fd[1]用于写数据
	if(pipe(pipe_fd) < 0)//判断匿名管道创建是否失败
	{
		perror("pipe()");
		return -1;
	}
	//[2] 创建子进程 
	pid = fork();
	if(pid == -1)//判断子进程创建是否失败
	{
		perror("fork()");
		return -2;
	}
	if(pid == 0)//子进程的工作(播放音乐)
	{
		close(pipe_fd[1]);//关闭写端 只用读段
		// 将标准输入重定向到管道读端
        	if(dup2(pipe_fd[0], STDIN_FILENO) == -1)
        	{
            	perror("dup2()");
            	exit(1);
        	}
        	close(pipe_fd[0]);  // 关闭原管道读端（已复制）
		//play_audio(pipe_fd[0]);//调用播放函数 传入管道的读端
		// 直接替换进程！子进程变成mpg123
		// 尝试启动mpg123播放器
        	// execlp会替换当前进程，如果成功就不会返回
    		//execlp("mpg123", "mpg123", "-", NULL);
    		execlp("mpg123", "mpg123", "--buffer", "2048", "-", NULL);
			execlp("aplay", "aplay", "-f", "cd", "-t", "wav", NULL);
    		exit(1);
		//return 0;
	}
	//父进程 负责接收组播数据
	//父进程不需要管道的读端，关闭读端
	close(pipe_fd[0]);//关闭读端，只用写端
	printf("[父进程] PID : %d\n", getpid());
	printf("[子进程] PID : %d\n", pid);
	//[3] 创建UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)//判断创建UDP是否失败
	{
		perror("socket()");
		kill(pid, SIGTERM);//给子进程发送终止信号
		return -3;
	}
	//[4] 设置端口重用
	//SO_REUSEADDR：允许多个程序同时绑定同一个端口 这样多个组员可以同时加入同一个组播组
	int reuse = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt()");
    }
	//[5] 绑定本地地址 
	//绑定到INADDR_ANY表示接收所有网络接口的数据
	memset(&local_addr, 0, sizeof(local_addr));//清零地址结构体
	local_addr.sin_family = AF_INET;              // IPv4地址族
	local_addr.sin_addr.s_addr = INADDR_ANY;      // 监听所有网络接口
	local_addr.sin_port = htons(PORT);            // 绑定的端口号
	if(bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind()");
        close(sockfd);
        kill(pid, SIGTERM);
        return -4;
    }
	//[6]加入组播组  配置组播组信息
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);  // 要加入的组播IP
	mreq.imr_interface.s_addr = INADDR_ANY;  // 从哪个网络接口加入
	// IP_ADD_MEMBERSHIP：加入组播组
	if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) 
	{
        perror("setsockopt()");
        close(sockfd);
        kill(pid, SIGTERM);
        return -1;
    }
	printf("已加入组播组: %s:%d\n", MULTICAST_IP, PORT);
	printf("开始接收音频数据...\n");
	printf("按 Ctrl+C 停止接收\n\n");
	//[7] 接收数据并写入管道 持续接收组播数据
	while(running)
	{// recvfrom：从socket接收数据
// 参数：socket, 缓冲区, 缓冲区大小, 标志位, 发送方地址(不需要传NULL), 地址长度
		bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
		if(bytes_received > 0)//表示成功接收到了数据
		{
			// 将接收到的数据写入管道，传给子进程播放
			ssize_t written = write(pipe_fd[1], buffer, bytes_received);
			if(written < 0)
			{
				if(running)
					perror("write()");
				break;
			}
			// 更新统计信息
			total_bytes += bytes_received;
			packet_count++;
			// 每100个包显示一次状态（避免刷新太快影响性能）
			if(packet_count % 100 == 0)
			{
				printf("\r已接收: %.2f KB, 包数: %d", total_bytes / 1024.0, packet_count);
				fflush(stdout);//刷新输出缓冲区
			}
		}
	}
	//[8] 清理资源
	//关闭管道写端，通知子进程没有更多数据了
	close(pipe_fd[1]);
	//等待子进程结束
	waitpid(pid, NULL, 0);
	//离开组播组  IP_DROP_MEMBERSHIP：退出组播组
	setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	//关闭socket
	close(sockfd);


	return 0;
}












