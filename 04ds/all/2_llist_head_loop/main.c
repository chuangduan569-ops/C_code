#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;//handler指向头节点
	LLIST *find = NULL;//find指向要找到的数据
	int find_id = 102;
	char *find_name = "stu4";
	LLIST data;//data变量存储客户的值
	LLIST save;//save存储取出的值
	int  i = 0;
	int ret = 0;//用来接收返回值
	
	handler = llist_create();//创建头节点
	if(handler == NULL)
	{
		fprintf(stderr,"llist_creat() Failed!\n");
		return -1;//创建头节点失败返回-1
	}

	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 50 + i;
		data.prev = data.next = NULL;//让指针指向NULL
		llist_insert(handler, &data, HEADINSERT);//插入数据
	}
	
	llist_display(handler);//遍历表
	printf("------------------------\n");
	find = llist_find(handler,&find_id);
	if(find == NULL)
		printf("Not Find It!\n");
	else
		printf("Find It! %d	 %s	 %d\n",find->id,find->name,find->math);
#if 0
	ret = llist_delete(handler,find_name);
	if(ret != 0)
		printf("Delete Failed!\n");
	else
		printf("Delete It!\n");
#else
	ret = llist_fetch(handler,&find_id,&save);
	if(ret != 0)
		printf("Fetch Failed!\n");
	else
		printf("Fetch It!	%d	%s	%d\n",save.id, save.name, save.math);
		

#endif
	printf("------------------------\n");
	llist_display(handler);
	
	llist_destry(handler);
	
	return 0;
}




