#ifndef __JOSEPH_H
#define __JOSEPH_H

typedef struct llist_node
{
	int id;
	struct llist_node *next;
}JOSEPH;

JOSEPH *joseph_insert(int num);
	
void joseph_display(JOSEPH *j);

int joseph_play(JOSEPH *j,int kill,int num);



#endif
