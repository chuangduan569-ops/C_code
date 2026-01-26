#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "llist.h"

LLIST * llist_create(void)
{
	LLIST *handler = NULL;
	handler = malloc(sizeof(LLIST));
	if(handler == NULL)
		return NULL;
	handler->next = handler->prev = handler;
	return handler;
}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	LLIST *p = handler;//代替指针p作操作
	LLIST *newnode = NULL;//开辟新的空间节点
	newnode = malloc(sizeof(LLIST));
	if(newnode == NULL)
		return -1;
	memcpy(newnode,data,sizeof(LLIST));
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : p = p->prev;break;
		default:free(newnode); return -2;
	}
	newnode->next = p->next;
	newnode->prev = p;
	newnode->next->prev = newnode;
	newnode->prev->next = newnode;

	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;
	for(cur = handler->next;cur != handler;cur=cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

void llist_destroy(LLIST *handler)
{
	LLIST *cur = NULL;//cur指向要释放的节点
	for(cur = handler->next;cur != handler;cur = handler->next)
	{
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
	}
	free(cur);
}


static LLIST *_find(LLIST *handler,const void *find_data)
{
	LLIST *cur = NULL;//指向要查找的数据节点
	for(cur = handler->next;cur!=handler;cur = cur->next)
		 if(cur->id == *(int *)(find_data) || !strcmp(cur->name, find_data))
            return cur;
    return NULL;
}

LLIST *llist_find(LLIST *handler,const void *find_data)
{
	return _find(handler,find_data);
}

int llist_delete(LLIST *handler,const void *find_data)
{
	LLIST *cur = NULL;//cur指针指向要删除的节点
	cur = _find(handler,find_data);
	if(cur == NULL)
	return -1;
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	free(cur);
	return 0;
}

int llist_fetch(LLIST *handler,const void *find_data,void *save)
{
	LLIST *cur = NULL;
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	memcpy(save,cur,sizeof(LLIST));
	free(cur);

	return 0;


}









