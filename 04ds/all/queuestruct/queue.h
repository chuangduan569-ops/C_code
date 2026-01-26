#ifndef __QUEUE_H
#define __QUEUE_H

#define QUEUESIZE 6

typedef struct queue
{
	int queue_arr[QUEUESIZE];//队列数据
	int queue_front;//队头
	int queue_rear;//队尾
}QUEUE;

QUEUE *queue_creat();

int queue_en(QUEUE *q,int data);

int queue_de(QUEUE *q,int *save);

void queue_display(QUEUE *q);

void queue_destory(QUEUE *q);





#endif 
