#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "tbf.h"

#define TBFMAX 1024

typedef struct tbf
{
	int token;//token值
	int cps;//速率(积攒令牌的速率以及取走令牌的速率)
	int burst;//令牌桶中令牌的上限
}tbf_t;//定义令牌桶类型

static tbf_t *tbf_libs[TBFMAX]; //指针数组存储令牌桶结构

static int initd;	//作为是否启动了令牌桶的标志[0 没有工作  1 已经工作]

static int get_btf_pos(void)//获取当前令牌桶库中可用的位置
{
	int i = 0;
	
	for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
	{
		if(tbf_libs[i] == NULL) //查找可用的位置
			return i;//返回可用下标的位置
	}
	return -1;
}

static void alarm_handler(int node)//注册SIGALRM信号的新行为
{
	int i = 0;

	alarm(1);//设置1s的闹钟
	for(i = 0; i < TBFMAX; i++) //循环遍历令牌桶库
	{
		if(tbf_libs[i] != NULL) //判断当前下标值的令牌桶是否正在使用
		{
			tbf_libs[i]->token += tbf_libs[i]->cps;//给当前令牌桶积攒令牌
			if(tbf_libs[i]->token > tbf_libs[i]->burst)
			//判断当前令牌桶中的令牌是否超过客户指定令牌的上限
				tbf_libs[i]->token = tbf_libs[i]->burst;
				//把当前令牌桶中的token设置为上限值
		}	
	}
}

static void insmod(void) //启动模块实现
{
	signal(SIGALRM, alarm_handler); //设置SIGALRM信号的新行为
	alarm(1);//设置闹钟
}

extern int tbf_init(int cps, int burst)
{
	int pos = 0;//存储令牌桶库中可以使用的令牌桶位置

	if(!initd) //判断令牌桶是否开启
	{
		insmod();//安装模块
		initd = 1;//设置为开始工作状态
	}
	
	pos = get_btf_pos();//获取当前令牌桶库中可用位置
	if(pos < 0)//判断可用位置是否失败
		return -1;

	tbf_libs[pos] = malloc(sizeof(tbf_t));//开辟令牌桶空间
	if(tbf_libs[pos] == NULL)//判断了开辟空间是否失败
		return -2;

	tbf_libs[pos]->token = 0;//初始化令牌桶中的令牌数为0
	tbf_libs[pos]->cps = cps;//初始化令牌桶的速率为客户指定的速率
	tbf_libs[pos]->burst = burst;//初始化为客户指定的上限

	return pos;//返回当前初始化的令牌桶的下标
}

extern int tbf_fetch_token(int td, int ntoken)
{
	int n = 0;//存储成功取走的token数量

	if(td < 0 || td > TBFMAX)//判断令牌桶的描述符是否合理
		return -1;
	if(tbf_libs[td] == NULL) //判断下标为td的令牌桶是否存在
		return -2;

	while(tbf_libs[td]->token <= 0) //等待下标为td的令牌桶积攒令牌
		pause(); //等通知

	if(tbf_libs[td]->token >= ntoken)//判断下标为td的令牌桶中的令牌是否充足
		n = ntoken;//只取走客户需要的token令牌数
	else
		n = tbf_libs[td]->token;//有多少取多少
	tbf_libs[td]->token -= n;//更新取走后td下标的令牌桶中的令牌数

	return n;//返回成功取走的令牌数
}

extern int tbf_destroy(int td)
{
	if(td < 0 || td > TBFMAX)//令牌桶的描述符不合理
		return -1;
	if(tbf_libs[td] == NULL)//令牌桶下标为td的令牌桶不存在
		return -2;

	free(tbf_libs[td]);//释放下标为td的令牌桶
	tbf_libs[td] = NULL;//避免野指针出现

	return 0;
}


