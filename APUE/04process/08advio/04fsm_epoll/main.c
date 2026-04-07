#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/epoll.h>
#include "fsm.h"

#define EPOLL

#define MAX_EVENTS 10 //定义epoll最多监听的事件数
#define TTY1 "/dev/tty9" 
#define TTY2 "/dev/tty10"

int main(void)
{
	fsm_t *fsm12 = NULL;//指向r9w10的有限状态机
	fsm_t *fsm21 = NULL;//指向r10w9的有限状态机
	int fd1, fd2;//存储打开文件的文件描述符
#if defined(EPOLL)
	int epfd; //epoll实例的文件描述符
	int i; //循环遍历就绪事件的变量
	struct epoll_event ev;//用于添加事件的epoll结构体
	struct epoll_event events[MAX_EVENTS];//用于接收就绪事件的数组
#else
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
#if defined(EPOLL)
	//创建epoll实例（替代原来的pfd[0].fd和pfd[1].fd赋值）
	epfd = epoll_create(1);//创建epoll实例，参数>0即可(内核已忽略该参数)
	if(epfd == -1)//判断创建epoll实例是否失败
	{
		perror("epoll_create()");
		goto ERR_1;
	}
#else
	//[2] 把打开的文件描述符填入到结构体成员fd中
	pfd[0].fd = fd1;//把/dev/tty9文件的文件描述符填入到poll0中
	pfd[1].fd = fd2;//把/dev/tty10文件的文件描述符填入到poll1中

#endif
	//推动有限状态机的运行
	while(fsm12->state != STATE_T && fsm21->state != STATE_T)
	{
#if defined(EPOLL)
	//处理错误状态（与poll版本逻辑完全相同）
	if(fsm12->state == STATE_E)//判断r9w10是否到达E态
	{
		fsm_drive(fsm12);//推动r9w10到T态
		continue;//跳过本次循环 继续下一次循环
	}
	if(fsm21->state == STATE_E)//判断r10w9是否到达E态
	{
		fsm_drive(fsm21);//推动r10w9到T态
		continue;//跳过本次循环 继续下一次循环
	}
	//删除旧事件(替代poll中的 pfd[0].events = 0 和 pfd[1].events = 0)
	//注意 epoll不能直接清空 需要逐个删除之前添加的文件描述符
	epoll_ctl(epfd, EPOLL_CTL_DEL, fsm12->rfd, NULL);//删除fsm12的读文件描述符
	epoll_ctl(epfd, EPOLL_CTL_DEL, fsm12->wfd, NULL);//删除fsm12的写文件描述符
	epoll_ctl(epfd, EPOLL_CTL_DEL, fsm21->rfd, NULL);//删除fsm21的读文件描述符
	epoll_ctl(epfd, EPOLL_CTL_DEL, fsm21->wfd, NULL);//删除fsm21的写文件描述符
	//添加新事件(替代poll中的 pfd[0].events )
	//判断r9w10要监听的状态
	if(fsm12->state == STATE_R)//判断r9w10是否到达了R
	{
		ev.events = EPOLLIN; //设置监听读事件(对应poll的POLLIN)
		ev.data.fd = fsm12->rfd;//记录要监听的读文件描述符
		epoll_ctl(epfd, EPOLL_CTL_ADD, fsm12->rfd, &ev);//添加读事件到epoll
	}
	if(fsm12->state == STATE_W)//判断r9w10是否到达了W
	{
		ev.events = EPOLLOUT; //设置监听写事件(对应poll的POLLOUT)
		ev.data.fd = fsm12->wfd;//记录要监听的写文件描述符
		epoll_ctl(epfd, EPOLL_CTL_ADD, fsm12->wfd, &ev);//添加写事件到epoll
	}
	//判断r10w9要监听的状态
	if(fsm21->state == STATE_R)//判断r10w9是否到达了R
	{
		ev.events = EPOLLIN;//设置监听读事件(对应poll的POLLIN)
		ev.data.fd = fsm21->rfd;//记录要监听的读文件描述符
		epoll_ctl(epfd, EPOLL_CTL_ADD, fsm21->rfd, &ev);//添加读事件到epoll
	}
	if(fsm21->state == STATE_W)//判断r10w9是否到达了W
	{
		ev.events = EPOLLOUT;//设置监听写事件(对应poll的POLLOUT)
		ev.data.fd = fsm21->wfd;//记录要监听的写文件描述符
		epoll_ctl(epfd, EPOLL_CTL_ADD, fsm21->wfd, &ev);//添加写事件到epoll
	}
	//开始监听(//添加写事件到epoll)
	int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
	if(nfds  == -1)//请求内核监听(阻塞)
	{//注意 : 阻塞的系统调用是有可能被信号打断的, 所以加个判断
		if(errno == EINTR)//判断是否被信号打断
			continue;//被信号打断，跳过本次循环重新等待
		perror("epoll_wait()");//打印错误信息
		goto ERR_2;//监听失败跳转到错误处理
	}
	//处理就绪的事件（替代poll中的revents检查）
	for(i = 0; i < nfds; i++)// 循环遍历events数组
	{
		if(events[i].events == 0)// 如果事件为0，表示没有更多就绪事件
			break;// 退出循环
		int ready_fd = events[i].data.fd;//// 获取就绪的文件描述符
	//判断是否是fsm12相关的读和写文件描述符
	if(ready_fd == fsm12->rfd || ready_fd == fsm12->wfd)
		fsm_drive(fsm12);//推动fsm12(r9w10)有限状态机
	//判断是否是fsm21相关的读和写文件描述符
	if(ready_fd == fsm21->rfd || ready_fd == fsm21->wfd)	
		fsm_drive(fsm21);//推动fsm21(r10w9)有限状态机
	}
#else
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

#endif
	}
ERR_2 :
#if defined(EPOLL)
	//关闭epoll实例，释放资源
	close(epfd);//关闭epoll实例的文件描述符
#endif
ERR_1 :
	fsm_destroy(fsm12);//释放r9w10的有限状态机
	fsm_destroy(fsm21);//释放r10w9的有限状态机

	close(fd2);//关闭/dev/tty9的设备文件
	close(fd1);//关闭/dev/tty10的设备文件

	return 0;
}

