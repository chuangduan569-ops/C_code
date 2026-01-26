#ifndef __LLIST_H
#define __LLIST_H
#define NAMESIZE 20

enum E{HEADINSERT,TAILINSERT};//枚举定义头插尾插

typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node *next;
}LLIST;
LLIST *llist_create(void);

int llist_insert(LLIST *handler,const void *data,int mode);

void llist_display(LLIST *handler);

LLIST *llist_find(LLIST *handler,const void *find_data);

int llist_delete(LLIST *handler,const void *find_data);

int llist_fetch(LLIST *handler,const void *find_data,void *save);

void llist_destroy(LLIST *handler);

#endif
