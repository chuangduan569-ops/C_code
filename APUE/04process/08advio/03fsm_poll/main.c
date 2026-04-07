#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include "fsm.h"

#define POLL

#define TTY1 "/dev/tty9" 
#define TTY2 "/dev/tty10"

int main(void)
{
	fsm_t *fsm12 = NULL;//指向r9w10的有限状态机
	fsm_t *fsm21 = NULL;//指向r10w9的有限状态机
	int fd1, fd2;//存储打开文件的文件描述符
#if defined(POLL)
	//[1] 定义poll类型结构体的变量
	struct pollfd pfd[2];//定义poll结构体类型的数组
#endif
	fd1 = open(TTY1, O_RDWR);//打开/dev/tty9设备(打开时没有加入非阻塞选项 后期通过fcntl加入)
	if(fd1 == -1)//判断打开/dev/tty9设备是否失败
	{
		perror("open()");
		return -1;
	}
	write(fd1, "[****tty9****]", 14);//用来区分是/dev/tty9的设备
	
	fd2 = open(TTY2, O_RDWR | O_NONBLOCK);//打开/dev/tty10设备(打开时加入非阻塞选项)
	if(fd2 == -1)//判断打开/dev/tty10设备是否失败
	{
		perror("open()");
		close(fd1);
		return -2;
	}
	write(fd2, "[!!!tty10!!!]", 13);//用来区分是/dev/tty10的设备

	fsm_init(&fsm12, fd1, fd2);//初始化r9w10的有限状态机
	fsm_init(&fsm21, fd2, fd1);//初始化r10w9的有限状态机
#if defined(POLL)
	//[2] 把打开的文件描述符填入到结构体成员fd中
	pfd[0].fd = fd1;//把/dev/tty9文件的文件描述符填入到poll0中
	pfd[1].fd = fd2;//把/dev/tty10文件的文件描述符填入到poll1中

#endif
	//推动有限状态机的运行
	while(fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
#if defined(POLL)
	//[3] 清空poll(2) 中监听的事件
	pfd[0].events = 0;//清空poll0监听的事件
	pfd[1].events = 0;//清空poll1监听的事件
	//[4] 判断fsm12(r9w10) 要监听的状态
	if(fsm12->state == STATE_R)//判断r9w10是否到达了R态
		pfd[0].events |= POLLIN;	//poll0监听读事件
	if(fsm12->state == STATE_W)//判断r10w9是否到达了W态
		pfd[1].events |= POLLOUT;//poll1监听写事件
	//[5] 判断fsm21(r10w9) 要监听的状态
	if(fsm21->state == STATE_R)//判断r10w9是否到达了R态
	    pfd[1].events |= POLLIN;    //poll1监听读事件
	if(fsm21->state == STATE_W)//判断r10w9是否到达了W态
    	pfd[0].events |= POLLOUT;//poll0监听写事件
	//[6] 开始监听
	if(poll(pfd, 2, -1) == -1)//请求内核监听(阻塞)
	{//注意 : 阻塞的系统调用是有可能被信号打断的, 所以加个判断
		if(errno == EINTR)//判断是否被信号打断
			continue;//跳过本次循环 进入下一次循环
		perror("poll()");//打印错误信息
		goto ERR_1;
	}
	// [7] 判断poll0是否可读 || 判断poll1是否可写
	if(pfd[0].revents & POLLIN || pfd[1].revents & POLLOUT)
		fsm_drive(fsm12);//推动fsm12(r9w10)有限状态机
	//[8] 判断poll1是否可读 || 判断poll0是否可写
	if(pfd[1].revents & POLLIN || pfd[0].revents & POLLOUT)
		fsm_drive(fsm21);//推动fsm21(r10w9)有限状态机
#else
		fsm_drive(fsm12);//推动r9w10的有限状态机
		fsm_drive(fsm21);//推动r10w9的有限状态机
#endif
	}
ERR_1 :
	fsm_destroy(fsm12);//释放r9w10的有限状态机
	fsm_destroy(fsm21);//释放r10w9的有限状态机

	close(fd2);//关闭/dev/tty9的设备文件
	close(fd1);//关闭/dev/tty10的设备文件

	return 0;
}

