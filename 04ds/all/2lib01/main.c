#include <stdio.h>
#include <string.h>
#include "llist.h"

#define NAMESIZE 20

struct stu//客户数据结构
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)
{
	const struct stu *p = data;//转换指针
	printf("%d	%s	%d	%d\n", p->id, p->name, p->math, p->tel);
}

int id_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	
	return !(p->tel - *k);
}


int name_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const char *k = key;

	return !strcmp(p->name,k);
}


int main(void)
{
	LLIST *handler = NULL;//handler指向头节点
	struct stu data;//存储客户录入的数据
	struct stu save;//存储客户取出的数据
	struct stu *find = NULL;//find指针指向要查找的数据
	int find_id = 101;
	char *find_name = "stu3";
	int find_tel = 10012;
	int i = 0;
	int ret = 0;

	handler = llist_create(sizeof(struct stu));//创建头节点
	if(handler == NULL)
	{
		fprintf(stderr,"llist_create() Failed!\n");
		return -1;
	}
	
	for(i = 0; i<5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.tel = 10010 + i;
		llist_insert(handler, &data, TAILINSERT);
	}

	llist_display(handler,print);

	printf("---------------------\n");

/*
	find = llist_find(handler, &find_tel, tel_cmp);
	if(find == NULL)
		printf("Not Find It!\n");
	else
	{
		printf("Find It!\n");
		print(find);
	}
*/

#if 0
	ret = llist_delete(handler,&find_id,id_cmp);
	if(ret != 0)
		printf("Delete Failed!\n");
	else
		printf("Delete It!\n");

#else
	ret = llist_fetch(handler, &find_tel, tel_cmp, &save);
	if(ret != 0)
		printf("Fetch Failed!\n");
	else
		{
			printf("Fetch It!\n");
			print(&save);
		}

#endif

	printf("-----------------------\n");

	llist_display(handler,print);

	llist_destory(handler);//销毁链表

	return 0;
}




