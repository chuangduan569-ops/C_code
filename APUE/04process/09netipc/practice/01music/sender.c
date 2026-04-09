#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MULTICAST_IP "224.0.0.1"
#define PORT 5202
#define BUFFER_SIZE 1024
#define TTL 2  // 组播TTL值（路由器跳数）

int main(void)
{
	int sd;// Socket文件描述符
	int fd;// 音频文件描述符
	struct sockaddr_in group_addr;// 组播地址结构体（存储IP和端口）
	char buffer[BUFFER_SIZE];// 数据缓冲区，用于存储从文件读取的数据
	ssize_t len;// 实际读取的字节数（ssize_t是有符号整型）
	int total_bytes = 0; // 累计发送的总字节数（用于统计）
	int packet_count = 0; // 累计发送的数据包数量（用于统计）
	//[1] 创建UDP套接字
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd == -1)//判断创建套接字是否失败
	{
		perror("socket()");       // 打印错误信息
		return -1;
	}
	//[2] 设置组播TTL（重要！否则数据包可能出不了本地网络）
	int ttl = TTL;// 将TTL值赋给变量（默认2跳）
	// IPPROTO_IP: IP协议层
    // IP_MULTICAST_TTL: 设置组播TTL的选项名
    // &ttl: 指向TTL值的指针
    // sizeof(ttl): TTL值的大小
	if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) == -1)
	{
		perror("setsockopt()");
		close(sd);// 关闭socket
		return -2;
	}
	//[3] 配置组播地址
	memset(&group_addr, 0, sizeof(group_addr));//将地址结构体清零
	group_addr.sin_family = AF_INET;//设置地址为IPv4
	group_addr.sin_port = htons(PORT);//设置端口号（htons将主机字节序转为网络字节序）
	inet_aton(MULTICAST_IP, &group_addr.sin_addr);//将点分十进制IP字符串转换为二进制网络字节序
	//[4] 打开音频文件
	fd = open("music.mp3", O_RDONLY);//只读模式打开
	if(fd < 0)//判断是否打开失败
	{
		perror("open()");
		close(sd);
		return -3;
	}
	// 打印程序启动信息
    printf("========================================\n");
    printf("    组播音频发送程序\n");
    printf("========================================\n");
    printf("组播组: %s:%d\n", MULTICAST_IP, PORT);  // 显示组播地址和端口
	//[5] 循环发送数据
	while(1)
	{
	len = read(fd, buffer, BUFFER_SIZE);//成功读取的字节数
	if(len <= 0)//判断读取失败或到达文件末尾
	{
		if(len == 0)// len==0 表示正常读取到文件末尾
		{
			lseek(fd, 0, SEEK_SET);  // 将文件指针移动到文件开头（SEEK_SET表示从文件头开始）
			total_bytes = 0;         // 重置累计字节数统计
			packet_count = 0;        // 重置包数量统计
			continue;                // 跳过本次循环的剩余部分，重新开始读取
		}
		else//len < 0 表示读取错误
		{
			perror("read()");
			break;// 跳出循环，结束程序
		}
	}
	//发送组播数据
	ssize_t sent = sendto(sd, buffer, len, 0, (struct sockaddr *)&group_addr, sizeof(group_addr));
	if(sent != len)// 如果发送的字节数与预期不符
	{
		perror("sendto()");
		break;
	}
	//统计信息
	total_bytes += len;       // 累加发送的总字节数
    packet_count++;           // 累加发送的包数量
	//每发送100个数据包，打印一次状态
	if(packet_count % 100 == 0)
	{
		// \r: 回车符，将光标移到行首（覆盖之前的内容，实现动态刷新效果）
        // %.2f: 保留两位小数的浮点数
        // total_bytes / 1024.0: 将字节转换为KB
		printf("\r已发送: %.2f KB, 包数: %d", total_bytes / 1024.0, packet_count);
		fflush(stdout);
	}
	// 流量控制：模拟实时音频流
        // 对于MP3，典型码率128kbps = 16KB/s
        // 每包1KB，所以每秒约16包，每包间隔约62ms
        // usleep(62000): 暂停62000微秒 = 62毫秒
        // 这样可以模拟实时音频流的速度，避免发送过快导致网络拥塞或接收端缓冲区溢出
	usleep(65000);//根据实际码率调整这个值
	}

	close(sd);
	close(fd);
	return 0;
}







