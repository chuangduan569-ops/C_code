#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include "tbf.h"



#define TBFMAX 1024

typedef struct tbf_st
{
int token;//表示当前令牌桶的令牌数
int cps;//表示当前令牌桶的速率
int burst;//当前令牌桶的容量
#if defined(PTHREAD)
	pthread_mutex_t mutex;//表示当前令牌桶的互斥量
	pthread_cond_t cond;//表示当前令牌桶的条件变量
#endif
}tbf_t;//定义了令牌桶的数据结构

static tbf_t *tbf_libs[TBFMAX]; //指针数组存储令牌桶库的结构

static int initd; //作为是否启动令牌桶的标志[0 没有工作 1 开始工作]

#if defined(PTHREAD)
static pthread_mutex_t mut_job =  PTHREAD_MUTEX_INITIALIZER;//初始化令牌桶库的互斥量
static pthread_t jobtid;//定义全局的线程标识符
#endif
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

#if defined(PTHREAD)
static void *thr_fun(void *arg)//线程要做的工作
{
	int i = 0;//循环变量
	while(1)
	{
		pthread_mutex_lock(&mut_job);//抢锁
		for(i = 0; i < TBFMAX; i++)//循环遍历令牌桶
		{
			if(tbf_libs[i] != NULL)//判断该位置的令牌桶是否在用
			{
				pthread_mutex_lock(&tbf_libs[i]->mutex);//抢指定令牌桶中的锁
				tbf_libs[i]->token += tbf_libs[i]->cps;//积攒令牌
				if(tbf_libs[i]->token > tbf_libs[i]->burst)//判断当前令牌桶中令牌是否超过上限
					tbf_libs[i]->token = tbf_libs[i]->burst;//把当前令牌桶中token设置为上限值
				pthread_mutex_unlock(&tbf_libs[i]->mutex);//解除指定令牌桶中的锁
				pthread_cond_broadcast(&tbf_libs[i]->cond);//指定令牌桶中发送条件变量变化的通知
			}
		}
		sleep(1);//睡1s(取消点  取消的位置)
		pthread_mutex_unlock(&mut_job);//解除令牌桶库的锁
	}
	pthread_exit(NULL);//终止线程
}

static void module_unload(void)//卸载模块
{
	int i = 0;//循环；变量
	pthread_cancel(jobtid);//给jobtid线程发送取消请求
	pthread_join(jobtid, NULL);//等待jobtib线程结束
	
	for(i = 0; i < TBFMAX; i++)
	{
		if(tbf_libs[i] != NULL)//判断当前令牌桶是否为空
			tbf_destroy(i);//释放令牌桶
	}
}


static void module_load(void)//加载模块
{
	int ret = 0;//用来存储返回值
	
	ret = pthread_create(&jobtid, NULL, thr_fun, NULL);
	if(ret != 0)
	{
		fprintf(stderr, "pthread_create() %s\n",strerror(ret));
		exit(1);//由于创建线程失败,终止进程,并且设置状态为1
	}
	atexit(module_unload);//设置进程终止处理函数
}

#else
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
#endif
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
#if defined(PTHREAD)
	pthread_mutex_init(&tbf_libs[pos]->mutex, NULL);//初始化令牌桶中的互斥量
	pthread_cond_init(&tbf_libs[pos]->cond, NULL);//初始化令牌桶中的条件变量

#endif
	return pos;//返回当前初始化的令牌桶的下标
}

#if defined(PTHREAD)
static int getmin(int m, int n)
{
	return m > n ? n : m;
}
#endif


int tbf_fetch_token(int td, int n)
{
	int ntoken  = 0;//存储成功取走的token数量
	if(td < 0 || td >= TBFMAX || n<= 0)//判断令牌桶的描述符是否合理
		return -1;
	if(tbf_libs[td] == NULL)//判断td的令牌桶是否存在
		return -2;
#if defined(PTHREAD)
	pthread_mutex_lock(&tbf_libs[td]->mutex);//抢指定令牌桶的锁
	while(tbf_libs[td]->token <= 0)//等待下标为td的令牌桶积攒令牌
		pthread_cond_wait(&tbf_libs[td]->cond, &tbf_libs[td]->mutex);//等待指定令牌桶的条件；变量的变化
	ntoken = getmin(tbf_libs[td]->token, n);//判断客户需要和令牌桶内令牌数哪个更下
	tbf_libs[td]->token -= ntoken;//更新td令牌桶中的令牌数
	pthread_mutex_unlock(&tbf_libs[td]->mutex);//解除指定令牌桶的锁	
#else
	while(tbf_libs[td]->token <= 0)//等待下标为td的令牌桶积攒令牌
		pause(); //等通知
	if(tbf_libs[td]->token >= n)//判断下标为td的令牌桶中的令牌是否充足
		ntoken = n;//只取走客户需要的token令牌数
	else
		ntoken = tbf_libs[td]->token;//有多少取多少 
	tbf_libs[td]->token -= ntoken;//更新取走后td下标的令牌桶中的令牌数
	pthread_mutex_unlock(&tbf_libs[td]->mutex);//解除指定令牌桶的锁
#endif
	return ntoken;//返回成功取走的令牌数
}

int tbf_destroy(int td)
{
	if(td < 0 || td >= TBFMAX)//令牌桶的描述符不合理
		return -1;
	if(tbf_libs[td] == NULL)//令牌桶下标为td的令牌桶不存在
		return -2;
#if defined(PTHREAD)
	pthread_mutex_destroy(&tbf_libs[td]->mutex);//释放互斥量
	pthread_cond_destroy(&tbf_libs[td]->cond);//释放条件变量
#endif
	free(tbf_libs[td]);//释放下标为td的令牌桶
	tbf_libs[td] = NULL; //避免野指针

	return 0;
}

#if defined(PTHREAD)

int tbf_return_token(int td, int ntoken)
{
	if(td < 0 || td >= TBFMAX || ntoken <= 0)
		return -1;
	if(tbf_libs[td] == NULL)
		return -2;
	pthread_mutex_lock(&tbf_libs[td]->mutex);//抢指定令牌桶的锁
	tbf_libs[td]->token += ntoken;//返回令牌
	if(tbf_libs[td]->token > tbf_libs[td]->burst)//判断返还后的令牌数是否超过上限
		tbf_libs[td]->token = tbf_libs[td]->burst;//如果超过上限就等于上限
	pthread_mutex_unlock(&tbf_libs[td]->mutex);//解除指定令牌桶的锁
	pthread_cond_broadcast(&tbf_libs[td]->cond);//给指定令牌桶中发送条件变量变化的通知

	return 0;	
}

void tbf_destroy_all(void)
{
	int i = 0;
	pthread_mutex_lock(&mut_job);//抢令牌桶库的锁
	for(i = 0; i < TBFMAX; i++)//循环销毁令牌桶
	{
		if(tbf_libs[i] != NULL)
			tbf_destroy(i);
	}
	pthread_mutex_unlock(&mut_job);//解除令牌桶库的锁
}

#endif








