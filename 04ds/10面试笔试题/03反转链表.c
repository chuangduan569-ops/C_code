//无头单向不循环链表的首节点l
//把这个链表反过来，并返回反转之后新的首节点
struct llist_node *func(struct llist_node *l)
{
	struct llist_node *cur = l;//cur指向要反转的节点
	struct llist_node *p = NULL;//p指针指向原链表中cur节点的下一个节点
	struct llist_node *next = NULL;//next指向反转之后cur指向节点的下一个节点
	while(cur != NULL)//遍历链表中的每一个节点
	{
		p = cur->next;//p指针指向cur的下一个节点
		cur->next = next;//反转cur指针指向的节点
		next = cur;//next指针指向反转后cur指向的下一个节点
		cur = p;//cur指针向后移动
	}
	return next;
}






