#include<stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;//handler指针指向头节点
	LLIST data;
	LLIST save;//save用来u存储取出节点的数据
	LLIST *find = NULL;//find指针指向查找的数据节点
	int find_id = 103;//存储要查找的ID
	char *find_name = "stu2";//存储要查找的NAME
	int ret = 0;//用来接收返回值
	int i = 0;
	llist_create(&handler);//创建头节点
	if(handler == NULL)
	{
		fprintf(stderr,"创建头节点失败\n");
		return -1;//返回值-1
	}
	for(i=0;i<5;i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 100 -i ;
		data.next = NULL;
		llist_insert(handler,&data,TAILINSERT);
	}
	llist_display(handler);//遍历链表
	printf("---------------------------------------\n");
/*	find = llist_find(handler,find_name);//查找链表
	if(find==NULL)
		printf("没有找到\n");
	else
		printf("找到了!%d	%s	%d\n",find->id,find->name,find->math);
*/
#if 0
	ret = llist_delete(handler,find_name);//删除节点
	if(ret != 0)
		printf("删除失败\n");
	else
		printf("删除成功\n");
#else
	ret = llist_fetch(handler,&find_id,&save);//取出数据节点
	if(ret != 0)
		printf("取出失败\n");
	else
		printf("成功了!%d	%s	%d\n",save.id,save.name,save.math);
#endif
	llist_display(handler);
	llist_destroy(handler);//销毁链表
	printf("===========\n");
	llist_display(handler);
	return 0;
}
