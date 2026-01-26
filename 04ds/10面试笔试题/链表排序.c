//插入排序将第一个元素已经排序好的部分
//然后依次将后面的元素插入到已排序的数列中，直到所有成员都插入为止

void sort(struct llist_node *handler)
{//如果是有头链表直接使用handler，如果无头链表创建一个头节点
	if(handler->next == NULL || handler == NULL)//判断是否不需要排序
		return ;
	struct llist_node *cur = handler->next;//cur指针标记当前位置
	struct llist_node *next = cur->next;//next指针表示当前位置的下一个位置
	struct llist_node *order = NULL;//order指针指向将来有序的链表

	cur->next = NULL;//断开链接；
	cur = next;
	
	while(cur != NULL)
	{
		order = handler;//order在有序链表中先标记handler的位置
		next = cur->next;//next指针往后移动
		while(order->next != NULL && order->next->val < cur->val)
		{
			order = order->next;
		cur->next = order->next;
		order->next = cur;
		cur = next;
		}	
	}
}




