 struct llist_node *func(struct llist_node *l,int val)
 {
 	struct llist_node *back = NULL;//back指向要删除节点的前一个节点
 	struct llist_node *cur = l;//cur指针指向要删除的节点
 	while(cur != NULL)
 	{
 		if(cur->val == val)//判读当前节点是要删除的节点
 			{
 				if(back == NULL)//判断要删除的节点是否为首节点
 					{
 					l = cur->next;//把l指针指向下一个节点
 					free(l);//释放首节点
 					cur = l;//cur指向新链表的首节点
 					}
 				else//要删除的不是首节点
 				{
 					back->next = cur->next;//把要删除的节点从链表中摘除
 					free(cur);//释放删除的节点
 					cur = back->next;//cur指针指向下一个节点	
 				}
 			}
 		else
 		{
 		back = cur;//让back指针往后移动
 		cur = cur->next;//让cur指针往后移动
 		}
 	}
 	return l;//返回新链表的首节点地址
 }
//反转链表


struct llist_node *func(struct llist_node *l)
{

}










