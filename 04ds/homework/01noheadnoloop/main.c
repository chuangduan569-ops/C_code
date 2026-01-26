#include <stdio.h>
#include <stdlib.h>
#include "llist.h"
int main(void)
{
	LLIST *l = NULL;//l指针指向链表的第一个数据节点
	LLIST *data = NULL;//动态数组
	LLIST *find = NULL;//指向找到的节点
	LLIST save;//存储取出的数据
	int find_id = 101;
	char *find_name = "stu4";//要查找的数据
	int i = 0;//循环变量
	int num = 0;//存储客户需要的节点数
	int ret = 0;//存储函数的返回值
	printf("请输入要录入的节点个数\n");
	scanf("%d",&num);

	data = malloc(sizeof(LLIST) * num);
	if(data == NULL)
	{
		fprintf(stderr,"开辟空间失败\n");
		return -1;
	}
	for(i=0;i<num;i++)//录入数据
	{
		data[i].id = 100+i;
		sprintf(data[i].name,"stu%d",i);
		data[i].math = 50+i;
		data[i].next = NULL;
	}
	
	l = llist_insert(data,num);//创建无头链表
	if(l == NULL)
	{
		fprintf(stderr,"ERRor\n");
		return -1;
	}
	llist_display(l);//遍历链表
		
	printf("======================================\n");
	/*
	find = llist_find(l,find_name);//查找数据节点
	if(find == NULL)
		printf("查找失败\n");
	else
		printf("找到了!%d	%s	%d\n",find->id,find->name,find->math);
	*/	
#if 0 
	ret = llist_delete(&l,&find_id);//删除数据节点
	if(ret != 0)
		printf("Delete It Is Failed!\n");
	else
		printf("Delete It!");

#else
	ret = llist_fetch(&l,find_name,&save);
	if(ret != 0)
		printf("Fetch It Is Failed!\n");
	else
	printf("Fetch It!%d	%s	%d\n",save.id,save.name,save.math);

#endif

	printf("========================\n");
	
	llist_display(l);//遍历链表
	
	llist_destroy(l);//销毁链表





	
	return 0;
}
