#include<stdio.h>
#include "llist.h"
int main(void)
{
	LLIST *handler = NULL;//handler指针指向LLIST的头节点
	LLIST data;//存储录入的数据
	LLIST save;//save变量存储取出节点的数据
	LLIST *find = NULL;//fine指向要查找的节点
	int find_id = 102;//存储要查找的id号（是变量）
	char *find_name = "stu1";//存储要查找的name（是变量）
	int i = 0;
	int ret = 0;//接收返回值
	handler = llist_create();//创建头节点
	if(handler == NULL)//判断头节点是否成功
	{
		fprintf(stderr,"头节点创建失败\n");
		return -1;
	}
	for(i=0;i<5;i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);//打印name的值
		data.math = 100 - i;
		data.next = NULL;//指针指向NULL
		llist_insert(handler,&data,TAILINSERT);//用指针去获取struct（LLIST）的数据，所以要取地址,增（插入）
	}
	llist_display(handler);//遍历
//	llist_destory(handler);//销毁

/*	find = llist_find(handler,&find_id);//查找数据节点
	if(find == NULL)//判断是否找到要查的节点
		printf("没有找到\n");
	else
		printf("找到了！%d	%s	%d\n",find->id,find->name,find->math);
*/	
#if 0
	ret = llist_delete(handler,&find_id);
	if(ret != 0)
		printf("删除失败\n");
	else
		printf("删除成功\n");
#else
	ret = llist_fetch(handler,&find_id,&save);//取出节点数据
	if(ret !=0)//判断取出节点的数据是否失败
	printf("取出失败\n");
	else
	printf("成功了！%d	%s	%d\n",save.id,save.name,save.math);
#endif
	printf("----------------------------------------------------\n");
	llist_display(handler);
	return 0;
}
