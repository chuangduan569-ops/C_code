#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "llist.h"

struct htable_head//哈希表核心结构体，对外隐藏
{
	int size;//单个数据节点的数据域大小
	int capacity;//哈希表的容量(数组的大小)
	LLIST **bucket;//桶指针数组(每个元素是一个链表(LLIST *)的头节点)
};

HTABLE *htable_create(int size, int capacity)
{
	struct htable_head *handler = NULL;//handler指向新开辟的哈希表结构空间
	
	if(size <= 0)
		return NULL;
	if(capacity <= 0)
		capacity = HTABLE_DEFAULT_CAPACITY;//使用默认值
	
	handler = malloc(sizeof(struct htable_head));//开辟哈希表结构的空间
	//struct htable_head//哈希表核心结构体
	if(handler == NULL)
	return NULL;
	
	handler->size = size;
	handler->capacity = capacity;
	handler->bucket = malloc(sizeof(LLIST *) * capacity);//一个指针(LLIST *)的大小*容量(capacity)的大小,分配桶数组的空间
	if(handler->bucket == NULL)
	{
		free(handler);//释放嘻哈结构的空间
		return NULL;
	}
	for(int i = 0; i < capacity; i++)//循环为每一个桶创建一个空链表(头节点)
	{
		handler->bucket[i] = llist_create(size);//为第i个桶创建头节点
		if(handler->bucket[i] == NULL)
		{
			for(int j = 0; j < i; j++)//回滚,销毁之前创建的所有桶
				llist_destroy(handler->bucket[j]);//销毁链表
			free(handler->bucket);
			free(handler);
			return NULL;
		}
	}
	return handler;
}

int htable_insert(HTABLE *handler, const void *data, htable_hash hash, htable_cmp cmp, int mode)
{
	struct htable_head *h = handler;//转换指针
	unsigned int idx = 0;//存储计算的索引值
	void *find_data = NULL;//指针指向要查找的数据节点
	
	if(h == NULL || cmp == NULL || data == NULL || hash == NULL)
	return -1;
	
	idx = hash(data, h->capacity);//使用客户指定的hash计算索引值
	
	switch(mode)
	{
	case HTABLE_INSERT_REPLACE ://覆盖已存在的数据
		llist_delete(h->bucket[idx], data, cmp);//在对应的桶中删除数据
		break;
	case HTABLE_INSERT_NOSAVE ://不允许重复
		find_data = llist_find(h->bucket[idx],data,cmp);//在对应的桶中查找重复的数据
		if(find_data != NULL)
			return -2;//由于找到了重复的数据，结束函数，并且返回-2
	case HTABLE_INSERT_SAVE ://允许重复
		break;
	
	default :return -3;

	}
	return llist_insert(h->bucket[idx], data, HEADINSERT);//将数据以尾插的形式插入到对应的桶中
}


void htable_display(HTABLE *handler, htable_op op)
{
	struct htable_head *h = handler;//转换指针
	
	if(h == NULL || op == NULL)
		return ;
	
	for(int i = 0; i < h->capacity; i++)
		{
			printf("BUCKET[%d] :", i);//打印哈希表桶的编号
			llist_display(h->bucket[i], op);//遍历下标为i的链表
			printf("\n");
		}
}


void htable_destroy(HTABLE *handler)
{
	struct htable_head *h = handler;
	if(h == NULL)
		return ;
	
	for(int i = 0; i < h->capacity; i++)
		llist_destroy(h->bucket[i]);
	free(h->bucket);//释放哈希表桶
	free(h);//释放哈希表结构
	printf("销毁完毕\n");
	h = NULL;
}

static int _find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	if(h == NULL || cmp == NULL || hash == NULL || find_data == NULL)
		return -1;
	idx = hash(find_data, h->capacity);//计算索引值
	return idx;
}

void *htable_find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	
	idx = _find(handler, find_data, cmp, hash);//调用内部函数，计算索引值

	if(idx < 0)
	return NULL;

	return llist_find(h->bucket[idx], find_data, cmp);
}


int htable_delete(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	
	idx = _find(handler, find_data, cmp, hash);//调用内部函数，计算索引值
	
	if(idx < 0)
		return -1;
	return llist_delete(h->bucket[idx], find_data, cmp);
}


int htable_fetch(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash, void *save)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;

	idx = _find(handler, find_data, cmp, hash);
	if(idx < 0)
		return -1;

	return llist_fetch(h->bucket[idx], find_data, cmp, save);

}




















