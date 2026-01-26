#ifndef __LLIST_H
#define __LLIST_H

#define HEADINSERT 0
#define TAILINSERT 1

typedef void LLIST;//因为这里使用了万能指针，万能指针可以存储任意类型的数据，但是不能通过万能指针直接访问数据的存储地址
typedef void (*llist_out)(const void*);
typedef int (*llist_cmp)(const void*, const void*);


LLIST *llist_create(int size);

int llist_insert(LLIST *handler, const void *data, int mode);

void llist_display(LLIST *handler, llist_out out);

void llist_destory(LLIST *handler);

void *llist_find(LLIST *handler,const void *find_data, llist_cmp cmp);

int llist_delete(LLIST *handler,const void *find_data, llist_cmp cmp);

int llist_fetch(LLIST *handler,const void *find_data, llist_cmp cmp, void *save);









#endif
