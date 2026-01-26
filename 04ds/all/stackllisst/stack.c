#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "llist.h"

STACK *stack_create(void)
{
	return llist_create();
}

static int is_full(STACK *s)
{
	return 0;
}

static int is_empty(STACK *s)
{
	return llist_is_empty(s);
}

int stack_push(STACK *s, int data)
{
	if(is_full(s))//判断栈是否满了
		return -1;
	llist_insert(s,&data,HEADINSERT);
	return 0;
}

int stack_pop(STACK *s, int *save)
{
	if(is_empty(s))
		return -1;
	llist_pop(s,save);
	return 0;
}

void stack_display(STACK *s)
{
	if(is_empty(s))
		return ;
	llist_display(s);	
}

void stack_destory(STACK *s)
{
	if(is_empty(s))
		return ;
	llist_destory(s);
}






