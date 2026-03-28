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
	void *arg,	//回调函数的参数
};

static struct anytimer_st *timer[ANYTIMER_MAX];
//定义指针数组,存储闹钟结构
static int initd = 0;
//作为是否启动了闹钟库的标志[0没有工作 1已经工作]





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
	timer[pos] = mallos(sizeof(struct anytimer_st));//为闹钟结构体分配内存
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





