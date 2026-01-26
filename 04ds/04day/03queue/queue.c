#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

QUEUE *queue_create(void)
{
	QUEUE *q = NULL;//q指针指向新开辟的队列结构
	
	q = malloc(sizeof(QUEUE));
	if(q == NULL)
		return NULL;
	q->queue_front = q->queue_rear = 0;//初始化队头队尾标记

	return q;
}
static int is_full(QUEUE *q)
{
	return ((q->queue_rear + 1) % QUEUESIZE) == (q->queue_front);
}

static int is_empty(QUEUE *q)
{
	return q->queue_front == q->queue_rear;
}
int queue_en(QUEUE *q,int data)
{
	if(is_full(q))
		return -1;//判断队列是否满了
	q->queue_rear = (q->queue_rear + 1) % QUEUESIZE;//移动队尾标记
	q->queue_arr[q->queue_rear] = data;//入队
	return 0;
}

int queue_de(QUEUE *q, int *save)
{
	if(is_empty(q))
		return -1;//判断队列是否为空
	q->queue_front = (q->queue_front + 1) % QUEUESIZE;
	*save = q->queue_arr[q->queue_front];//出队
	return 0;
}

void queue_display(QUEUE *q)
{
	int i = q->queue_front;
	if(is_empty(q))
		return ;//由于队列为空，结束函数
printf("\n-------------------------\n");	
	do{
	i = (i + 1) % QUEUESIZE;
	printf("%d ",q->queue_arr[i]);
	}while(i!=q->queue_rear);
	
	printf("\n-------------------------\n\n");
}

void queue_destroy(QUEUE *q)
{
	if(is_empty(q))
		return ;
	free(q);
}
