//删除倒数第k个节点
//思路让快指针走k+1步，快慢一起走，
//慢指针指向的下一个节点(定义一个新的指针)就是要删除的节点
int llist(struct llist_node *handler, int k)
{
	struct llist_node *f = handler;
	struct llist_node *s = handler;
	struct llist_node *d = NULL;
	do{//先让快指针走k+1步
		if(f == NULL)
			return NULL:
		f = f->next;
	}while(k--)
	while(f != NULL)
	{//快慢指针一起走
		f = f->next;
		s = s->next;
	}
	d = s->next;//d指针指向要删除的节点
	s->next = d->next;//把要删除的节点从链表中摘除
	free(d);//释放要删除的节点
	return 0;
}


