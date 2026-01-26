#include <stdio.h>
#include "joseph.h"

int main(void)
{
	JOSEPH *j = NULL;//指针指向第一个数据节点
	int num = 0;//存储人数
	int kill = 0;//存储数到几杀人
	int alive = 0;//存储活着的人
	printf("请输入需要多少人");
	scanf("%d",&num);
	
	j = joseph_insert(num);//创建约瑟夫环
	if(j==NULL)
		return -1;
	joseph_display(j);//遍历约瑟夫环

	printf("请输入数到几杀人");
	scanf("%d",&kill);

	alive = joseph_play(j,kill,num);//杀人游戏
	printf("\nalive : %d\n",alive);

	return 0;
}
