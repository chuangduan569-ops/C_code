#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST *llist_create(void)
{
	LLIST *handler = NULL;//接收malloc的返回值
	handler = malloc(sizeof(LLIST));
	if(handler == NULL)
		return NULL;
	handler->next = NULL;
	return handler;//把头节点返回地址
}

int llist_insert(LLIST *handler,const void *data,int mode)
{
	LLIST *p = handler;//指针p代替handler指针作操作
	LLIST *newnode = NULL;
	newnode = malloc(sizeof(LLIST));
	if(newnode == NULL)
		return -1;
	memcpy(newnode,data,sizeof(LLIST));
	switch(mode)
	{
		case HEADINSERT : break;
		case TAILINSERT : while(p->next != NULL) p = p->next; break;
		default : free(newnode);return -2;
	}
	newnode->next = p->next;
	p->next = newnode;
	
	return 0;
}

void llist_display(LLIST *handler)
{
	LLIST *cur = NULL;//cur指向每一个数据节点
	for(cur = handler->next;cur!=NULL;cur = cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);

}

 void llist_destroy(LLIST *handler)
{
	LLIST *back = handler;//前继节点
	LLIST *cur = handler->next;//当前要释放的节点
	
	while(cur!=NULL)//循环释放所有的节点
	{
		back->next = cur->next;
		free(cur);
		cur = back->next;
	}
	free(back);//释放头节点
}

static LLIST *_find(LLIST *handler,const void *find_data)
{
	LLIST *cur = NULL;
	for(cur = handler->next;cur!=NULL;cur = cur->next)
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
	LLIST *cur = NULL;//指向要删除的节点
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;
	while(back->next!=cur)//找到删除节点前的前一个节点
		back = back->next;
	back->next = cur->next;//把要删除的节点从链表中摘除
	free(cur);
	
	return 0;
}

int llist_fetch(LLIST *handler,const void *find_data,void *save)
{
	LLIST *back = handler;//前指针
	LLIST *cur = NULL;//指向要取出的数据节点
	cur = _find(handler,find_data);
	if(cur == NULL)
		return -1;
	while(back->next!=cur)
		back = back->next;
	back->next = cur->next;
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	
	return 0;


}





