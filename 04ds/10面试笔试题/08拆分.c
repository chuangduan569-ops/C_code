//无头单向不循环链表的首节点l给了一个val值
//将小于val值的节点排在剩余节点之前
//不能改变链表原来的顺序，返回重新整理过的首节点
struct llist_node *func(struct llist_node *l,int val)
{
	struct llist_node *cur = l;//cur指向旧链表的当前节点
	struct llist_node *minhead = NULL;//minhead指向小链表
	struct llist_node *maxhead = NULL;//maxhead指向大链表
	struct llist_node *mincur = NULL;//mincur指向小链表当前操作的节点
	struct llist_node *maxcur = NULL;//maxcur指向大链表当前操作的节点

	minhead = malloc(sizeof(struct llist_node));//创建小链表的头
	maxhead = malloc(sizeof(struct llist_node));//创建大链表的头
	
	minhead->next = maxhead->next =NULL;//让小链表和大链表的头节点的next指向NULL

	mincur = minhead;//让mincur代替ninhead做操作
	maxcur = maxhead;//让maxcur代替maxhead做操作

	while(cur != NULL)//循环遍历旧链表的每个节点
	{
	if(cur->val < val)//判断当前节点是否小于val
	{
		mincur->next = cur;//把旧链表的节点挂到小链表上
		mincur = mincur->next;//mincur往后移动
	}
	else
	{
		maxcur->next = cur;////把旧链表的节点挂大小链表上
		maxcur = maxcur->next;////maxcur往后移动
	}
		cur = cur->next;//cur往后移动
	}
	mincur->next = maxhead->next;//让大链表的第一个数据挂在小链表的最后位置
	maxcur->next = NULL;//让大链表的最后一个节点的next指向NULL
	l = mixhead->next;//让l指针指向小链表的第一个数据节点
	free(minhead);
	free(maxhead);
	return l;
}


