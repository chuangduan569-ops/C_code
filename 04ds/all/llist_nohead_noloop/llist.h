#ifndef __LLIST_
#define __LLIST_

#define NAMESIZE 20
#define HEADINSERT 0
#define TAILINSERT 1

typedef struct llist_node
{
	int id;
	char name[NAMESIZE];
	int math;
	struct llist_node *next;
}LLIST;

LLIST *llist_insert(LLIST *l, const void *data, int mode);

void llist_display(LLIST *l);

void llist_destroy(LLIST *l);

LLIST *llist_find(LLIST *l, const void *find_data);

int llist_delete(LLIST **l, const void *find_data);

int llist_fetch(LLIST **l, const void *find_data, void *save);






#endif
