#ifndef __QUEUE_H
#define __QUEUE_H

#define QUEUESIZE 6
/*
typedef struct queue
{
	int queuearr[QUEUESIZE];//队列
	int front;//队头
	int rear;//队尾

}QUEUE;
*/

int queue_en(int data);

int queue_de(int *save);
	
void queue_display(void);




















#endif














