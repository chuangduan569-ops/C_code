#ifndef __LLIST_H
#define __LLIST_H
#define NAMESIZE 20
enum E{HEADINSERT,TAILINSERT};//枚举头插尾插

typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node* next;

}LLIST;//起别名
int llist_create(LLIST** handler);
int llist_insert(LLIST* handler,const void* data,int mode);
void llist_display(LLIST* handler);
void llist_destroy(LLIST* handler);
LLIST* llist_find(LLIST* handler,const void* find_data);
int llist_delete(LLIST* handler,const void* find_data);
int llist_fetch(LLIST* handler,const void* find_data,void* save);

#endif
