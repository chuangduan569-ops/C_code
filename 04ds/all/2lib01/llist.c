#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST *llist_create(int size)
{
	LLIST *handler = NULL;
	handler = malloc(sizeof(LLIST));
	if(handler == NULL)
		return NULL;
	handler->size = size;//存储用户指定的数据域大小
	handler->head.prev = handler->head.next = &handler->head;//头节点下的head是变量
	//头节点首尾指向自己
	return handler;//返回头节点的首地址
}

int llist_insert(LLIST *handler,const void *data, int mode)
{
	struct llist_node *p = &handler->head;//p指针代替头节点做操作
	struct llist_node *newnode = NULL;

	newnode = malloc(sizeof(struct llist_node) + handler->size);
	if(newnode == NULL)
		return -1;
	
	memcpy(newnode->data,data,handler->size);//拷贝数据
	
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev ;break;
		default : free(newnode); return -2;
	}
		newnode->next = p->next;
		newnode->prev = p;//把newnode挂在链表上
		newnode->prev->next = newnode;
		newnode->next->prev = newnode;
		
		return 0;
}

void llist_display(LLIST *handler,llist_op op)
{
	struct llist_node *cur = NULL;
	for(cur = handler->head.next; cur != &handler->head; cur = cur->next)
		op(cur->data);
}

void llist_destory(LLIST *handler)
{
	struct llist_node *cur = handler->head.next;//cur指向每一个数据节点
		
	while(cur != &handler->head)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);
		cur = handler->head.next;
	}
		free(handler);
}

static struct llist_node *_find(LLIST *handler,const void *find_data,llist_cmp cmp)
{
	struct llist_node *cur = NULL;
	for(cur = handler->head.next; cur != &handler->head; cur = cur->next)
		if(cmp(cur->data, find_data))//判断是否找到数据节点
			return cur;
	return NULL;
}

void *llist_find(LLIST *handler,const void *find_data,llist_cmp cmp)
{
	struct llist_node *cur = NULL;
		
	cur = _find(handler,find_data,cmp);
	if(cur != NULL)
		return cur->data;//需要返回的是data
	return NULL;
}

int llist_delete(LLIST *handler,const void *find_data,llist_cmp cmp)
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

int llist_fetch(LLIST *handler,const void *find_data,llist_cmp cmp,void *save)
{
	struct llist_node *cur = NULL;

	cur = _find(handler,find_data,cmp);
	if(cur == NULL)
		return -1;

	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save,cur->data,handler->size);
	free(cur);

	return 0;
}









