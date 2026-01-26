#include<stdio.h>
#include "llist.h"

int main(void)
{	
	
	LLIST data;//接收数据
	int i = 0;
	LLIST *handler = NULL;
	int find_id = 103;//查找的id号
	char *find_name = "stu1";//存储查找的name号
	LLIST *find = NULL;//find指向要查找的节点
	int ret = 0;//用来接收返回值
	LLIST save;//用来存储取出来的数据
	handler = llist_create();
	if(handler == NULL)//判断头节点是否创建成功
	{
		fprintf(stderr,"创建失败\n");
		return -1;//开辟头节点失败，返回-1
	}
	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.next = NULL;
		llist_insert(handler,&data,TAILINSERT);//增。
	}
	llist_display(handler);
	
/*	find = llist_find(handler,&find_id);
	if(find == NULL)
		printf("没有找到\n");
	else
		printf("找到了!%d	%s	%d\n",find->id,find->name,find->math);
*/
#if 0 
	ret = llist_delete(handler,&find_id);
	if(ret != 0)
		printf("删除失败\n");
	else
		printf("删除成功\n");
#else
	ret = llist_fetch(handler,&find_id,&save);
	if(ret != 0)
		printf("没有找到要取出的数据\n");
	else
	printf("取出的数据是:%d	%s	%d\n",save.id,save.name,save.math);
#endif
printf("-------------------------------------------------\n");
	llist_display(handler);
printf("===========================\n");
	llist_destroy(handler);
	llist_display(handler);


	return 0;
}

