#ifndef __QUEUE_H
#define __QUEUE_H
#include "llist.h"

typedef LLIST QUEUE;

QUEUE *queue_creat();

int queue_en(QUEUE *q,int data);

int queue_de(QUEUE *q,int *save);

void queue_display(QUEUE *q);

void queue_destory(QUEUE *q);





#endif 
