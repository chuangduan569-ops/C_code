#include <stdio.h>
#include "llist.h"

int main(void)
{
	int i = 0;//循环变量
	LLIST data;//用于存储数据
	LLIST save;//save变量用来存储取出节点的数据
	LLIST* find = NULL;//find指向查找的数据节点
	int find_id = 102;//存储查找的id
	char *find_name = "stu3";//存储查找的NAME
	int	ret = 0;//用来接收返回值
	LLIST* handler = NULL;//handler指向头节点
	llist_create(&handler);//创建头节点
	if(handler==NULL)//判断头节点是否开辟成功
	{
		fprintf(stderr,"创建头节点失败\n");
		return -1;
	}
	for(i=0;i<5;i++)
	{
		data.id = 100+i;
		sprintf(data.name,"stu%d",i);
		data.math = 50+i;
		data.next = NULL;
		llist_insert(handler,&data,TAILINSERT);	
	}
	llist_display(handler);//遍历链表
	printf("------------------------\n");
	find = llist_find(handler,find_name);//查找链表
/*	if(find==NULL)
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
	printf("----------------------\n");
	llist_display(handler);//遍历链表
	llist_destroy(handler);//销毁链表
	return 0;
}
