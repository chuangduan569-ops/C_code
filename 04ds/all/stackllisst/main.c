#include <stdio.h>
#include "stack.h"

int main(void)
{
	STACK *s = NULL;//s指针指向栈式存储结构
	int arr[] = {11,22,33,44,55,66,77,88};
	int i = 0;
	int ret = 0;//接收返回值
	int save = 0;//save存储出栈的数据
	s = stack_create();//创建栈式存储结构
	if(s == NULL)
	{
		fprintf(stderr,"stack_create() Failed!\n");
		return -1;
	}
	
	for(i = 0; i < sizeof(arr) / sizeof(*arr); i++)
	{
		ret = stack_push(s,arr[i]);//入栈
		if(ret != 0)
		{
			printf("Stack Full!\n");
			break;
		}
	}

	stack_display(s);

	ret = stack_pop(s,&save);
	if(ret != 0)
		printf("Stack Empty!\n");
	else
		printf("save = %d\n\n",save);

	stack_push(s,110);
	
	stack_display(s);



	stack_destory(s);




	return 0;
}
















