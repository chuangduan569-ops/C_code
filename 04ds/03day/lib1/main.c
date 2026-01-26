#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

#define NAMESIZE 20

struct stu//客户数据的结构
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)
{
	const struct stu *p = data;//转换指针
	printf("%d	%s	%d\n",p->id,p->name,p->math,p->tel);
}

int id_cmp(const void *data,const void *key)//data是数据域的数据 key是要找的数据
{
	const struct stu *p = data;
	const int *k = key;

	return !(p->id - *k);
}	

int tel_cmp(const void *data,const void *key)
{
	const struct stu *p = data;
	const int *k = key;

	return !(p->tel - *k);
}

int name_cmp(const void *data,const void *key)
{
	const struct stu *p = data;
	const char *k = key;

	return !strcmp(p->name,k);
}

int main(void)
{
	LLIST *handler = NULL;//handler指向头节点
	struct stu data;//存储客户录入的数据
	struct stu save;//存储从链表中取出的数据
	struct stu *find = NULL;//指向查找到的数据
	int find_id = 100;
	char *find_name = "stu4";
	int find_tel = 10012;
	int i = 0;
	int ret = 0;
	
	handler = llist_create(sizeof(struct stu));//创建头节点
	if(handler == NULL)//判断创建头节点是否失败
	{
		fprintf(stderr,"Failed\n");
		return -1;//创建失败，并且返回-1；
	}

	for(i=0;i<5;i++)
	{
		data.id = 100+i;
		sprint(data.name,"stu%d",i);
		data.math = 50+i;
		data.tel = 10010+i;
		llist_insert(handler.&data,TAILINSERT);//插入数据
	}

	llist_display(handler,print);//遍历链表

	printf("-----------------------------\n");
/*
	find = llist_find(handler,&find_tel, tel_cmp);//查找节点
	if(find == NULL)
		printf("No Find It\n");
	else
	{
		printf("Find It\n");
		printf(find);
	}
*/
#if 0
	ret = llist_delete(handler,&find_tel,tel_cmp);//删除数据节点
	if(ret != 0)
		printf("Delete It Failed\n");
	else
		printf("Delete It\n");
#else
	ret = llist_fetch(handler,&find_tel,tel_cmp,&save);//取出数据节点
	if(ret != 0)
		printf("Fetch It Is Failed\n");
	else
	{
		printf("Fetch It \n");
		printf(&save);
	}
#endif

	printf("--------------------\n");
	
	llist_display(handler);
	
	llist_destroy(handler);
	
	return 0;
}







