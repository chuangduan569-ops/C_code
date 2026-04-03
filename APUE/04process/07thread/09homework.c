
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define NUM 5   //修改要创建线程的个数

#define COND

/*
job == -1   表示任务没有发放
job == 0    表示打印'a'的线程可以工作
job == 1    表示打印'b'的线程可以工作
job == 2    表示打印'c'的线程可以工作
job == 3    表示打印'd'的线程可以工作
job == 4    表示打印'e'的线程可以工作
*/
static int job = -1;//初始化临界区资源
/*
全局的job相当于是任务池
所有的线程都会来访问所以job是个临界区
我们在写并发任务时需要给临界区加上互斥量(互斥锁)
*/
//[1]初始化互斥量
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;//用宏初始化互斥量

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;//用宏初始化条件变量


static void *thr_job(void *num);//线程要干的事

int main(void)
{
    /*
        [1]初始化互斥量(要么调用pthread_mutex_init(3) | 使用宏)
        [2]创建所有的任务线程(4个)
        [3]发放任务
        [4]告诉线程们,所有任务发放完毕
        [5]等着收尸
        [6]销毁互斥量
    */
    pthread_t tids[NUM];//存储NUM个线程标识符的数组
    int i = 0;//循环变量
    int ret = 0;//存储返回值

    alarm(5);//定时5s

    //[2]创建所有的任务线程(5个)
    for(i = 0; i < NUM; i++)
    {
        ret = pthread_create(tids + i, NULL, thr_job, (void *)(i));
        //创建线程
        if(ret != 0)//判断创建线程是否失败
        {
            fprintf(stderr, "pthread_create():%s\n", strerror(ret));
            //打印错误信息
            exit(1);//由于创建线程失败,终止进程,并且设置状态1
        }
    }

    //[3]发放任务
    /*
        我们需要等上一次发放的任务取走之后在分配新的任务
        想要获取job的值,需要先访问互斥量
    */
    for(i = 0; ; i = (i + 1) % NUM)
    {
        //main线程要访问job,所以需要加锁
        pthread_mutex_lock(&mut);
        //如果job已经被其他线程lock了,会阻塞,直到main线程拿到lock为止
        //能够执行到这里说明已经拿到锁了
        while(job != -1)//轮询直到job != -1
        {
            //原子操作:[1]mut->unlock [2]等待条件 [3]mut->lock
            pthread_cond_wait(&cond, &mut);//等待条件通知
        }
        job = i;//发布新任务
        pthread_cond_broadcast(&cond);//由于修改了job的值,发送通知
        pthread_mutex_unlock(&mut);//解锁
    }

    //因为是疯跑所以没有任务发放结束的时候

    //[5]main()线程等待创建的线程结束(收尸)
    for(i = 0; i < NUM; i++)
        pthread_join(tids[i], NULL);//收尸

    //[6]销毁互斥量
    pthread_mutex_destroy(&mut);//销毁互斥量
    pthread_cond_destroy(&cond);//销毁条件变量


    return 0;
}

static void *thr_job(void *num)//线程要干的事
{
    int n = (int)(num);//独立的存储

    while(1)
    {
        pthread_mutex_lock(&mut);//加锁
        //线程使用加锁解锁最怕出现的问题是死锁
        while(job != n)//因为会出现死锁的问题,所以改用while
        {//因为while循环想要判断需要等到条件改变
            //原子操作:[1]mut->unlock [2]等待条件 [3]mut->lock
            pthread_cond_wait(&cond, &mut);//等待条件改变的通知
        }
        putchar('a' + n);//打印输出字符
        fflush(NULL);//刷新缓冲区

        job = -1;//修改临界区的状态
        pthread_cond_broadcast(&cond);//由于取走了资源,通知其他线程工作
        //为啥不用pthread_cond_signal(3)?
        //因为pthread_cond_signal(3)只唤起一个线程,没办法确定是否是main线程
        pthread_mutex_unlock(&mut);//解锁
    }
}



