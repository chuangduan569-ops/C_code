#ifndef __LLIST_H
#define __LLIST_H
#endif
#define NAMESIZE 20

enum E{HEADINSERT,TAILINSERT};//枚举设定头插尾插

typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node *next;//指向下一个节点
}LLIST;//起个别名

LLIST *llist_create(void);
int llist_insert(LLIST *handeler,const void *data,int mode);
void llist_display(LLIST *handler);
void llist_destory(LLIST *handler);
LLIST *llist_find(LLIST *handler,const void *find_data);
int llist_delete(LLIST *handler,const void *find_data);
int llist_fetch(LLIST *handler,const void *find_data,void *save);
