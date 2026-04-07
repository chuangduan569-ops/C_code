#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include "fsm.h"

#define SELECT

#define TTY1 "/dev/tty9" 
#define TTY2 "/dev/tty10"

int main(void)
{
	fsm_t *fsm12 = NULL;//指向r9w10的有限状态机
	fsm_t *fsm21 = NULL;//指向r10w9的有限状态机
	int fd1, fd2;//存储打开文件的文件描述符

#if defined(SELECT)
	fd_set rset, wset;//定义读集和写集
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
	
	//推动有限状态机的运行
	while(fsm12->state != STATE_T && fsm21->state != STATE_T)
	{//不再是无脑的推动方式 而是满足一定条件才推动状态机
#if defined(SELECT)
	//[2] 如果状态到达E态 不需要监听
	if(fsm12->state == STATE_E)//判断r9w10是否到达E态
	{
		fsm_drive(fsm12);//推动r9w10到达T态
		continue;//结束本次循环，继续下一次循环
	}
	if(fsm21->state == STATE_E)//判断r10w9是否到达E态
	{
		fsm_drive(fsm21);//推动r10w9到达T态
		continue;//结束本次循环，继续下一次循环
	}
	//[3] 初始化集合
	FD_ZERO(&rset);//初始化读集为空
	FD_ZERO(&wset);//初始化写集为空
	//[4] 判断fsm12(r9w10)要监听的状态
	if(fsm12->state == STATE_R)//判断r9w10是否到达了R状态
		FD_SET(fsm12->rfd, &rset);//把读文件描述符添加读集中
	if(fsm12->state == STATE_W)//判断r9w10是否到达了W状态
		FD_SET(fsm12->wfd, &wset);//把写文件描述符添加写集中
	//[5] 判断fsm21(r10w9)要监听得状态
	if(fsm21->state == STATE_R)//判断r10w9是否到达了R状态
    		FD_SET(fsm21->rfd, &rset);//把读文件描述符添加读集中
	if(fsm21->state == STATE_W)//判断r10w9是否到达了W状态
	    	FD_SET(fsm21->wfd, &wset);//把写文件描述符添加写集中
	//[6] 开始监听(等待文件描述符变为"ready")
	//注意 : 阻塞的系统调用时有可能被信号打断 所以需要加个判断
	if(select((fd1>fd2?fd1:fd2)+1, &rset, &wset, NULL, NULL) == -1)//开始监听
	{
		if(errno == EINTR);//判断是否被信号打断
			continue;//结束本次循环进入下一次循环
		perror("select()");//打印错误信息
		goto ERR_1;//由于监听失败跳转到ERR_1;
	}
	//[7] 判断那个文件描述符变为"ready"
	if(FD_ISSET(fsm12->rfd, &rset) || FD_ISSET(fsm12->wfd, &wset))
	//判断fsm12的读文件描述符是否在读集中 || fsm12的写文件描述符是否在写集中
		fsm_drive(fsm12);//推动读9写10的有限状态机
	if(FD_ISSET(fsm21->rfd, &rset) || FD_ISSET(fsm21->wfd, &wset))
	//判断fsm21的读文件描述符是否在读集中 || fsm21的写文件描述符是否在写集中
		fsm_drive(fsm21);//推动读10写9的有限状态机
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

