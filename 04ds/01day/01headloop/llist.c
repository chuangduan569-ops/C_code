#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"
int llist_create(LLIST** handler)
{
	*handler = malloc(sizeof(LLIST));
	if(*handler == NULL)
	return -1;
	(*handler)->next = *handler;//首尾相连
	return 0;
}
int llist_insert(LLIST* handler,const void* data,int mode)
{
	LLIST* p = handler;
	LLIST* newnode = NULL;
	newnode = malloc(sizeof(LLIST));
	if(newnode==NULL)
		return -1;
	memcpy(newnode,data,sizeof(LLIST));
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT :while(p->next != handler) p=p->next;break;
		default:free(newnode);return -2;
	}
	newnode->next = p->next;
	p->next = newnode;

	return 0;
}
void llist_display(LLIST* handler)
{
	LLIST* cur = NULL;
	for(cur = handler->next;cur!=handler;cur=cur->next)
	printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

void llist_destroy(LLIST* handler)
{
	LLIST* back = handler;
	LLIST* cur = handler->next;//指针指向每一个数据节点
	while(cur!=handler)//循环释放每一个数据节点
	{
	back->next = cur->next;
	free(cur);
	cur = back->next;
	}
	free(back);
}
static LLIST* _find(LLIST* handler,const void* find_data)
{
	LLIST* cur = NULL;
	for(cur = handler->next;cur!=handler;cur=cur->next)
		if(cur->id ==*(int *)find_data || !strcmp(cur->name,find_data))
			return cur;
	return NULL;
}

LLIST* llist_find(LLIST* handler,const void* find_data)
{
	return _find(handler,find_data);
}

int llist_delete(LLIST* handler,const void* find_data)
{
	LLIST* back = handler;
	LLIST* cur = NULL;
	cur = _find(handler,find_data);
	if(cur==NULL)
	return -1;
	while(back->next!=cur)
	back = back->next;
	back->next = cur->next;
	free(cur);
	return 0;
}

int llist_fetch(LLIST* handler,const void* find_data,void* save)
{
	LLIST* back = handler;
	LLIST* cur = NULL;
	cur = _find(handler,find_data);
	if(cur == NULL)
	return -1;
	while(back->next!=cur)
	back = back->next;

	back->next=cur->next;
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	return 0;
}


