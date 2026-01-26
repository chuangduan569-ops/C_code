#include <stdio.h>
#include "queue.h"
int queue_arr[QUEUESIZE];//队列存储空间
int queue_front;//队头的标记
int	queue_rear;//队尾的标记

static int is_full(void)
{
	return ((queue_rear + 1) % QUEUESIZE == queue_front);
}
static int is_empty(void)
{
	return queue_front == queue_rear;
}


int queue_en(int data)
{
	if(is_full())//判断队列是否满了
	return -1;
	queue_rear = (queue_rear + 1) % QUEUESIZE;//移动队尾标记
	queue_arr[queue_rear] = data;//入队
	return 0;
}

int queue_de(int *save)
{
	if(is_empty())
	return -1;
	queue_front = (queue_front + 1) % QUEUESIZE;//移动队头标记
	*save = queue_arr[queue_front];
	return 0;
}
void queue_display(void)
{
	int i = 0;//使用i变量代替front做操作
	if(is_empty())//判断队列是否为空
		return ;//队列空了，返回-1
	i = (queue_front + 1) % QUEUESIZE;
	printf("\n-------------------------\n");
	do
	{
		printf("%d ",queue_arr[i]);
		i = (i + 1) % QUEUESIZE;
	}while(i != queue_rear);
	printf("%d ",queue_arr[i]);
	
	
	printf("\n-------------------------\n\n");
}












/*
队列满
	(rear + 1) % QUEUESIZE == front
入队操作
	rear = (rear + 1) % QUEUESIZE;
出队操作
	front = (front + 1) %QUEUESIZE
空队列
	front == rear
*/
