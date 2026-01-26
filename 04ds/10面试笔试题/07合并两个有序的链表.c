//将两个升序的无头链表合并为一个新的升序链表并返回
//新链表是通过拼接两个链表的所有节点组合而成的

struct llist_node *func(struct llist_node *l1,struct llist_node *l2)
{
	struct llist_node *l = NULL;//l指针指向新链表的首节点
	struct llist_node *cur = NULL;//cur指向当前正在操作的节点
	if(l1 == NULL)
		return l2;
	if(l2 == NULL)
		return l1;
	if(l1->val < l2->val)//判断两条链表的首节点，谁来当链表的首节点
	{
		l = cur = l1;//选择l1作为新链表的首节点
		l1 = l1->next;//找到l1链表的下一个节点
	}
	else
	{
		l = cur = l2;//选择l2作为新链表的首节点
		l2 = l2->next;//找到l2链表的下一个节点
	}
	while(l1 != NULL && l2 != NULL)//循环拆旧链表的节点插入新链表中
	{
		if(l1->val < l2->val)
		{
			cur->next = l1;//把l1链表的节点挂到新链表上
			l1 = l1->next;//找到l1的下一个节点
		}
		else
		{
			cur->next = l2;//把l2链表的节点挂到新链表上
			l2 = l2->next;//找到l2的下一个节点
		}
		cur = cur->next;//cur向后移动
	}
	if(l1 == NULL)//判断了l1是否先结束
		cur->next = l2;//把l2链表的剩余节点挂到新链表上
	else
		cur->next = l1;//把l1链表的剩余节点挂到新链表上
	return l;
}

