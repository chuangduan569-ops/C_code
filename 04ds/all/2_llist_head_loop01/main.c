#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;
	LLIST data;//存储数据
	LLIST save;//取出的数据
	LLIST *find = NULL;//find指向要查找的数据
	int find_id = 102;
	char *find_name = "stu4";
	int i = 0;//循环
	int ret = 0;//接收返回值
	
	handler = llist_create();//创建头节点
	if(handler == NULL)
	{
		fprintf(stderr,"llist_create() Failed!\n");
		return -1;
	}

	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);	
		data.math = 50 + i;
		data.next = NULL;
		data.prev = NULL;
		llist_insert(handler, &data, HEADINSERT);
	}
	
	llist_display(handler);
	printf("---------------------------------\n");

	find = llist_find(handler,&find_id);
	if(find == NULL)
	printf("Not Find It!\n");
	else
	printf("Find It!\n	%d	%s	%d\n", find->id, find->name, find->math);
#if 0
	ret = llist_delete(handler,find_name);
	if(ret != 0)
	printf("Delete Failed!\n");
	else
	printf("Delete It!\n");
#else
	ret = llist_fetch(handler, &find_id, &save);
	if(ret != 0)
	printf("Fetch Failed!\n");
	else
	{
		printf("Fetch It!\n");
		printf("%d	%s	%d\n",save.id, save.name, save.math);
	}
	
#endif
	printf("---------------------------------\n");
	llist_display(handler);
	llist_destroy(handler);
	llist_display(handler);



	return 0;
}






