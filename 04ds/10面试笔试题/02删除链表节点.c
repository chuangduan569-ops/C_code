//给一个无头单向不循环链表的首届点handler和一个val数据
//把该链表中所有等于val的节点删除掉，并且返回新的首节点
struct llist_node *func(struct llist_node *l,int val)
{
	struct llist_node *back = NULL;//back指针指向要删除节点的前一个节点
	struct llist_node *cur = l;//cur指向要删除的节点
	

	while(cur!=NULL)//遍历所有节点
	{
		if(cur->val == val)//判断当前节点是否是要删除的节点
		{
			if(back->val == val)//判断要删除的节点是否首节点
			{
				l = cur->next;//把l指针指向下一个节点
				free(cur);//释放首节点
				cur = l;//cur指向新的链表的首节点
			}
			else//要删除的不是首节点
			{
				back->next = cur->next;//把要删除的节点从链表中删除
				free(cur);//释放要删除的节点
				cur = back->next;//cur指针指向下一个节点
			}
		}
		else
		{
			back = cur;//让back指针往后移动
			cur = cur->next;//让cur指针往后移动
		}
	}
		return l;//返回新链表的首节点
}
