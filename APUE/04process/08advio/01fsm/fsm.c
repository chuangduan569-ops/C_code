#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "fsm.h"

int fsm_init(fsm_t **f, int rfd, int wfd)
{
	fsm_t *p = NULL;//指向开辟的空间
	int saver = 0, savew = 0;//存储客户传递的文件描述符状态
	
	p = malloc(sizeof(fsm_t));//开辟有限状态机的空间
	if(p == NULL)//判断开辟空间是否失败
		return -1;

	p->rfd = rfd;//存储客户指定的读文件的文件描述符
	p->wfd = wfd;//存储客户指定的写文件的文件描述符
	memset(p->buf, 0, BUFSIZE);//清空存储空间
	p->count = 0;//初始化成功读取到的字节数为0
	p->pos = 0;//初始化已写入的字节数为0
	p->state = STATE_R;//初始化默认状态为读状态
	p->errmsg = NULL;//初始化出错函数名为NULL

	saver = fcntl(p->rfd, F_GETFL);//获取rfd文件描述符的状态
	fcntl(p->rfd, F_SETFL, saver | O_NONBLOCK);//在原有的状态上加入非阻塞

	savew = fcntl(p->wfd, F_GETFL);//获取wfd文件描述符的状态
	fcntl(p->wfd, F_SETFL, savew | O_NONBLOCK);//在原有的状态上加入非阻塞

	*f = p;//把成功初始化的有限机回填

	return 0;

}

int fsm_drive(fsm_t *f)
{
	int ret = 0;//存储成功写入的字节数
	//推动有限状态机
	switch(f->state)//通过switch语句推倒有限状态机
	{
		case STATE_R : //读的状态	
			f->count = read(f->rfd, f->buf, BUFSIZE);//读取rfd文件的数据
			if(f->count == -1)//判断是否读取失败
			{
				if(errno != EAGAIN)//判断是否是假错
				{
					f->errmsg = "read()";//记录是read()出的错
					f->state = STATE_E;//切换到E状态
				}
			}
			else if(f->count == 0)//判断是否读到了EOF
				f->state = STATE_T;//切换到T状态
			else//读取成功
			{
				f->pos = 0;//切换到w之前清空pos值
				f->state = STATE_W;//切换到w状态
			}
			break;
	case STATE_W : //写的状态
		ret = write(f->wfd, f->buf + f->pos, f->count);
		//把buf中的数据写到wfd文件中
		if(ret == -1)//判断是否写失败
		{
			f->errmsg = "write()";//记录是write(2)出的错
			f->state = STATE_E;//切换到E状态
		}
		else
		{
			if(ret < f->count)//判断是否没有写完
			{
				f->pos += ret;//记录已写入的字节数
				f->count -= ret;//减去已写入的字节数
			}
			else
			{
				f->state = STATE_R;//切换到R状态
			}
		}
		break;
	case STATE_E : //异常状态
		perror(f->errmsg);//打印错误信息
		f->state = STATE_T;//切换到T状态
		break;
	case STATE_T : //终止状态(当前在T状态什么也不做[不能结束进程])
		break;
	default :
		break;
	}
	return 0;
}

int fsm_destroy(fsm_t *f)
{
	free(f);//释放有限机的状态
	f = NULL;//避免野指针
	return 0;
}






