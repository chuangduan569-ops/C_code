#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;//handler指向头节点
	LLIST data;//data变量存储客户录入的信息
	int i = 0;
	LLIST *find = NULL;
	int find_id = 102;
	char *find_name = "stu3";
	int ret = 0;
	LLIST save;
	handler = llist_create();//创建头节点
	if(handler == NULL)
	{
		fprintf(stderr,"llist_create() Failed\n");
		return -1;
	}
	
	for(i = 0; i< 5; i++)
	{
		data.id = 100+i;
		sprintf(data.name,"stu%d",i);	
		data.math = 50+i;
		data.prev = data.next =NULL;//让指针指向NULL
		llist_insert(handler,&data,HEADINSERT);

	}

	llist_display(handler);//遍历
/*	find = llist_find(handler,&find_id);
	if(find == NULL)
		printf("Failed\n");
	else
		printf("success %d	%s	%d\n",find->id,find->name,find->math);
*/
#if 0
	ret = llist_delete(handler,find_name);
	if(ret != 0)
		printf("Failed\n");
	else
		printf("success\n");
#else
	ret = llist_fetch(handler,find_name,&save);
	if(ret != 0)
		printf("Failed\n");
	else
		printf("success%d	%s	%d\n",save.id,save.name,save.math);
#endif
	llist_display(handler);	

	llist_destroy(handler);//销毁



	return 0;
}
