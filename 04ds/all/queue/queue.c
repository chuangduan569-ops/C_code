#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int queue_arr[QUEUESIZE];
int queue_front;
int queue_rear;

static int is_full(void)
{
	return ((queue_rear + 1) % QUEUESIZE == queue_front);
}

static int is_empty(void)
{
	return queue_rear == queue_front;
}

int queue_en(int data)
{
	if(is_full())
		return -1;//判断队是否满了
	queue_rear = (queue_rear + 1) % QUEUESIZE;
	queue_arr[queue_rear] = data;//入队
	return 0;
}

int queue_de(int *save)
{
	if(is_empty())
		return -1;//判断栈是否为空
	queue_front = (queue_front + 1) % QUEUESIZE;
	*save = queue_arr[queue_front];//出队
	return 0;
}

void queue_display(void)
{
	int i = 0;
	if(is_empty())
		return ;

	i = (queue_front + 1) % QUEUESIZE;
	printf("\n--------------------\n\n");
	do{
	printf("	%d",queue_arr[i]);
	i = (i + 1) % QUEUESIZE;
	}while(i != queue_rear);
	printf("    %d",queue_arr[i]);	
	printf("\n--------------------\n\n");
}


