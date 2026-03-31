#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>    // 进程间通信(IPC)基本定义
#include <sys/msg.h>    // 消息队列相关函数及结构
#include <sys/types.h>  // 基本系统数据类型
#include <unistd.h>     // 标准符号常量和类型 (fork)
#include <sys/wait.h>   // 进程控制 (wait)
#include <errno.h>      // 错误号定义

#define BUF_SIZE	128
struct msgbuf
{
	long mtype;//消息类型 必须大于0
	char mtext[BUF_SIZE];//消息正文
};
//必须遵守内核规范：第一个成员必须是 long 类型的消息类型(mtype)
int main(int argc, char *argv[])
{
	int msgid;	//消息队列标识符
	pid_t pid;	//进程ID标识符
	struct msgbuf data;	//消息缓冲区实例

	//我们将来执行程序时需要3个参数 ./a.out id msg
    if(argc < 3)
    {
        fprintf(stderr, "Usage : %s + id + msg\n", argv[0]);
        return -1;
    }

	//创建私有的消息队列
	msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);//创建消息队列
	if(msgid == -1)
	{
		perror("msgget()");
		exit(1);
	}
	//创建子进程
	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		msgctl(msgid, IPC_RMID, NULL);//销毁消息队列
		exit(2);
	}

	if(pid == 0)//子进程的操作
	{
		//发送数据
		memset(&data, 0, BUF_SIZE);//清空脏数据
		data.mtype = atol(argv[1]);//把命令行参数的id通过atoi(3)转换成整型
		strncpy(data.mtext, argv[2], BUF_SIZE - 1);//把命令行参数的数据转存到data
		data.mtext[BUF_SIZE - 1] = '\0';

		if(msgsnd(msgid, &data, strlen(data.mtext) + 1, 0) == -1)//判断发送数据是否失败
		{
			perror("msgsnd()");
			exit(3);
		}
		exit(0);//终止子进程
	}

	//父进程操作
	if(msgrcv(msgid, &data, BUF_SIZE, atol(argv[1]), 0) == -1)//判断接收消息队列是否失败
	{
		perror("msgrcv()");
		msgctl(msgid, IPC_RMID, NULL);
		exit(4);
	}
	puts(data.mtext);
	
	wait(NULL);

	//销毁队列
	msgctl(msgid, IPC_RMID, NULL);


	return 0;
}

