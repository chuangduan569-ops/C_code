#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

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

LLIST *llist_create(int size)
{
	struct llist_head *handler = NULL;
	
	handler = malloc(sizeof(struct llist_head));//开辟头节点空间
	if(handler == NULL)
		return NULL;
	handler->size = size;
	handler->head.prev = handler->head.next = &handler->head;

	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	struct llist_head *h = handler;//转换指针
	struct llist_node *p = &h->head;//p代替头指针做操作
	struct llist_node *newnode = NULL;

	newnode = malloc(sizeof(struct llist_node) + h->size);
	if(newnode == NULL)
		return -1;

	memcpy(newnode->data, data, h->size);

	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev; break;
		default : free(newnode); return -2;
	}
		
	newnode->next = p->next;
	newnode->prev = p;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	return 0;
}
void llist_display(LLIST *handler, llist_out out)
{
	struct llist_head *h = handler;
	struct llist_node *cur = NULL;
	for(cur = h->head.next; cur != &h->head; cur = cur->next)
		out(cur->data);
}

void llist_destory(LLIST *handler)
{
	struct llist_head *h = handler;
	struct llist_node *cur = h->head.next;

	while(cur != &h->head)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);
		cur = h->head.next;
	}
	free(handler);
}

static struct llist_node *_find(LLIST *handler,const void *find_data, llist_cmp cmp)
{	
	struct llist_head *h = handler; 
	struct llist_node *cur = NULL;

	for(cur = h->head.next; cur != NULL; cur = cur->next)
		if(cmp(cur->data, find_data))
			return cur;
	return NULL;

}

void *llist_find(LLIST *handler,const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = NULL;
	cur = _find(handler,find_data,cmp);
	if(cur != NULL)
		return cur->data;
	else
		return NULL;
	
}

int llist_delete(LLIST *handler,const void *find_data, llist_cmp cmp)
{
	struct llist_node *cur = NULL;
	
	cur = _find(handler,find_data,cmp);
	if(cur == NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);
	
	return 0;
}

int llist_fetch(LLIST *handler,const void *find_data, llist_cmp cmp, void *save)
{
	struct llist_head *h = handler;//转换指针,size使用
	struct llist_node *cur = NULL;
	cur = _find(handler,find_data,cmp);
	if(cur == NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save, cur->data, h->size);
	free(cur);

	return 0;
}






















