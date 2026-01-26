#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

struct llist_node
{
	struct llist_node *prev;
	struct llist_node *next;
	char data[0];
};

struct llist_head
{
	int size;
	struct llist_node head;
};









