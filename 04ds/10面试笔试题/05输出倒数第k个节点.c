//头节点为handler的单项不循环链表
//输出链表的第k个节点
struct llist_node *func(struct llist_node *handler, int k)
{
	struct llist_node *f = handler;
	struct llist_node *s = handler;
	while(k--)//先让快指针移动k步
	{
		if(f == NULL)
			return NULL;
		f = f->next;//先让快指针移动k步
	}
	while(f != NULL)//再让快慢指针一起走
	{
		f = f->next;
		s = s->next;
	}
	return s;//返回慢指针

}


