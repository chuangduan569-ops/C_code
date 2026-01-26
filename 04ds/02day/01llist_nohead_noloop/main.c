#include <stdio.h>
#include "llist.h"


int main(void)
{
	LLIST *l = NULL;//l指向无头单项不循环链表的第一个节点
	int i = 0;//循环变量
	int ret = 0;
	LLIST *find = NULL;
	int find_id = 102;
	char *find_name = "stu3";
	LLIST data;//data存储客户录入的数据
	LLIST save;	
	for(i = 0;i < 5; i++)
	{
		data.id = 100+i;
		sprintf(data.name,"stu%d",i);
		data.math = 50+i;
		data.next = NULL;
		l = llist_insert(l,&data,HEADINSERT);
	}

	llist_display(l);
	printf("------------------\n");
/*	find = llist_find(l,&find_id);//查找节点
	if(find == NULL)
	printf("Not Find It\n");
	else
	printf("Find It%d	%s	%d\n",find->id,find->name,find->math);
*/
#if 0
	ret = llist_delete(&l,&find_id);//删除节点
	if(ret != 0)
		printf("Failed\n");
	else
		printf("delete It\n");
#else
  ret = llist_fetch(&l,&find_id,&save);//删除节点
  if(ret != 0)
      printf("Failed\n");
  else
      printf("Fetch It%d	%s	%d\n",save.id,save.name,save.math);

#endif
	llist_display(l);
	printf("------------------\n");   
	llist_destroy(l);

	return 0;
}
