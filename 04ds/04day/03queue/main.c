#include <stdio.h>
#include "queue.h"

int main(void)
{
	QUEUE *q = NULL;//指向队列的结构
	int arr[] = {11,22,33,44,55,66,77,88};
	int i = 0;
	int ret = 0;
	int save = 0;

	q = queue_create();
	if(q==NULL)
	{
		fprintf(stderr,"Create Is Failed\n");
		return -1;
	}

	for(i=0;i < sizeof(arr) / sizeof(*arr); i++)
	{
		ret = queue_en(q,arr[i]);
		if(ret != 0)
		{
			printf("Queue Is Full");
			break;
		}
	}

	queue_display(q);

	ret = queue_de(q,&save);
	if(ret!=0)
		printf("Is Empty\n");
	else
		printf("save = %d\n",save);
	queue_display(q);
	
	
	queue_en(q, 9527);	

	queue_display(q);

	queue_destroy(q);



	return 0;
}





