#include <stdio.h>
#include "stack.h"

extern int stack_arr[STACKSIZE];
extern int stack_ind;

int main(void)
{
	int data[] = {11,22,33,44,55,66,77,88,99};//数据源
	int i = 0;
	int ret = 0;
	int save = 0;//存储出战数据

	for(i=0;i<sizeof(data)/sizeof(data[0]);i++)
	{
		ret = stack_push(data[i]);//入栈
		if(ret != 0)//判断入栈是否失败
		{
			printf("Stack Is Full\n");//打印栈满的信息
			break;//栈满，跳出循环
		}
	}
	
	stack_display();//遍历栈

	ret = stack_pop(&save);//出栈
	if(ret != 0)
	printf("Stack Is Empty\n");
	else
	printf("save = %d\n\n",save);

	stack_display();

	stack_push(9527);

	stack_display();
	



	return 0;
}




