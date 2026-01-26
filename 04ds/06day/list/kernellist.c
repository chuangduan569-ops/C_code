#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"

#define NAMESIZE 20

typedef struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
	struct list_head node;//指针域
}STU;


int main(void)
{
	LIST_HEAD(handler);//创建头节点
	STU *data = NULL;//data指向开辟的空间
	int find_id = 102;
	char *find_name = "stu3";
	STU *datap = NULL;//指向数据节点的首地址
	STU save;
	struct list_head *pos = NULL;//pos指针指向每一个数据节点的指针域
	int i = 0;

	for(i=0;i<5;i++)
	{
		data = malloc(sizeof(STU));//开辟空间
		if(data == NULL)
			break;
		data->id = 100 + i;
		sprintf(data->name,"stu%d",i);
		data->math = 50 + i;
		list_add_tail(&data->node,&handler);//node是成员(变量)  handler也是变量  头插法
	}
	list_for_each(pos,&handler)
		//遍历链表(千万不要加分号，因为这个宏是一个循环，加；就算空循环)
	{
		datap = list_entry(pos, STU, node);//找到数据节点的首地址
		printf("%d	%s	%d\n",datap->id,datap->name,datap->math);
	}

	//查找链表
	list_for_each(pos,&handler)
	{
		datap = list_entry(pos, STU, node);
		if(datap->id == find_id || !strcmp(datap->name,find_name))
		{
			printf("Find It!%d	%s	%d\n",datap->id,datap->name,datap->math);
			break;
		}
	}
#if 0	//删除链表
	list_for_each(pos, &handler)//遍历链表
	{
		datap = list_entry(pos, STU, node);////找到数据节点的
		if(datap->id == find_id || !strcmp(datap->name,find_name))
			{
				printf("Delete It\n");
				list_del(pos);//把数据节点从链表中拆除
				free(datap);//释放
				break;	
			}
	}
	list_for_each(pos,&handler)
     //遍历链表(千万不要加分号，因为这个宏是一个循环，加；就算空循环)
     {
         datap = list_entry(pos, STU, node);//找到数据节点的首地址
         printf("%d  %s  %d\n",datap->id,datap->name,datap->math);
     }
#else	//取出链表
	list_for_each(pos, &handler)
	{
		datap = list_entry(pos, STU, node);//找到要取出节点的地址
		if(datap->id == find_id || !strcmp(datap->name,find_name))//判断是否为我们要取出节点的位置
		{
			memcpy(&save,datap,sizeof(STU));
			printf("Fetch It!%d	%s	%d\n",save.id,save.name,save.math);
			list_del(pos);//把数据节点从链表中删除
			free(datap);
			break;
		}
	}


#endif
	list_for_each(pos,&handler)
	//遍历链表(千万不要加分号，因为这个宏是一个循环，加；就算空循环)
	{
		datap = list_entry(pos, STU, node);//找到数据节点的首地址
		printf("%d	%s	%d\n",datap->id,datap->name,datap->math);
	}
	//销毁链表
	list_for_each(pos, &handler)
	{
		datap = list_entry(pos, STU, node);//找到数据节点的首地址
		list_del(pos);//把数据节点从链表中拆除
		free(datap);//释放
		pos = &handler;//pos指针指向头节点
	}

//可以写成之前那种的增删改查的函数，注意传参和变量的定义
	
	return 0;
}











