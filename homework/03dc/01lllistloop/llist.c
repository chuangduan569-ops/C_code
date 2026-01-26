#include<stdio.h>
#include<stdlib.h>
#include "llist.h"
#include<string.h>

int llist_create(LLIST **handler)
{
	*handler = malloc(sizeof(LLIST));
	if(*handler == NULL)
		return -1;	
	(*handler)->next = *handler;//首尾相连
	return 0;
}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	LLIST *p = handler;
	LLIST *newnode = NULL;
	
	newnode = malloc(sizeof(LLIST));
	if(newnode == NULL)
		return -1;
	memcpy(newnode,data,sizeof(LLIST));
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : while(p->next != handler) p=p->next; break;
		default: free(newnode);return -2;
		//由于插入模式失败，释放新开辟的空间，返回-2
	}
	newnode->next = p->next;
	p->next = newnode;

	return 0;
}
void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;//cur指针指向每一个数据节点
	for(cur = handler->next;cur != handler;cur = cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

void llist_destroy(LLIST *handler)
{
	LLIST *back = handler;//前指针
	LLIST *cur = handler->next;//cur指向每一个数据节点
	while(cur!=handler)//循环每一个数据节点
	{
		back->next = cur->next;//把数据节点从链表中摘除
		free(cur);//释放当前数据节点
		cur = back->next;
	}
	free(cur);//释放头节点
}
static LLIST *_find(LLIST *handler,const void *find_data)
{
	LLIST *cur = NULL;//cur指向每一个数据节点
	for(cur = handler->next;cur!=handler;cur = cur->next)//遍历每一个数据节点
	if(cur->id == *(int *)find_data || !strcmp(cur->name,find_data))
		return cur;
	return NULL;
}
LLIST *llist_find(LLIST *handler,const void *find_data)
{
    return _find(handler,find_data);
}


int llist_delete(LLIST *handler,const void *find_data)
{
	LLIST *back = handler;//前指针
	LLIST *cur = NULL;//cur指向要删除的节点
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;//没有找到要删除的节点
	while(back->next!=cur)//找到要删除前的前一个节点
		back = back->next;
	back->next = cur->next;//把要删除的节点从链表中释放	
	free(cur);//释放要删除的节点
	return 0;
}

int llist_fetch(LLIST *handler,const void *find_data,void *save)
{
	LLIST *back = handler;//前指针
	LLIST *cur = NULL;//cur指向要取出的指针
	cur = _find(handler,find_data);//查找要取出的节点
	if(cur == NULL)
		return -1;//没有找到要取出的节点，返回-1
	while(back->next != cur)//找到要取出节点的前一个位置
	back = back->next;
	back->next = cur->next;//把要取出的节点从链表中删除；
	memcpy(save,cur,sizeof(LLIST));
	free(cur);//释放删除的节点

	return 0;
}














