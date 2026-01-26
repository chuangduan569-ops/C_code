#include <stdio.h>
#include "queue.h"
int main(void)
{
	int arr[] = {11,22,33,44,55,66,77,88};
	int i = 0;
	int ret = 0;
	int save = 0;

	for(i = 0; i < sizeof(arr) / sizeof(*arr); i++)	
	{
		ret = queue_en(arr[i]);
		if(ret != 0)
		{
			printf("Queue full\n");
			break;
		}
	}
	
	queue_display();//遍历队列

	ret = queue_de(&save);//出队
	if(ret != 0)
		printf("queue Empty!\n");
	else
		printf("save = %d\n",save);

	queue_display();

	queue_en(110);

	queue_display();
	return 0;
}




