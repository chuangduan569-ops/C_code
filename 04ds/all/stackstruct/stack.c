#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

STACK *stack_create(void)
{
	STACK *s = NULL;

	s = malloc(sizeof(STACK));
	if(s == NULL)
		return NULL;
	s->stack_ind = 0;//初始化栈顶元素为0；	

	return s;
}

static int is_full(STACK *s)
{
	return !(s->stack_ind - NAMESIZE);
}

static int is_empty(STACK *s)
{
	return !(s->stack_ind);
}

int stack_push(STACK *s, int data)
{
	if(is_full(s))//判断栈是否满了
		return -1;

	s->stack_arr[s->stack_ind++] = data;//入栈
	return 0;
}

int stack_pop(STACK *s, int *save)
{
	if(is_empty(s))
		return -1;
	
	*save = s->stack_arr[--s->stack_ind];//出栈
	return 0;
}

void stack_display(STACK *s)
{
	int i = s->stack_ind;//使用i变量代替栈顶元素做操作
	if(is_empty(s))
		return ;
	
	while(i--)
		printf("	%d\n",s->stack_arr[i]);
	printf("------------------------------\n\n");
}

void stack_destory(STACK *s)
{
	if(is_empty(s))
		return ;
	free(s);//释放栈
}






