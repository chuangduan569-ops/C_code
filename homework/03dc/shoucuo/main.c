#include <stdio.h>
#include "llist.h"

int main(void)
{
	int i = 0;//循环变量
	int ret = 0;//接收返回值
	LLIST data;//存储数据
	LLIST save;//用来存储取出的数据
	LLIST *find = NULL;//定义来查找的指针
	int find_id = 102;
	char *find_name = "stu3";
	LLIST *handler = NULL;//定义头指针
	
	handler = llist_create();//创建头节点
	if(handler == NULL)
	{	
		fprintf(stderr,"Failed\n");
		return -1;
	}
	for(i=0;i<5;i++)
	{
		data.id =  100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.next = NULL;
		llist_insert(handler,&data,HEADINSERT);
	}

/*
 	find = llist_find(handler,&find_id);
 	if(find == NULL)
		printf("没有找到\n");
	else
		printf("找到了!%d	%s	%d",find->id,find->name,find->math);
 */
#if 0
	ret = llist_delete(handler,find_name);
	if(ret != 0)
		printf("删除失败\n");
	else
		printf("删除成功!\n");

#else
	ret = llist_fetch(handler,find_name,&save);
	if(ret != 0)
		printf("取出失败\n");	
	else
		printf("成功了!%d	%s	%d\n",save.id,save.name,save.math);
#endif

	printf("--------------------\n");
	llist_display(handler);
	//printf("===================\n");
	
	
	llist_destroy(handler);
	return 0;
}
