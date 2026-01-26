#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *l = NULL;//指向无头不循环链表的第一个节点
	LLIST *find = NULL;//指针指向查找到的数据节点
	int find_id = 102;
	char *find_name = "stu2";
	LLIST data;//data变量存储客户录入的数据
	LLIST save;//save变量存储取出的数据
	int i = 0;
	int ret = 0;

	for(i = 1; i < 5;i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 50 + i;
		data.next = NULL;
		l = llist_insert(l, &data,HEADINSERT);//插入数据节点
	}

	llist_display(l);//遍历链表
	
	printf("----------------------------\n");
	find = llist_find(l,&find_id);
	if(find == NULL)
		printf("Find Failed!\n");
	else
		printf("Find It!	%d	%s	%d\n",find->id,find->name,find->math);
#if 0
	ret = llist_delete(&l,find_name);//删除节点
	if(ret != 0)
		printf("Delete Failed\n");
	else
		printf("Delete It!\n");
#else
	ret = llist_fetch(&l,find_name,&save);
	if(ret != 0)
		printf("Fetch Failed\n");
	else
		printf("Fetch It!	%d	%s	%d\n",save.id,save.name,save.math);
#endif
	printf("----------------------------\n");
	llist_display(l);
	llist_destroy(l);
	return 0;
}
