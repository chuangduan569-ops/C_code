#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define MIN 100
#define MAX 300
#define NUM 4
static int job;//全局变量 把控临界区
/*
job == 0  表示任务还没有发送
job > 0	表示任务发放了 但是还未被取走
job = -1	表示所有任务都发放完了
Job静态的全局变量 所有的线程都会访问\修改job 所以就是临界区
容易出现竞态 为了避免出现竞态 需要给临界区加互斥量(互斥锁(加锁))

*/
//[1]初始化互斥量(要么调用pthread_mutex_init(3) 或者 使用宏)
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//初始化互斥量

int is_prime(int num)
{
	int i = 0;
	
	sleep(1);

	if(num <= 1)	//判断num是否小于等于1(是否不是负数)
		return 0;
	if(num == 2 || num == 3)
		return 1;
		
	for(i = 2; i <= num / i; i++)
	{
		if(num % i == 0)
		return 0;
	}
	return 1;
}

void *thr_job(void *arg)//线程要做的事情
{
/*
	循环抢任务
	[1] 遇到 job == -1	说明任务发放完毕->终止线程
	[2] 遇到 job == 0	说明还没有分配任务->等待抢任务
	[3] 遇到 job > 0	说明有任务->处理任务 
*/
	int n = 0;//另存空间
	while(1)
	{
		pthread_mutex_lock(&mutex);//加锁
		if(job == -1)//判断任务是否发放完毕
		{
			pthread_mutex_unlock(&mutex);//解锁
			pthread_exit(0);//终止线程
		}
		if(job == 0)//判断任务是否还没有发放
		{//也得解锁 不能拿着锁不放
			pthread_mutex_unlock(&mutex);
			continue;//跳过本次循环,继续下一次循环
		}
		n = job;//把临界区资源拷贝一份
		job = 0;//修改取走后临界区的状态
		pthread_mutex_unlock(&mutex);//解锁
		if(is_prime(n))//判断是否质数
			printf("%d is Primer Number\n", n);
		sleep(1);//模拟工作线程处理的任务 工作比较复杂 用的时间长
	}
}


int main(void)
{
	/*
	[1]初始化互斥量(要么调用pthread_mutex_init(3) 或者 使用宏)
	[2]创建所有的工作线程(pthread_creat(3))
	[3]发放任务
	[4]告诉工作线程门 所有任务发放完毕
	[5]main()线程等待工作线程门结束(收尸)	
	[6]销毁互斥量
	 */
	int ret = 0;
	pthread_t tid[NUM];//存储NUM个线程标识符的数组;
	int i = 0, j = 0;
	
	for(i = 0; i < NUM; i++)
	{
		//[2]创建所有的工作线程(pthread_creat(3))
		ret = pthread_create(tid + i, NULL, thr_job, NULL);
		//创建线程
		if(ret != 0)//判断创建线程是否失败
		{
			fprintf(stderr,"pthread_create : %s\n",strerror(ret));
			exit(1);
		}
	}
	//[3]发放任务
	for(i = MIN; i < MAX; i++)
	{//我们需要等上一个任务被取走之后再分配新的任务
	//需要访问/修改job的值 所以需要先访问互斥量
	pthread_mutex_lock(&mutex);//加锁
	//如果互斥量已经被其他线程加锁 会阻塞 直到这个线程拿到为止
	//能够执行到此 说明已经拿到互斥量了
	while(job > 0)//轮训等待job==0(言外之意是轮询等待其他线程取走任务)
	{
		pthread_mutex_unlock(&mutex);//解锁
		pthread_mutex_lock(&mutex);//加锁
	}
	job = i;//main()线程发放新的任务
	pthread_mutex_unlock(&mutex);//解锁
	}
	//[4]告诉工作线程门 所有任务发放完毕(也就是 job == -1)
	pthread_mutex_lock(&mutex);//加锁
	while(job > 0)//轮询直到job <= 0为止
    {
        pthread_mutex_unlock(&mutex);//解锁
        pthread_mutex_lock(&mutex);//加锁
    }
	job = -1;//告诉线程们,所有任务发放完毕
	//main()线程改变临界区状态为发放完毕
	pthread_mutex_unlock(&mutex);//解锁

	//[5]main()线程等待工作线程门结束(收尸) 
	for(i = 0; i < NUM; i++)
		pthread_join(tid[i], NULL);//main()线程等待创建的线程结束(收尸)

	//[6]销毁互斥量
	pthread_mutex_destroy(&mutex);//销毁互斥量

	return 0;
}


