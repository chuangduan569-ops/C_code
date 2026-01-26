#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

int queue_init(queue_t **q, int capacity, int size)
{
	*q = malloc(sizeof(queue_t));
	if(*q == NULL)
		return -1;
	(*q)->s = calloc(capacity + 1, size);
	if((*q)->s == NULL)
	{
		free(*q);
		*q = NULL;//这里取的地址，避免使用到野指针
		return -2;
	}
	(*q)->front = (*q)->rear = 0;
	(*q)->capacity = capacity + 1;//在队列结构中存储客户指定的容量 + 1
	(*q)->size = size;

	return 0;
}


int queue_is_empty(const queue_t *q)
{
	return q->front == q->rear;
}

int queue_is_full(const queue_t *q)
{
	return ((q->rear + 1) % q->carefully) == q->front;
}

int queue_en(queue_t *q,const void *data)
{
	if(queue_is_full(q))
		return -1;
	q->rear = (q->rear + 1) % q->capacity;
	memcpy((char *)q->s+q->rear*q->size, data, q->size);
	return 0;
}

int queue_de(queue_y *q,void *data)
{
	if(queue_is_empty(q))
		return -1;
	q->front = (q->front + 1) % q->capacity;
	memcpy(data, (char *)q->s+q->front*q->size, q->size);
	memset((char *)q->s+q->front*q->size, '\0', q->size);
	return 0;
}

void queue_destroy(queue_t *q)
{
	free(q->s);//先释放队列的空间
	free(q);//再释放队结构的空间
}



