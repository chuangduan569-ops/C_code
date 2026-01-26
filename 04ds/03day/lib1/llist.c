#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST *llist_create(int size)
{
	LLIST *handler = NULL;//handler指针指向开辟的头节点
	handler = malloc(sizeof(LLIST));//开辟头节点空间
	if(handler == NULL)
		return NULL;//开辟空间失败，结束函数，并且返回NULL;
	handler->size = size;//把客户数据的大小存储到头节点中
	handler->head.prev = &handler->head;//让头节点的prev指针指向头节点中的head
	handler->head.next = &handler->head;//让头节点的next指针指向头节点中的head
	return handler;//把开辟的头节点的首地址返回
}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	struct llist_node *p = &handler->head;//p指针代替头节点中的head做操作
	struct llist_node *newnode = NULL;//newnode指针指向新开辟的数据节点
	newnode = malloc(sizeof(struct llist_node));//开辟数据节点的空间
	if(newnode == NULL)
		return -1;//开辟数据节点失败，结束函数，返回-1
	newnode->data = malloc(handler->size);//开辟数据域的空间
	if(newnode->data == NULL)
	{
		free(newnode);
		return -2;//开辟数据域的空间失败，释放开辟的数据节点空间，并且返回-2
	}
	memcpy(newnode->data,data,handler->size);//把客户的数据拷贝到数据域

	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev; break;
		default : free(newnode->data);free(newnode);return -3;
		//由于插入失败，释放数据域，释放数据节点，结束函数，并且返回-3；
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;

	return 0;
}

void llist_display(LLIST *handler,llist_op op)
{
	struct llist_node *cur = NULL;//cur指向每一个数据节点
	for(cur = handler->head.next;cur!=handler->head;cur = cur->next)
		op(cur->data);
}

void llist_destroy(LLIST *handler);

void *llist_find(LLIST *handler;const void *find_data,llist_cmp cmp);

int llist_delete(LLIST *handler,const void *find_data,llist_cmp cmp);

int llist_fetch(LLIST *handler,const void *find_data,llist_cmp cmp,void *save);









