#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "llist.h"

LLIST *llist_create(void)
{
	LLIST *handler = NULL;//指针指向开辟空间
	handler = malloc(sizeof(LLIST));//开辟空间大小
	if(handler == NULL)//判断开辟空间是否失败
		return NULL;//开辟失败返回NULL
	handler->next = NULL;//开辟成功，让头节点的next指向NULL
	return handler;//返回开辟空间的地址
}
int llist_insert(LLIST *handler,const void *data,int mode)
{
	LLIST *p = handler;//定义一个指针p代替handler作操作
	LLIST *newnode = NULL;//指针指向新开辟的空间
	newnode = malloc(sizeof(LLIST));//
	if(newnode == NULL)//判断新开辟的空间是否失败
		return -1;//失败返回-1
	memcpy(newnode,data,sizeof(LLIST));//拷贝数据到新的节点
	//检查插入模式
	switch(mode)
	{
	case HEADINSERT : break;
	 //尾插，找到最后一个指向NULL的节点
	case TAILINSERT : while(p->next!=NULL) p = p->next; break;
	//因为开辟空间成功了，所以如果模式错误需要释放空间，返回-2
	default : free(newnode); return -2;
	}
	newnode->next = p->next;//插入新节点
	p->next = newnode;

	
}
void llist_display(LLIST *handler)
{
//	printf("------------------------------\n");
	LLIST *cur = NULL;//遍历每一个节点
	for(cur = handler->next;cur!=NULL;cur = cur->next)
		printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
}

static LLIST *_find(LLIST *handler,const void *find_data)
{
        LLIST *cur = NULL;//cur指针指向每个数据节点
            for(cur = handler->next;cur!=NULL;cur = cur->next){
        //      printf("找到了！%d  %s  %d\n",cur->id,cur->name,cur->math);
    
            if(cur->id==*(int *)find_data || !strcmp(cur->name,find_data))
                    return cur;
            }
            return NULL;
}

LLIST *llist_find(LLIST *handler,const void *find_data)
{
        return _find(handler,find_data);
}

int llist_delete(LLIST *handler,const void *find_data)
{
	LLIST *back = handler;//前指针
	LLIST *cur = NULL;
	cur = _find(handler,find_data);//找到要删除的节点
	if(cur == NULL)
		return -1;//没有找到返回-1
	while(back->next!=cur)//找到要删除的前一个节点
		back = back->next;
	back->next = cur->next;//把要删除的节点从链表中摘除
	free(cur);
	return 0;
	
}  
int llist_fetch(LLIST *handler,const void *find_data,void *save)
{
	LLIST *back = handler;//前指针
	LLIST *cur = NULL;
	cur = _find(handler,find_data);//查找要取出的数据
//	printf("============");
//	printf("%d	%s	%d\n",cur->id,cur->name,cur->math);
	if(cur == NULL)
		return -1;//没有找到返回-1
	while(back->next!=cur)
		back = back->next;
	back->next = cur->next;
	memcpy(save,cur,sizeof(LLIST));
	free(cur);
	return 0;
}
void llist_destroy(LLIST *handler)
{
	LLIST *back = handler;//前指针
	LLIST *cur = handler->next;//后指针
	while(cur!=NULL)//循环释放所有的节点
	{
		back->next = cur->next;
		free(cur);
		cur = back->next;
	}
		free(back);
}











