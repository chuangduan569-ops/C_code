#ifndef __JOSEPH_H
#define __JOSEPH_H


typedef struct llist_node
{
	int id;
	struct llist_node *next;
}JOSEPH;

JOSEPH *llist_insert(int p_n);

void llist_display(JOSEPH *j);

int llist_play(JOSEPH *j,int p_n, int k_n);



#endif
