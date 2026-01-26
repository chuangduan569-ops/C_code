//单向的链表首节点handler判断链表中是否存在环状结构
//如果是环状结构，找到环的入口点
//循环链表也属于环状结构，属于特殊的环状结构，首尾相连

struct llist_node *func(struct llist_node *handler)
{
	struct llist_node *f = handler;
	struct llist_node *s = handler;
	struct llist_node *p = handler;//p指针从链表头开始走
	while(f != NULL && f->next !=NULL)
	{
		f = f->next->next;//快二慢一
		s = s->next;
		if(s == f)
		{
			while(s != p)//让p指针和慢指针一起走，只要碰到一起就说明是入口点了
			{
				s = s->next;
				p = p->next;
			}	
			return p;
		}
	}
	return NULL;
}	





