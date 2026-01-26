#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST *llist_insert(const void *data,int num)
{
	LLIST *l = NULL;//l指针指向第一个数据节点
	LLIST *p = NULL;//p指针代替l作操作
	LLIST *newnode = NULL;//newnode指针指向新开辟的空间
	int i = 1;//循环变量
	l = malloc(sizeof(LLIST));//开辟数据节点的空间
	if(l == NULL)
		return NULL;
	memcpy(l,data,sizeof(LLIST));//拷贝数据
	p = l;//指针p代替l作操作
	for(i = 1;i<num;i++)
	{
		newnode = malloc(sizeof(LLIST));
		if(newnode == NULL)
			return NULL;
		memcpy(newnode,(LLIST *)(data)+i,sizeof(LLIST));
		p->next = newnode;
		p = newnode;//p指针永远指向最后一个节点
	}
		return l;//返回第一个节点的位置
}

void llist_display(LLIST *l)
{
	LLIST *cur = NULL;//cur指针指向所有的数据节点
	for(cur = l;cur!=NULL;cur = cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);

}

void llist_destroy(LLIST *l)
{
	LLIST *back = l;//back指针指向要销毁的前一个位置
	LLIST *cur = l->next;//cur指针指向要销毁的节点

	while(cur != NULL)
	{
		back->next = cur->next;
		free(cur);
		cur = back->next;
	}
	free(back);
}

LLIST *_find(LLIST *l,const void *find_data)
{
	LLIST *cur = NULL;//cur指针指向每一个数据节点
	
	for(cur = l;cur != NULL;cur = cur->next)
		if(cur->id == *(int *)find_data || !strcmp(cur->name,find_data))
		return cur;
	return NULL;
}
LLIST *llist_find(LLIST *l,const void *find_data)
{
	return _find(l,find_data);
}

int llist_delete(LLIST **l,const void *find_data)
{
	LLIST *back = *l;//back指向第一个数据节点
	LLIST *cur = NULL;//指向要删除的节点
	
	cur = _find(*l,find_data);//查找要删除的节点
	if(cur == NULL)
		return -1;
	if(cur == *l)//判断要删除的节点是不是第一个数据节点
	{
		*l = (*l)->next;//改变main()中l的指向
	}
	else
	{
		while(back->next!=cur)//找到删除节点的前一个位置
		back = back->next;
		back->next = cur->next;
	}	
	free(cur);
	return 0;
}

int llist_fetch(LLIST **l,const void *find_data,void *save)
{
	LLIST *back = *l;//back指针指向要取出节点的前一个位置
	LLIST *cur = NULL;//指针指向要取出节点的位置
	cur = _find(*l,find_data);//查找要取出的数据
	if(cur == NULL)
		return -1;
	if(cur == *l)//判断取出的节点是第一个数据节点
	{
		*l = (*l)->next;
	}
	else
	{
		while(back->next != cur)
			back = back->next;
		back->next = cur->next;
	}
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	return 0;
}











