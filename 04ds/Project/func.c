#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"func.h"
//链表地层实现
LLIST *LLIST_head_create(int size)
{
	LLIST *handler=NULL;//创建头节点
	handler=malloc(sizeof(LLIST));//开辟头节点的存储空间
	if(handler==NULL)
	{	
		return NULL;
	}
	handler->size=size;//客户空间的大小
	handler->head.next=&handler->head;
	handler->head.prev=&handler->head;//定义了双向循环链表
//	handler->head.data=NULL;
	return handler;//返回头节点地址； 
}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	struct llist_node *newnode = NULL;//指向新开辟的空间
	struct llist_node *p = &handler->head;//代替头指针做操作
	
	newnode = malloc(sizeof(struct llist_node) + handler->size);//开辟空间
	if(newnode == NULL)
		return -1;

	memcpy(newnode->data,data,handler->size);//拷贝数据
	
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT :p = p->prev; break;
		default: free(newnode); return -2;
		//插入模式失败，结束函数，返回-2
	}
	newnode->next = p->next;
	newnode->prev = p;//把新节点挂在链表上
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	return 0;
}

static struct llist_node *_find(LLIST *handler, const void *data)
{
	struct llist_node *cur = NULL;//cur指向要查找的数据
	for(cur = handler->head.next; cur != &handler->head; cur = cur->next)//遍历链表
	//	if(cur->data.id == *(int *)data || !strcmp(cur->data.name,data))//通过id和name查找要查找的数据
		if(memcmp(cur->data, data, handler->size) == 0)//直接比较整个内存块
		return cur;//返回查找到的链表
	return NULL;
}

void *llist_find(LLIST *handler, const void *data)
{
	struct llist_node *cur = NULL;
	cur = _find(handler,data);
	if(cur != NULL)
		return cur->data;//返回查找的数据
	return NULL;
}

int llist_revise(LLIST *handler,const void *old_data,const void *new_data)
{
	struct llist_node *cur = NULL;

	cur = _find(handler,old_data);
	if(cur == NULL)
		return -1;
	
	memcpy(cur->data,new_data,handler->size);
	return 0;
	
}


int llist_delete(LLIST *handler, const void *data)
{
	struct llist_node *cur = NULL;
	cur = _find(handler,data);//指向要删除的数据
	if(cur == NULL)
		return -1;//没有找到返回-1
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);//删除节点
	return 0;
}

void llist_destroy(LLIST *handler)
{
	struct llist_node *cur = NULL;//指向要销毁的数据
	for(cur = handler->head.next; cur != &handler->head; cur = handler->head.next)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		free(cur);//销毁数据节点
	}
	free(handler);//销毁头节点
}






