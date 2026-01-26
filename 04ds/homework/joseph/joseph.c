#include <stdio.h>
#include <stdlib.h>
#include "joseph.h"

JOSEPH *joseph_insert(int num)
{
	JOSEPH *j = NULL;//指针指向第一个数据节点
	JOSEPH *p = NULL;//p指针辅助j指针进行操作
	JOSEPH *newnode = NULL;//newnode指向新创建出的节点
	int i = 1;
	j = malloc(sizeof(JOSEPH));//创建第一个节点
	if(j==NULL)
		return NULL;
	j->id = i;
	j->next = j;
	p = j;//使用p指针辅助j指针

	for(i = 2; i <= num; i++)
	{
		newnode = malloc(sizeof(JOSEPH));
		newnode->id = i;
		newnode->next = p->next;	
		p->next = newnode;
		p = p->next;
	}
	return j;
}

void joseph_display(JOSEPH *j)
{
	JOSEPH *cur = NULL;
	for(cur = j;cur->next != j;cur = cur->next)
		printf("%d ",cur->id);
	printf("%d",cur->id);
}

int joseph_play(JOSEPH *j,int kill,int num)
{
	JOSEPH *back = NULL;//back指向要杀掉的人的前一个位置
	JOSEPH *p = NULL;//p指针指向要杀掉的人
	int i = 0;//循环变量
	int alive = 0;
	for(back = j;back->next != j;back = back->next);//找到最后一个节点

	while(--num)
	{
		for(i = 0;i < kill - 1; i++)
			back = back->next;
		p = back->next;//p指向要杀掉的人
		back->next = p->next;
		printf("%-3d",p->id);
		free(p);
	}
	alive = back->id;
	free(back);

	return alive;

}






