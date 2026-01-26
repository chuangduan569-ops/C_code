#include <stdio.h>
#include "joseph.h"

int main(void)
{
	JOSEPH *j = NULL;//指针指向约瑟夫环
	int people_n = 0;//用来存储总人数
	int kill_n = 0;//用来存储数道几杀人
	int alive = 0;//用来存储活下来的人的id
	
	printf("请输入总人数 : ");
	scanf("%d", &people_n);
	
	j = llist_insert(people_n);//根据总人数创建约瑟夫环
	
	if(j == NULL)
		return -1;
	
	llist_display(j);//遍历约瑟夫环

	printf("请输入数到几杀人 : ");
	scanf("%d", &kill_n);
	
	alive = llist_play(j,people_n,kill_n);//开始杀人游戏
	printf("\n============\n");
	printf("%d \n",alive);

	


	return 0;
}
