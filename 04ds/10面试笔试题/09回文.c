//无头单向不循环首节点l，最后返回值为真假值
//是回文返回真值
//不是回文假值
//只存在一个节点也是回文

int func(struct llist_node *l)
{
	struct llist_node *s,*f;
	struct llist_node *p = NULL;
	
	if(l == NULL || l->next == NULL)//判断链表是否没有节点或者只有一个节点
		return 1;
	s = f = l;
	while(f != NULL && f->next != NULL)
	{//让慢指针走一步,快指针走两步
		f = f->next->next;
		s = s->next;
	}
	f = s->next;//快指针指向慢指针后续节点
	s->next = NULL;
	
	while(f != NULL)//反转后续链表
	{
		p = f->next;
		f->next = s;
		s = f;
		f = p;
	}
	 //将快指针指向旧链表的首节点
	f = l;
	while(f != NULL && s!= NULL)
	{
		if(f->val != s->val)
			return 0;
		f = f->next;
		s = s->next;
	}
	return 1;
}


