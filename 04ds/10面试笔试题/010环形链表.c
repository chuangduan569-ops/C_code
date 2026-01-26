//给一个单向的链表首节点handler，判断链表中是否存在环状结构
//循环链表也属于环状结构，属于特殊的环状结构，首尾相连

int struct(struct llist_node *handler)
{
	struct llist node *f = handler;
	struct llist node *s = handler;
	while(f != NULL && f->next != NULL)
	{
		f = f->next-next;//快指针一次走两步
		s = s->next;//慢指针一次走一步
		if(s==f)//判断快慢指针是否相遇
		return 1;//返回真值
	}
	return 0;//返回假值
}



