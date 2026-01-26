#include <stdio.h>
#include "queue.h"


int main(void)
{
	QUEUE *q = NULL;//开辟数据结构
	int arr[] = {11,22,33,44,55,66,77,88,99,110};
	int i = 0;
	int ret = 0;
	int save = 0;

	q = queue_creat();
	if(q == NULL)
	{
		fprintf(stderr,"queue_creat() Failed!\n");
		return -1;//创建失败，结束函数，并且返回-1；
	}
	
	for(i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		ret = queue_en(q,arr[i]);
		if(ret != 0)
		{
			printf("Queue is Full!\n");//打印队满了，并且结束函数
			break;
		}
	}
	queue_display(q);
	
	ret = queue_de(q,&save);
	if(ret != 0)
		printf("Queue Is Empty!\n");//打印队空了
	else
		printf("save = %d\n", save);
	
	queue_en(q,110);
	
	queue_display(q);
	
	queue_destory(q);
	return 0;
}






