#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "llist.h"


LLIST *llist_insert(LLIST *l,const void *data,int mode)
{
	LLIST *newnode = NULL;
	LLIST *p = l;
	if(l == NULL)
	{
		l = malloc(sizeof(LLIST));
			if(l == NULL)
				return NULL;
	memcpy(l,data,sizeof(LLIST));
	return l;//返回开辟成功的第一个数据节点地址
	}
	//说明当前链表不是空链表
	newnode = malloc(sizeof(LLIST));//开辟新的数据节点
	if(newnode == NULL)//判断新的数据节点开辟是否失败
		return l;//由于开辟新的数据节点失败，结束函数，并且返回链表的第一个节点的地址
	memcpy(newnode,data,sizeof(LLIST));
	switch(mode)
	{
		case HEADINSERT :newnode->next = l; l =newnode;break;
		case TAILINSERT :while(p->next != NULL) p = p->next;p->next = newnode;break;
		default:free(newnode);return l;
		//由于插入失败
	}
	return l;
}


void llist_display(LLIST *l)
{
	LLIST *cur = NULL;
	for(cur = l;cur != NULL; cur = cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

void llist_destroy(LLIST *l)
{
	LLIST *cur = l;//指向要销毁的节点
	LLIST *next = l->next;//next指针指向要销毁节点的下一个位置
	while(next != NULL)
	{
	free(cur);
	cur = next;
	next = next->next;
	}
	free(cur);
}





LLIST *_find(LLIST *l,const void *find_data)
{
	LLIST *cur = NULL;
	for(cur = l;cur->next!=NULL;cur = cur->next)
		if(cur->id == *(int *)find_data || !strcmp(cur->name, find_data))
            return cur;
}

LLIST *llist_find(LLIST *l,const void *find_data)
{
	return _find(l,find_data);
}

int llist_delete(LLIST **l,const void *find_data)
{
	LLIST *back = *l;//前指针
	LLIST *cur = NULL;//cur指向要删除的节点
	
	cur = _find(*l,find_data);
	if(cur == NULL)
	return -1;
	
	if(cur == *l)
	{
		*l = (*l) ->next;
		free(cur);
	}
	else
	{
		while(back->next!=cur)
		back = back->next;
		back->next = cur->next;
		free(cur);
	}
	return 0;
}

int llist_fetch(LLIST **l, const void *find_data, void *save)
{
    LLIST *back = *l;//前指针
    LLIST *cur = NULL;//cur指针指向要取出的节点

    cur = _find(*l, find_data);//查找要取出的节点
    if(cur == NULL)//判断是否没有找到要取出的节点
        return -1;//由于没有找到要取出的节点,结束函数,并且返回-1

    if(cur == *l)//判断找到的节点是否是第一个节点
    {
        *l = (*l)->next;//指向第二个数据节点
    }
    else
    {
        while(back->next != cur)//循环找到要拿出节点的前一个位置
            back = back->next;
        back->next = cur->next;
    }
    memcpy(save, cur, sizeof(LLIST));//拷贝数据
    free(cur);//释放第一个数据节点
    return 0;
}






