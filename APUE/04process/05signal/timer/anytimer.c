#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "anytimer.h"

#define ANYTIMER_MAX 1024

struct anytimer_st
{
	int sec;	//剩余秒数,用于倒计时
	handler_t func;	//会嗲奥函数指针,定时后调用
	void *arg;	//回调函数的参数
};

static struct anytimer_st *timer[ANYTIMER_MAX];
//定义指针数组,存储闹钟结构
static int initd = 0;
//作为是否启动了闹钟库的标志[0没有工作 1已经工作]

//获取闹钟当前可用位置
static int get_free_pos(void)
{
	int i = 0;
	for(i = 0; i < ANYTIMER_MAX; i++)//循环遍历整个闹钟组
	{
		if(timer[i] == NULL)//如果当前位置是空的,获取当前位置
		return i;
	}
	return -1;
}

/*
SIGALRM信号处理函数 (每一秒调用一次)
功能 : 遍历所有闹钟 将每个闹钟的剩余秒数减1
		当剩余秒数为0时 执行回调函数并且销毁闹钟
*/
static void alarm_handler(int sig)//信号处理函数,sig是信号编号
{
	int i = 0;
	//遍历所有闹钟
	for(i = 0; i < ANYTIMER_MAX; i++)//循环遍历每个闹钟数组
	{
		if(timer[i] != NULL)//如果当前位置有闹钟
		{
			timer[i]->sec--;//将闹钟的剩余秒数-1
			if(timer[i]->sec == 0)//如果剩余秒数减到0
			{
				//执行回调函数,并且回调函数不为空才执行
				if(timer[i]->func != NULL)
					timer[i]->func(timer[i]->arg);//调用回调函数,并且把arg参数传入函数

			//执行完之后释放内存,销毁闹钟
			free(timer[i]);//释放闹钟结构体内存
			timer[i] = NULL;//避免野指针
			}
		}
	}
}


//启动模块  
//功能 注册SIGALRM 信号处理函数  设置1s定时器
static void insmod(void)
{
	struct sigaction act;//设置sigaction结构体,用于设置和信号处理
	struct itimerval itv;//设置itimerval结构体,用于设置定时器
	//注册SIGALRM信号处理函数
	act.sa_handler = alarm_handler;//设置信号处理函数为alarm_handler
	sigemptyset(&act.sa_mask);//清空信号集
	act.sa_flags = 0;//设置标志为为0,无特殊标志
	sigaction(SIGALRM, &act, NULL);//为SIGALRM信号设置新的行为

	//设置定时器 : 每秒触发一次SIGALRM信号
	itv.it_interval.tv_sec = 1;//设置间隔时间为1s
	itv.it_interval.tv_usec = 0;//设置间隔微妙部分为0
	itv.it_value.tv_sec = 1;//设置第一次触发时间为1s
	itv.it_value.tv_usec = 0;//设置第一次触发微妙部分为0
	setitimer(ITIMER_REAL, &itv, NULL);//调用setitimer设置实时定时器

}

int anytimer_init(int sec, handler_t func, void *arg)
{
	int pos = 0;//存储可用闹钟库的下标

	if(sec <= 0)// 如果定时秒数小于等于0
		return -1;
	if(func == NULL)// 如果回调函数为空
		return -2;
	
	if(!initd)// 如果initd为0，表示未初始化
	{
		insmod();	//调用函数初始化模块
		initd = 1;  //将initd设置为1,表示已初始化
	}

	pos = get_free_pos();//调用函数获取空闲位置的下标
	if(pos < 0)// 如果返回值小于0，表示没有空闲位置
		return -3;

	//分配内存
	timer[pos] = malloc(sizeof(struct anytimer_st));//为闹钟结构体分配内存
	if(timer[pos] == NULL)//内存分配失败
		return -4;

	//初始化结构体的值为 传入的sec func arg
	timer[pos]->sec = sec;
	timer[pos]->func = func;
	timer[pos]->arg = arg;

	return pos;//返回闹钟ID（即数组下标）
}

int anytimer_destroy(int id)
{
	if(id < 0 || id >= ANYTIMER_MAX)// 如果id小于0或大于等于最大闹钟数
		return -1;
	if(timer[id] == NULL)// 如果指定位置的闹钟不存在
		return -2;
	
	free(timer[id]);// 释放闹钟结构体内存
	timer[id] = NULL;//避免野指针

	return 0;

}





