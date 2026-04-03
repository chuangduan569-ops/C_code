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
#define NUM (MAX - MIN + 1)


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
    int  n = *(int *)arg;//数据类型转换
    if(is_prime(n))//判断是否质数
        printf("%d is Primer Number\n", n);
}


int main(void)
{
	int ret = 0;
	pthread_t tid[NUM];//存储NUM个线程标识符的数组;
	int i = 0, j = 0;
	
	for(i = MIN, j = 0; i < MAX; i++, j++)
	{
		ret = pthread_create(tid + j, NULL, thr_job, &i);
		//创建线程
		if(ret != 0)//判断创建线程是否失败
		{
			fprintf(stderr,"pthread_create : %s\n",strerror(ret));
			exit(1);
		}
	}
	
	
	for(i = 0; i < NUM; i++)
		pthread_join(tid[i], NULL);//main()线程等待创建的线程结束(收尸)

	return 0;
}


