#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1


typedef void LLIST;
typedef void(*llist_op)(const void*);//定义一个函数指针
typedef int(*llist_cmp)(const void*, const void *);

LLIST *llist_create(int size);

int  llist_insert(LLIST *handler, const void *data, int mode);

void llist_display(LLIST *handler, llist_op op);

void llist_destroy(LLIST *handler);

LLIST *llist_find(LLIST *handler,const void *find_data, llist_cmp cmp);

int llist_delete(LLIST *handler, const void *find_data, llist_cmp cmp);

int llist_fetch(LLIST *handler, const void *find_data, llist_cmp cmp, void *save);


#endif


