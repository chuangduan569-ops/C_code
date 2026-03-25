#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

#define NAMESIZE 20

struct llist_node
{
	struct llist_node *prev;
	struct llist_node *next;
	char data[0];
};

struct llist_head//头节点的结构
{
	int size;//存储客户指定的大小
	struct llist_node head;////头节点;是一个变量，而且是llist_node类型的头节点变量，里面有prev next data[0]
};


struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

int llist_save(LLIST *handler, const char *pathname)
{
	FILE *fp = NULL;
	fp = fopen(pathname, "w");

	if(fp == NULL)
	{
		perror("fopen()");
		return -1;
	}
	struct llist_head *h = handler;
	struct llist_node *cur = NULL;

	for(cur = h->head.next; cur != &h->head; cur = cur->next)
	{

	struct stu *p = (struct stu *)cur->data;
		fprintf(fp,"%d	%s	%d	%d\n",p->id, p->name, p->math, p->tel);

	}
	fclose(fp);
//	fp = NULL;
	return 0;
}



