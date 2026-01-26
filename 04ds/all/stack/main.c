#include <stdio.h>
#include "stack.h"

extern int stack_arr[STACKSIZE];
extern int stack_ind;

int main(void)
{
	int data[] = {11,22,33,44,55,66,77,88};//数据源
	int i = 0;//循环变量
	int ret = 0;//接收返回值
	int save = 0;//存储出栈数据
	
	for(i = 0; i < sizeof(data) / sizeof(data[0]); i++)
	{
		ret = stack_push(data[i]);//入栈
		if(ret != 0)
			{
				printf("Stack is Full\n");
				break;
			}
	}

	stack_display();//遍历栈

	ret = stack_pop(&save);//出栈
	if(ret != 0)
		printf("Stack Is Empty\n");
	else
		printf("save = %d\n",save);

	
	stack_display();//遍历栈

	stack_push(110);

	stack_display();

	return 0;
}



