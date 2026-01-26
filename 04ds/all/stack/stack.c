#include <stdio.h>
#include "stack.h"

int stack_arr[STACKSIZE];//使用全局数组实现栈式存储结构
int stack_ind;//使用全局变量作为栈顶元素的标记,默认标记为0

static int is_full(void)
{
	return !(stack_ind-STACKSIZE);
}

static int is_empty(void)
{
	return !(stack_ind);
}

int stack_push(int data)
{
	if(is_full())//判断栈是否满了
		return -1;
	stack_arr[stack_ind++] = data;//入栈,用数组下标入栈
	return 0;
}

void stack_display(void)
{
	int i = stack_ind;//代替栈顶元素访问栈
	if(is_empty())//判断栈是否为空
		return ;
	while(i--)
	{
		printf("	%d\n",stack_arr[i]);
	}
	printf("---------------------\n");
}

int stack_pop(int *save)
{
	if(is_empty())//判断栈是否为空
		return -1;
	*save = stack_arr[--stack_ind];//出栈
	return 0;

}




