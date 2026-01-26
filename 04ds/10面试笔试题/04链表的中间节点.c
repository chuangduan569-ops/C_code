//头节点为handler的单项不循环链表
//返回链表的中间节点，如果有两个返回第二个人中间节点
//快慢指针
struct llist_node *func(struct llist_node *handler)
{
	struct llist_node *f = handler;//快指针
	struct llist_node *s = handler;//慢指针
	//f != NULL 当链表是偶数节点
	//f->next != NULL当链表是奇数个节点
	while(f != NULL && f->next != NULL)
	{
		f = f->next->next;//快指针一次走两步
		s = s->next;//慢指针一次走一步
	}
	return s;//返回慢指针
}
