#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "llist.h"

struct llist_node//数据节点结构
{
	struct llist_node *prev;
	struct llist_node *next;
	char data[0];//用来标记数据域的首地址,
};

struct llist_head//头节点结构
{
	int size;
	struct llist_node head;//表示头节点
};

LLIST *llist_create(int size)
{
	struct llist_head *handler = NULL;//handler指针指向开辟的头节点
	handler = malloc(sizeof(struct llist_head));//开辟头节点的空间
	if(handler == NULL)
		return NULL;
	handler->size = size;//存储用户指定数据的大小
	handler->head.prev = handler->head.next = &handler->head;//头节点的首位指向自己
	
	return handler;//返回头节点首地址
}

int llist_insert(LLIST *handler,const void *data, int mode)
{
	struct llist_head *h = handler;//转换指针
	struct llist_node *p = &h->head;//p指针代替头节点做操作
	struct llist_node *newnode = NULL;//newnode指针指向新开辟的数据节点

	newnode = malloc(sizeof(struct llist_node) + h->size);//开辟数据节点的空间*h = handler(h->size)
	if(newnode == NULL)
		return -1;
	memcpy(newnode->data,data,h->size);//拷贝数据

	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT :p = p->prev;break;
		default:free(newnode);return -2;
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->prev->next = newnode;
	newnode->next->prev = newnode;
	return 0;
}

void llist_display(LLIST *handler,llist_op op)
{
	struct llist_head *h = handler;//转换指针
	struct llist_node *cur = NULL;//cur指向每一个人数据节点
	for(cur = h->head.next;cur!=&h->head;cur = cur->next)
		op(cur->data);
}

void llist_destroy(LLIST *handler)
{
	struct llist_head *h = handler;//转换指针
	struct llist_node *cur = h->head.next;//cur指针指向每一个数据节点
	while(cur != &h->head)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);
		cur = h->head.next;
	}
	free(h);//释放头节点
}
static struct llist_node *_find(LLIST *handler,const void *find_data,llist_cmp cmp)
{
	struct llist_head *h = handler;//转换指针
	struct llist_node *cur = NULL;//cur指针指向每一个数据节点
	for(cur = h->head.next;cur != &h->head; cur = cur->next)
		if(cmp(cur->data,find_data))
		return cur;
	return NULL;
}

void *llist_find(LLIST *handler,const void *find_data,llist_cmp cmp)
{
	struct llist_node *cur = NULL;
	cur = _find(handler,find_data,cmp);
	if(cur != NULL)
		return cur->data;
	else
		return NULL;

}

int llist_delete(LLIST *handler,const void *find_data,llist_cmp cmp)
{
	struct llist_node *cur = NULL;//cur指向要删除的数据节点
	cur = _find(handler,find_data,cmp);//查找要删除的数据节点
	if(cur==NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);
	return 0;
}

int llist_fetch(LLIST *handler,const void *find_data,llist_cmp cmp,void *save)
{
	struct llist_head *h =handler;//转换指针
	struct llist_node *cur = NULL;//指针指向要取出的数的节点

	cur = _find(handler,find_data,cmp);//查找要取出的数据
	if(cur==NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save,cur->data,h->size);//拷贝数据，数据大小为头节点的疏浚大小
	free(cur);
	return 0;

}







