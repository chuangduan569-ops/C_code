#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "my_fifo"

int main(int argc, char *argv[])
{
	int fd = 0;
	char message[1024] = {0};//储存写入的数据
	int i = 0;

	if(argc < 2)
	{
		fprintf(stderr, "Usage : %s + writer message\n", argv[0]);
		return -2;
	}
	for(i = 1; i < argc; i++)
	{
		if(i > 1)
		strcat(message, " ");
	strcat(message, argv[i]);
	}

//	message = argv[1];// 使用命令行参数作为消息
	//创建命名管道
	mkfifo(FIFO_PATH, 0666);

	//以只写的方式打开管道(此处会阻塞,直到读端打开)
	fd = open(FIFO_PATH, O_WRONLY);
	if(fd == -1)
	{
		perror("open()");
		return -1;
	}
	
	//写入数据
	write(fd, message, strlen(message) + 1);
	
	//关闭
	close(fd);

	return 0;
}






