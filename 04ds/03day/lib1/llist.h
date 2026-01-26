#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void(*llist_op)(const void*);//定义了函数指针
typedef int(*llist_cmp)(const void*,const void*);//定义了函数指针

struct llist_node//数据节点结构
{
	void *data;//指向数据域
	struct llist_node *prev;//指向前驱节点
	struct llist_node *next;//指向后继节点
};

typedef struct llist_head//头节点结构
{
	int size;//存储客户指点的大小
	struct llist_node head;//头节点
}LLIST；

LLIST *llist_create(int size);

int llist_insert(LLIST *handler,const void *data,int mode);

void llist_display(LLIST *handler,llist_op op);

void llist_destroy(LLIST *handler);

void *llist_find(LLIST *handler;const void *find_data,llist_cmp cmp);

int llist_delete(LLIST *handler,const void *find_data,llist_cmp cmp);

int llist_fetch(LLIST *handler,const void *find_data,llist_cmp cmp,void *save);










#endif
