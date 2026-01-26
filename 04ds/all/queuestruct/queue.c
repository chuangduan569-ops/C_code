#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

QUEUE *queue_creat()
{
	QUEUE *q = NULL;
	
	q = malloc(sizeof(QUEUE));//开辟队列结构
	if(q == NULL)
		return NULL;

	q->queue_front = q->queue_rear = 0;//初始化为0：
	return q;
}

static int is_full(QUEUE *q)
{
	return ((q->queue_rear + 1) % QUEUESIZE) == q->queue_front;
}

static int is_empty(QUEUE *q)
{
	return q->queue_front == q->queue_rear;
}


int queue_en(QUEUE *q,int data)
{
	if(is_full(q))//判断栈是否满了
		return -1;
	q->queue_rear = (q->queue_rear + 1) % QUEUESIZE;//队尾入队，调整位置
	q->queue_arr[q->queue_rear] = data;//入队
	return 0;
}

int queue_de(QUEUE *q, int *save)
{
	if(is_empty(q))//判断队是否为空
		return -1;
	q->queue_front = (q->queue_front + 1) % QUEUESIZE;//队头出队。调整位置
	*save = q->queue_arr[q->queue_front];//出队
	return 0;
}

void queue_display(QUEUE *q)
{
	int i = q->queue_front;
	if(is_empty(q))//判断队是否为空
    return ;
	printf("\n--------------------------------\n");
	do{
		i = (i + 1) % QUEUESIZE;
		printf(" %d",q->queue_arr[i]);
	//	i = (i + 1) % QUEUESIZE;
	}while(i != q->queue_rear);
	printf("\n--------------------------------\n\n");
	
}

void queue_destory(QUEUE *q)
{
	if(is_empty(q))//判断队是否为空
	return ;
	free(q);
}




