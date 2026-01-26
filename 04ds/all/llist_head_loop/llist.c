#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"


int llist_create(LLIST **handler)
{
	*handler = malloc(sizeof(LLIST));
	if(*handler == NULL)
		return -1;
	
	(*handler)->next = *handler;

	return 0;
}

int llist_insert(LLIST *handler, const void *data, int mode)
{
	LLIST *p = handler;
	LLIST *newnode = NULL;
	newnode = malloc(sizeof(LLIST));
	if(newnode == NULL)
		return -1;
	
	memcpy(newnode, data, sizeof(LLIST));

	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : while(p != handler) p = p->next; break;
		default: free(newnode); return -2;
	}
	newnode->next = p->next;
	p->next = newnode;

	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;
	for(cur = handler->next; cur != handler; cur = cur->next)
		printf("%d	%s	%d\n",cur->id, cur->name, cur ->math);
}

void llist_destory(LLIST *handler)
{
	LLIST *back = handler;//前指针
	LLIST *cur = handler->next;//cur指向每一个数据节点
	int count = 0;
	
	while(cur != handler)
	{
		back->next = cur->next;
		printf("第%d destorying:		%d",++count, cur->id);
		free(cur);
		printf("	destroy over\n");
		cur = back->next;
	}
	printf(" %d destorying        \n",++count);
	free(cur);
	printf("over    destroy(all)\n");
	printf("共释放%d次，释放结束\n",count);
}


LLIST *_find(LLIST *handler, const void *find_data)
{
	LLIST *cur = NULL;

	for(cur = handler->next; cur != handler; cur = cur->next)
		if(cur->id == *(int *)find_data || !strcmp(cur->name,find_data))
			return cur;
	return NULL;
}

LLIST *llist_find(LLIST *handler, const void *find_data)
{
	return _find(handler,find_data);
}

int llist_delete(LLIST *handler, const void *find_data)
{
	LLIST *back = handler;
	LLIST *cur = NULL;
	
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;
	while(back->next != cur)
		back = back->next;//找到要删除节点的前一个的位置
	back->next = cur ->next;
	printf("正在删除%d...	",cur->id);
	free(cur);
	return 0;
}

int llist_fetch(LLIST *handler, const void *find_data, void *save)
{
	LLIST *back = handler;
	LLIST *cur = NULL;
	
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;

	while(back->next != cur)
		back = back->next;
	back->next = cur ->next;
	memcpy(save, cur, sizeof(LLIST));
	free(cur);

	return 0;
}











