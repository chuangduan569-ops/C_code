#include <stdio.h>
#include "queue.h"

int main(void)
{
	int arr[] = {11,22,33,44,55,66,77,88};
	int i = 0;
	int ret = 0;	
	int save = 0;

	for(i= 0i;i<sizeof(arr) / sizeof(*arr);i++)
	{
		ret = queue_en(arr[i]);//入队
		if(ret != 0)//入队失败
		{
			printf("Is Full\n");
			break;
		}
	}
	
	
	queue_display();

	ret = queue_de(&save);//出队
	if(ret!=0)
		printf("Queue Is Empty\n");
	else
		printf("save = %d\n",save);

	queue_display();

	queue_en(9527);//入队

	queue_display();//遍历队列



	return 0;
}




