#ifndef __STACK_H
#define __STACK_H
#include "llist.h"

typedef LLIST STACK;

STACK *stack_create(void);

int stack_push(STACK *s, int data);

int stack_pop(STACK *s, int *save);

void stack_display(STACK *s);

void stack_destory(STACK *s);


#endif



