#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "thread_pool.h"
#include "log.h"

static void *admin_job(void *arg);//管理者线程要做的事情
static void *worker_job(void *arg);//工作线程要做的事情

int pool_init(pool_t **mypool, int capacity)
{
	pool_t *me = NULL;//存储开辟成功的线程池地址
	int err = 0;//存储返回值	
	int i = 0;//循环变量

	me = malloc(sizeof(pool_t));//开辟线程池空间
	if(me == NULL)//判断是否开辟失败
		return -1;
	
	//动态开辟工作线程结构
	me->workers = calloc(capacity, sizeof(pthread_t));
	if(me->workers == NULL)//判断动态开辟工作线程结构是否失败
	{
		free(me);//释放线程池结构
		return -2;
	}
	//任务队列初始化
	queue_init(&me->task_queue, MAXJOB, sizeof(task_t));
	me->max_threads = capacity;//存储可和指定的最多线程并发个数
	me->min_free_threads = MIN_FREE_NR;//指定的最小空闲线程个数
	me->max_free_threads = MAX_FREE_NR;//指定的最大空闲线程个数
	me->busy_threads = 0;//指定busy线程数0
	me->live_threads = me->min_free_threads;//指定live线程数为最小空闲线程个数
	me->exit_threads = 0;//指定需要终止的线程数为0
	me->shutdown = 0;//指定是否需要关闭线程池的标记为0(正常运行)

	//互斥量以及条件变量初始化
	pthread_mutex_init(&me->mut_pool, NULL);//初始化整个线程池的互斥量
	pthread_mutex_init(&me->mut_busy, NULL);//初始化busy线程的互斥量
	pthread_cond_init(&me->queue_not_empyt, NULL);//初始化任务队列不为空的条件变量
	pthread_cond_init(&me->queue_not_full, NULL);//初始化任务队列不为满的条件变量
	
	//me->workers每一个成员初始化为-1(为了告诉使用者这个位置上没有线程)
	memset(me->workers, -1, capacity * sizeof(pthread_t));

	//需要构建管理者线程,填入标识符
	err = pthread_create(&me->admin_tid, NULL, admin_job, me);
	if(err != 0)//判断创建管理者线程是否失败
	{
		free(me->workers);//释放工作线程结构
		free(me);//释放整个线程池
		return -err;//由于创建管理者线程失败 结束函数 并且返回-错误玛
	}
	
	//创建线程池最小free的线程
	for(i = 0; i < me->min_free_threads; i++)
	{
		err = pthread_create(me->workers + i, NULL, worker_job, me);//创建工作线程
		if(err != 0)//判断创建工作线程是否失败
		{
			free(me->workers);//释放工作线程结构
			free(me);//释放整个线程池
			return -err;//由于创建工作线程失败 结束函数 并且返回-错误玛
		}
		//线程分离(将来可以不用收尸)
		pthread_detach(me->workers[i]);//创建成功一次我就分离
	}
	
	*mypool = me;//把开辟成功的线程池地址,回填
	return 0;
}

//找到工作线程数组中空闲的位置
static int  __get_free_pos(pthread_t *jobs, int n) 
{
	int i = 0;
	for(i = 0; i < n; i++)//轮询查找工作线程中是否存在没有使用过的位置
		if(jobs[i] == -1)
			return i;
	//检测线程是否存在
	for(i = 0; i < n; i++)
		if(pthread_kill(jobs[i], 0) == ESRCH)//判断当前工作的线程是否不存在
			return i;//如果存在返回当前位置的下标
	return -1;
}

//管理者线程要做的事情
void *admin_job(void *arg)
{
	pool_t *mypool = arg;//把形参转换为线程池结构
	int busy_cnt = 0;//记录有多少个busy线程
	int free_cnt = 0;//记录有多少个free线程
	int i = 0;
	int pos = 0;//标记合适位置
	static int debug_counter = 0;  // 用于限制日志频率

	while(1)//使用轮询法
	{
		pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的锁
		if(mypool->shutdown)//判断线程池是否关闭了
		{
			pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的锁
			break;//跳出死循环
		}
		pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
		busy_cnt = mypool->busy_threads;//获取线程池中有多少个busy线程
		pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁
		free_cnt = mypool->live_threads - busy_cnt;//计算线程池中有多少个free线程

		//如果空闲的线程数大于最大空闲线程 + 预留线程 需要减少线程
		if(free_cnt >= mypool->max_free_threads + STEP)//判断是否需要减少线程
		{
			mypool->exit_threads = STEP;//每次减少STEP个线程
			for(i = 0; i < STEP; i++)//STEP是几 就发送几次条件变量变化的通知
			{
				pthread_cond_signal(&mypool->queue_not_empyt);//发送工作队列不为空的通知
				//不是为了取任务 主要是为了唤醒线程 
			}
		}
		//如果现在busy线程和live线程一样多,而且没有达到线程的上限->增加线程
		if(busy_cnt == mypool->live_threads &&\
				mypool->live_threads < mypool->max_threads)//判断是否需要增加线程		
		{
			for(i = 0; i < STEP; i++)
			{//需要把新创建的线程放到工作线程的合适位置,所以需要查找合适的位置
				pos = __get_free_pos(mypool->workers, mypool->max_threads);
				if(pos == -1)//判断线程池中是否没有多与的工作线程位置了
				{
					fprintf(stderr, "[%d] __get_free_pos() failed\n", __LINE__);
					//打印出错函数信息
					break;
				}
				pthread_create(mypool->workers + pos, NULL, worker_job, mypool);//创建新的工作线程
				pthread_detach(mypool->workers[pos]);//线程分离(将来不用收尸)
				mypool->live_threads++;//创建成功后增加live线程数
			}
		}
		
		
		// 每10秒输出一次线程池状态（避免日志过多）
        debug_counter++;
        if(debug_counter >= 10)
        {
            LOG_INFO("Thread pool status - busy:%d free:%d live:%d max:%d",
                     busy_cnt, free_cnt, mypool->live_threads, mypool->max_threads);
            debug_counter = 0;
        }

		pthread_mutex_unlock(&mypool->mut_pool);//解整个线程的锁
		sleep(1);///管理者线程1s看一次线程池
	}
	pthread_exit(0);//终止线程
}

//工作线程要做的事情
void *worker_job(void *arg)
{
	pool_t *mypool = arg;//把形参转换为线程池类型
	task_t mytask;//存储出队的任务	

	while(1)
	{
		pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
		if(queue_is_empty(mypool->task_queue))//判断任务队列是否为空
		{
			pthread_cond_wait(&mypool->queue_not_empyt, &mypool->mut_pool);
			//原子操作,等待队列不为空的条件并且解整个线程池的互斥锁
		}
		if(mypool->shutdown)//判断线程池是否已关闭
		{
			pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
			break;//跳出循环
		}
		if(mypool->exit_threads > 0)//判断是否有线程需要终止
		{
			mypool->exit_threads--;//终止线程之前减少要终止的线程个数
			mypool->live_threads--;//终止线程之前减少活着的线程个数
			pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
			break;//跳出循环
		}
		//任务来了
		queue_de(mypool->task_queue, &mytask);//出队任务
		pthread_cond_signal(&mypool->queue_not_full);//发送队列不为满的通知
		pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
		//做任务(free线程要变为busy线程)
		pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
		mypool->busy_threads++;//增加busy线程的数量
		pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁
		
		(mytask.job)(mytask.arg);//执行任务
		pthread_mutex_lock(&mypool->mut_busy);//抢busy线程的互斥锁
		mypool->busy_threads--;//减少busy线程的数量
		pthread_mutex_unlock(&mypool->mut_busy);//解busy线程的互斥锁
	}
	pthread_exit(0);//终止线程
}

int pool_add_task(pool_t *mypool, const task_t *t)
{
	pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
	while(queue_is_full(mypool->task_queue))//判断任务队列是否已满
	{
		//原子操作,等待队列不为满的条件并且解整个线程池的互斥锁
		pthread_cond_wait(&mypool->queue_not_full, &mypool->mut_pool);
	}
	queue_en(mypool->task_queue, t);//任务入队
	pthread_cond_signal(&mypool->queue_not_empyt);//发送任务队列不为空的通知
	pthread_mutex_unlock(&mypool->mut_pool);//解整个线程池的互斥锁
	return 0;
}

void pool_destroy(pool_t *mypool)
{
	pthread_mutex_lock(&mypool->mut_pool);//抢整个线程池的互斥锁
	mypool->shutdown = 1;//把线程池关闭状态设置为1
	pthread_cond_broadcast(&mypool->queue_not_empyt);//骗线程(目地为了释放所有线程)
	pthread_mutex_unlock(&mypool->mut_pool);
	sleep(1);//阻塞1s

	free(mypool->workers);//销毁工作线程结构
	queue_destroy(mypool->task_queue);//销毁任务队列
	pthread_mutex_destroy(&mypool->mut_pool);//销毁整个线程池的互斥量
	pthread_mutex_destroy(&mypool->mut_busy);//销毁busy的互斥量
	pthread_cond_destroy(&mypool->queue_not_empyt);//销毁任务队列不为空的条件变量
	pthread_cond_destroy(&mypool->queue_not_full);//销毁任务队列不为满的条件变量
	free(mypool);//销毁整个线程池
}




