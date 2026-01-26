#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

QUEUE *queue_creat()
{
	return llist_create();
}

static int is_full(QUEUE *q)
{
	return 0;
}

static int is_empty(QUEUE *q)
{
	return llist_is_empty(q);
}


int queue_en(QUEUE *q,int data)
{
	if(is_full(q))//判断栈是否满了
		return -1;
	llist_insert(q, &data, TAILINSERT);
	return 0;
}

int queue_de(QUEUE *q, int *save)
{
	if(is_empty(q))//判断队是否为空
		return -1;
	llist_pop(q,save);
}

void queue_display(QUEUE *q)
{
	if(is_empty(q))//判断队是否为空
    return ;
	llist_display(q);
	printf("\n--------------------------------\n\n");
	
}

void queue_destory(QUEUE *q)
{
	if(is_empty(q))//判断队是否为空
		return ;
	llist_destory(q);
}




