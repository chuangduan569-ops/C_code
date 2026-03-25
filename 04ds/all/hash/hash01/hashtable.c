#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"
#include "hashtable.h"

struct htable_head{
	int size;//单个数据节点的数据域大小
	int capacity;//哈希表的容量(数组的长度)
	LLIST **bucket;//桶指针数组(每个元素是一个(LLIST *)的链表)

};

HTABLE *htable_create(int size, int capacity)
{
	struct htable_head *handler = NULL;
	
	int i = 0;

	if(size <= 0)
		return NULL;
	if(capacity <= 0)
	capacity = HTABLE_DEFAULT_CAPACITY;//由于参数不合法,使用默认的容量

	handler = malloc(sizeof(struct htable_head));
	if(handler == NULL)
		return NULL;

	handler->size = size;
	handler->capacity = capacity;
	handler->bucket = malloc(sizeof(LLIST *) * capacity);//分配桶数组的空间
	if(handler->bucket == NULL)
	{
		free(handler);
		return NULL;
	}
	
	for(i = 0; i < capacity; i++)
	{
		handler->bucket[i] = llist_create(size);////为第i个桶创建头节点
		if(handler->bucket[i] == NULL)
		{
			for(int j = 0; j < i; j++)//回滚释放已经开辟的桶
				llist_destroy(handler->bucket[j]);//为第i个桶创建头节点
			free(handler->bucket);//释放桶数组的空间
			free(handler);//释放哈希表结构的空间;
			return NULL;//由于为第i个桶创建头节点失败,结束函数,并且返回NULL
		}
		}

		return handler;
	}


int htable_insert(HTABLE *handler, const void *data, htable_hash hash, htable_cmp cmp,int mode)
{
	struct htable_head *h = handler;//转换指针
	unsigned int idx = 0;//idx变量存储计算的索引值
	void *find_data = NULL;//find_data指针指向查找到的数据节点
	
	if(h == NULL || data == NULL || hash == NULL || cmp == NULL)
		return -1;//由于参数不合理,结束函数,并且返回-1

	idx = hash(data, h->capacity);////使用客户指定的hash函数计算索引值

	switch(mode)
	{
		case HTABLE_INXERT_REPLACE : //覆盖已存在的数据
			llist_delete(h->bucket[idx], data, cmp);//在对应的桶中删除重复的数据
			break;
		case HTABLE_INXERT_NOSAVE : //不允许重复
			find_data = llist_find(h->bucket[idx], data, cmp);//在对应的桶中查找重复的数据
			if(find != NULL)//判断是否查找到了重复的数据
				return -2;//由于查找到了重复的数据,结束函数,并且返回-2
		case HTABLE_INXERT_SAVE : //允许重读
			break;
		default : return -3;//由于插入模式失败,结束函数,并且返回-3
	}
		return llist_insert(h->bucket[idx], data, TAILINSERT);//将数据以尾插的形式插入到对应

}


void htable_display(HTABLE * handler, htable_op op)
{
	struct htable_head *h = handler;
	int i = 0;
	if(h == NULL || op == NULL)
		return ;
	for(i = 0; i < h->capacity; i++)//循环遍历哈希表桶
	{
		printf("bucket[%d]", i);//打印哈希表桶的编号
		llist_display(h->bucket[i],op);;//遍历下标为i的链表
		printf("\n");
	}
	
}


void htable_destory(HTABLE *handler)
{
	struct htable_head *h = handler;//转换指针
	int i = 0;
	if(h == NULL)
	return ;

	for(i = 0; i < h-capacity; i++)//循环释放哈希表桶中的每条链表
		llist_destroy(h->bucket[i]);
	
	free(h->bucket);//释放哈希表桶
	free(h);//释放哈希表结构
	printf("释放完毕\n");
}

static int _find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	
	if(h == NULL || cmp == NULL || hash == NULL || find_data)
		return -1;

	idx = hash(dind_data, hash);//计算索引值
	return idx;//返回索引值
}

void *htable_find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	
	idx = _find(handler, find_data, cmp, hash);//调用内部的函数,计算索引值

}


int htable_delete(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)




int htable_fetch(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash, void *save)






