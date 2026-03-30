#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "tbf.h"

#define TBFMAX 1024

typedef struct tbf_st
{
int token;//表示当前令牌桶的令牌数
int cps;//表示当前令牌桶的速率
int burst;//当前令牌桶的容量
}tbf_t;//定义了令牌桶的数据结构

static tbf_t *tbf_libs[TBFMAX]; //指针数组存储令牌桶库的结构

static int initd; //作为是否启动令牌桶的标志[0 没有工作 1 开始工作]

static int get_btf_pos(void)//获取当前令牌桶库中可用的位置
{
	int i = 0;
	for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
	{
		if(tbf_libs[i] == NULL)//查找到可用的位置
			return i;//返回可用下标的位置
	}
	return -1;
}

static void alarm_handler(int none)//注册SIGALRM信号的新行为
{
	int i = 0;
	alarm(1);//设置一秒的闹钟
	
	for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
	{
		if(tbf_libs[i] != NULL)//判断当前下标值的令牌桶是否正在使用
		{
			tbf_libs[i]->token += tbf_libs[i]->cps;//给当前令牌桶积攒令牌
		if(tbf_libs[i]->token > tbf_libs[i]->burst)
		//判断当前令牌桶中的令牌是否超过客户指定的上限
		tbf_libs[i]->token = tbf_libs[i]->burst;
		//把当前令牌桶中的token设置为上限制
		}
	}
}

static void module_load(void)//启动模块函数
{
	signal(SIGALRM, alarm_handler);//给SIGALRM信号设置行为
	alarm(1);//设置一秒的闹钟
}

int tbf_init(int cps, int burst)
{
	if(cps <= 0 || burst <= 0)//判断形参是否合理
		return -1;
	
	int pos = 0;//存储令牌桶中可以使用的位置
	
	if(!initd)//判断令牌桶库是否没有工作
	{
	module_load();//启动模块
	initd = 1;
	}

	pos = get_btf_pos();//获取当前令牌桶中可用的位置
	if(pos < 0)//判断可用位置是否失败
		return -1;

	tbf_libs[pos] = malloc(sizeof(tbf_t));//开辟令牌桶空间
	
	if(tbf_libs[pos] == NULL)//判断开辟空间是否失败
		return -2;

	tbf_libs[pos]->token = 0;//初始化令牌桶中的令牌数为0
	tbf_libs[pos]->cps = cps;//初始化令牌桶的速率为客户指定的速率
	tbf_libs[pos]->burst = burst;//初始化为客户指定的上限

	return pos;//返回当前初始化的令牌桶的下标
}

int tbf_fetch_token(int td, int n)
{
	int ntoken  = 0;//存储成功取走的token数量
	if(td < 0 || td >= TBFMAX || n<= 0)//判断令牌桶的描述符是否合理
		return -1;
	if(tbf_libs[td] == NULL)//判断td的令牌桶是否存在
		return -2;
	
	while(tbf_libs[td]->token <= 0)//等待下标为td的令牌桶积攒令牌
		pause(); //等通知
	if(tbf_libs[td]->token >= n)//判断下标为td的令牌桶中的令牌是否充足
		ntoken = n;//只取走客户需要的token令牌数
	else
		ntoken = tbf_libs[td]->token;//有多少取多少 
	tbf_libs[td]->token -= ntoken;//更新取走后td下标的令牌桶中的令牌数

	return ntoken;//返回成功取走的令牌数
}

int tbf_destroy(int td)
{
	if(td < 0 || td >= TBFMAX)//令牌桶的描述符不合理
		return -1;
	if(tbf_libs[td] == NULL)//令牌桶下标为td的令牌桶不存在
		return -2;

	free(tbf_libs[td]);//释放下标为td的令牌桶
	tbf_libs[td] = NULL; //避免野指针

	return 0;
}




