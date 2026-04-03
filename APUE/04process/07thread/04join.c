#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void *thr_job(void *arg)//线程执行的函数
{
	int i = 0;
	for(i = 0; i < 10; i++)
	{
		if(i == 5)
			//pthread_exit((void *) 0);//终止线程
		pthread_exit(NULL);
		//return NULL;
		write(1, "!", 1);//往标准输出写1byte *
		sleep(1);
	}
	//pthread_exit((void *) 0);//终止线程
}


int main(void)
{
	pthread_t tid;//存储新创建线程的标识
	int ret = 0;//存储返回值
	int i = 0;

	ret = pthread_create(&tid, NULL, thr_job, NULL);//创建线程
	if(ret != 0)//判断创建线程是否失败
	{
		fprintf(stderr,"pthread_create() %s\n", strerror(ret));
		exit(1);//由于创建线程失败 终止进程 设置状态1
	}

	for(i = 0; i < 3; i++)
	{
		write(1, "*", 1);//往标准输出写1byte *
		sleep(1);//睡1s
	}

	pthread_join(tid, NULL);//等待创建的进程结束
	return 0;
}




