#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"


LLIST *llist_insert(LLIST *l, const void *data, int mode)
{
	LLIST *newnode = NULL;//指针指向新开辟的空间
	LLIST *p = l;//p指针代替l做操作
	//判断是不是空链表
	if(l == NULL)
	{
		l = malloc(sizeof(LLIST));//开辟第一个数据节点
		if(l == NULL)
			return NULL;//开辟失败，结束函数并且返回NULL
		memcpy(l,data,sizeof(LLIST));//拷贝数据
		return l;//返回开辟成功的第一个数据节点
	}
	//说明当前链表不是空链表
	newnode = malloc(sizeof(LLIST));//开辟新的数据节点
	if(newnode == NULL)
		return l;//开辟新的数据节点失败，结束函数，并且返回第一个数据节点
	memcpy(newnode,data,sizeof(LLIST));//拷贝数据
	
	switch(mode)
	{
		case HEADINSERT : newnode->next = l;l = newnode;break;
		case TAILINSERT : while(p->next != NULL) p = p->next; p->next = newnode; break;
		default : free(newnode); return l;
		//由于插入模式失败，释放新的节点，并且返回第一个节点的地址
	}
	return l;
}

void llist_display(LLIST *l)
{
	LLIST *cur = NULL;
	for(cur = l; cur != NULL; cur = cur ->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

void llist_destroy(LLIST *l)
{
	LLIST *cur = l;//cur指向要销毁的链表
	LLIST *next = l->next;//next指向要删除的下一个节点
	while(next != NULL)
	{
		free(cur);
		cur = next;
		next = next->next;
	}
	free(cur);
}

LLIST *_find(LLIST *l, const void *find_data)
{
	LLIST *cur = NULL;
	for(cur = l; cur != NULL; cur = cur->next)
		if(cur->id == *(int *)find_data || !strcmp(cur->name,find_data))
			return cur;
	return NULL;
		
}

LLIST *llist_find(LLIST *l, const void *find_data)
{
	return _find(l,find_data);
}

int llist_delete(LLIST **l, const void *find_data)
{
	LLIST *back = *l;//前指针
	LLIST *cur = NULL;//cur指向要删除的节点

	cur = _find(*l,find_data);
	if(cur == NULL)
		return -1;
	if(cur == *l)
	{
		*l =  (*l)->next;
		free(cur);
	}
	else
	{
		while(back->next != cur)
			back = back->next;
		back->next = cur->next;
		free(cur);
	}
	return 0;
}

int llist_fetch(LLIST **l, const void *find_data, void *save)
{
	LLIST *back = *l;//前指针
	LLIST *cur = NULL;

	cur = _find(*l,find_data);
	if(cur == NULL)//判断没有找到要取出的节点，并返回-1
		return -1;
	
	if(cur == *l)
		*l = (*l)->next;//判断要取出的是否是第一个数据节点，是的话指向第二个数据节点
	else
	{
		while(back->next != cur)
			back = back->next;//循环找到要取出的数据节点
		back->next = cur->next;
	}
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	return 0;
}











