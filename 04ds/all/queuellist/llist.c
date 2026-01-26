#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"


LLIST *llist_create(void)
{
	LLIST *handler = NULL;
	
	handler = malloc(sizeof(LLIST));
	if(handler == NULL)
		return NULL;
	
	handler->prev = handler->next = handler;
	return handler;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *p = handler;//p指针代替handler做操作
	LLIST *newnode = NULL;//指向新开辟的空间
	
	newnode = malloc(sizeof(LLIST));
	if(newnode == NULL)
	return -1;

	memcpy(newnode,data,sizeof(LLIST));//拷贝数据
	
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

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;
	for(cur = handler->next; cur != handler; cur = cur->next)
		printf("	%d\n",cur->id);
}

void llist_destory(LLIST *handler)
{
	LLIST *cur = NULL;
	int count = 0;
	for(cur = handler->next; cur != handler; cur = cur->next)
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
		printf("正在销毁	%d\n",cur->id);
		free(cur);
		count++;
	}
	free(cur);
	printf("正在销毁头节点\n");
	printf("共销毁了%d次,销毁完毕!\n",count);
}

LLIST *_find(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;//cur指向要找到的数据
	for(cur = handler->next; cur != handler; cur = cur->next)
		if(cur->id == *(int *)find_data)
			return cur;
	return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
	return _find(handler,find_data);
}

int llist_delete(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;
	
	cur = _find(handler, find_data);
	if(cur == NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	free(cur);
	return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
	LLIST *cur = NULL;
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	return 0;
}	


int llist_is_empty(LLIST *handler)
{
	return (handler->next == handler);
}

int llist_pop(LLIST *handler, void *save)
{
	LLIST *cur = handler->next;
	
	if(llist_is_empty(handler))
		return -1;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	memcpy(save,&cur->id,sizeof(cur->id));
	free(cur);
	return 0;
}





