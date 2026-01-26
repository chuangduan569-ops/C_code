#ifndef __LLIST_H
#define __LLIST_H

#define NAMESIZE 20

typedef struct llist_node
{
	int id;//数据域
	char name[NAMESIZE];
	int math;
	struct llist_node *next;//指针域
}LLIST;

LLIST *llist_insert(const void *data,int num);

void llist_display(LLIST *l);

void llist_destroy(LLIST *l);

LLIST *llist_find(LLIST *l,const void *find_data);

int llist_delete(LLIST **l,const void *find_data);

int llist_fetch(LLIST **l,const void *find_data,void *save);




#endif
