#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "anytimer.h"

#define ANYTIMERMAX 1024

typedef struct
{
	int remain;//剩余的秒数
	HANDLER callback;//闹钟响了之后执行的回调函数
	void *arg;//回调函数的参数
	int is_used;//标记当前闹钟是否被使用[0没有工作 1已经工作]
}anytimer_st;

static anytimer_st timer_libs[ANYTIMERMAX];

//定义指针数组,存储闹钟结构
static int lib_inited = 0;//标记闹钟库是否初始化(0未被初始化 1初始化了)

static int get_anytimer_pos(void)
{
	int i = 0;
	for(i = 0; i < ANYTIMERMAX; i++)//循环遍历每个闹钟数组
	{
		if(timer_libs[i].is_used == 0)//判断当前闹钟是否可用
			return i;//返回可用的标识
	}
	return -1;
}

void alarm_handler(int none)//SIGALRM信号触发行为,遍历所有闹钟,更新剩余秒数
{
	int i = 0;
	alarm(1);
	//遍历所有闹钟
	for(i = 0; i < ANYTIMERMAX; i++)//循环遍历每个闹钟数组
	{
		if(timer_libs[i].is_used == 1)//判断当前当前闹钟是否正在使用
		{
			timer_libs[i].remain--;//更新剩余秒数
			if(timer_libs[i].remain <= 0)//判断该闹钟是否到了定时时间
				{
				if(timer_libs[i].callback != NULL)//判断是否有回调函数
					timer_libs[i].callback(timer_libs[i].arg);//执行回调函数并传参
			timer_libs[i].is_used = 0;//执行完回调函数之后把该闹钟标记为空闲状态
		}
		}
	}
}

static void lib_init(void)//初始化闹钟库
{
	int i = 0;
	for(i = 0; i < ANYTIMERMAX; i++)//循环遍历所有闹钟为空闲
	{
	timer_libs[i].remain = 0;
	timer_libs[i].callback = NULL;
	timer_libs[i].arg = NULL;
	timer_libs[i].is_used = 0;
	}
	signal(SIGALRM, alarm_handler);//设置SIGALRM信号的行为
	alarm(1);
	lib_inited = 1;//切换闹钟库使用的状态(切换为已初始化的状态)
}

extern int anytimer_init(int second, HANDLER handler, void *arg)
{
	int pos = 0;//当前闹钟库中可用的最小标识
	if(second <= 0 || handler == NULL)//参数不合法
		return -1;
	
	if(!lib_inited)//判断是否第一次使用闹钟库
	{
		lib_init();//由于第一次使用闹钟库,进行初始化
	}

	pos = get_anytimer_pos();//查找当前闹钟库可用的最小标识
	if(pos < 0)// 如果返回值小于0，表示没有空闲位置
		return -2;
	timer_libs[pos].remain = second;//存储客户指定的秒数
	timer_libs[pos].callback = handler;//存储客户指定的回调函数
	timer_libs[pos].arg = arg;////存储客户指定回调函数的参数
	timer_libs[pos].is_used = 1;//把当前的闹钟状态设置为正在使用

	return pos;
}

extern int anytimer_destroy(int timer_id)
{
	if(timer_id < 0 || timer_id >= ANYTIMERMAX)// 如果id小于0或大于等于最大闹钟数
		return -1;
	if(timer_libs[timer_id].is_used == 0)//判断用户指定的闹钟是否未使用
		return -2;
	//把用户指定的闹钟清空
	timer_libs[timer_id].remain = 0;
	timer_libs[timer_id].callback = NULL;
	timer_libs[timer_id].arg = NULL;
	timer_libs[timer_id].is_used = 0;

	return 0;

}


