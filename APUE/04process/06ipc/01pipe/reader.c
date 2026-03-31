#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "my_fifo"

int main(void)
{
	int fd = 0;
	char buffer[1024];//接收管道的数据
	ssize_t count = 0;//接收数据

	//以只读的方式打开管道(此处会阻塞,直到写端打开)
	fd = open(FIFO_PATH, O_RDONLY);
	if(fd == -1)
	{
		perror("open()");
		return -1;
	}
	//读取数据
	count = read(fd, buffer, sizeof(buffer));
	if(count < 0)
	{
		perror("read()");
		return -2;
	}
	puts(buffer);
	
	//关闭并删除管道文件
	close(fd);
	unlink(FIFO_PATH);
	return 0;
}



