#ifndef __FUNC_H
#define __FUNC_H
enum 
{
	HEADINSERT,
	TAILINSERT
};
struct llist_node
{
//	void *data;//指向数据域
	struct llist_node *prev;//前驱指针
	struct llist_node *next;//后去指针
	char data[0];//可变长的结构体
};
typedef struct llist_head
{
	int size;
	struct llist_node head;
}LLIST;
LLIST *llist_head_create(int size);

int llist_insert(LLIST*,const void *data,int mode);

int llist_revise(LLIST *handler,const void *old_data, const void *new_data);

void *llist_find(LLIST*,const void *data);

int llist_delete(LLIST*,const void *);

void llist_destroy(LLIST*);
#endif
