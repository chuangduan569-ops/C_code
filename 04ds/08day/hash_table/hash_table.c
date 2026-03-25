#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "llist.h"

struct htable_head//哈希表核心结构体,(对外隐藏，仅内部可见，为了保证封装性)
{
	int size;//单个数据节点的数据域大小
	int capacity;//哈希表的容量(数组的长度)
	LLIST **bucket;//桶数组指针(每个元素是一个链表(LLIST *)的头节点)
};


HTABLE *htable_create(int size, int capacity)
{
	struct htable_head *handler = NULL;
	if(size<=0)//判断参数是否合法
		return NULL;//由于参数不合法,结束函数,并且返回NULL
	if(capacity <= 0)
		capacity = HTABLE_DEFAULT_CAPACITY;//由于参数不合法,使用默认的容量

	handler = malloc(sizeof(struct htable_head));//开辟哈希表结构的空间

	if(handler == NULL)//判断开辟哈希表结构的空间是否失败
		goto ERR_1;
		//return NULL;//由于开辟哈希表结构的空间失败,结束函数,并且返回NULL

	handler->size = size;//存储客户指定的数据节点中数据域的大小
	handler->capacity = capacity;//存储最终的容量大小
	handler->bucket = malloc(sizeof(LLIST *)*capacity);//分配桶数组的空间
	//开辟一个指针(LLIST *)的大小，*capacity 是客户指定的容量(指定的指针个数)
	if(handler->bucket == NULL)//判断分配桶数组的空间是否失败
	{
		//free(handler);//释放哈希表结构的空间
		//return NULL;//由于分配桶数组的空间失败,结束函数,并且返回NULL
		goto ERR_2;
	}
	
	for(int i = 0; i < capacity; i++)//循环为每个桶创建一个空链表(头节点)
	{
		handler->bucket[i] = llist_create(size);//为第i个桶创建头节点
		if(handler->bucket[i] == NULL)//判断为第i个桶创建头节点是否失败
		{
			for(int j = 0; j < i; j++)//回滚,销毁之前创建的所有桶
			{
				llist_destroy(handler->bucket[j]);//销毁链表
				//free(handler->bucket);//释放桶数组的空间
				//free(handler);//释放哈希表结构的空间
				//return NULL;//由于为第i个桶创建头节点失败,结束函数,并且返回NULL
				goto ERR_3;
			}
		}
	}
	return handler;
ERR_3:
	free(handler->bucket);//释放桶数组的空间
ERR_2:
	free(handler);//释放哈希表结构的空间
ERR_1:
	return NULL;

}


int htable_insert(HTABLE *handler,const void *data,htable_hash hash,htable_cmp cmp, int mode)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;//idx变量存储计算的索引值
	void *find_data = NULL;//指向查找到的数据节点

	if(h == NULL || data == NULL || hash == NULL || cmp == NULL)
		return -1;//由于参数不合理，结束函数，返回-1;
	
	idx = hash(data,h->capacity);//使用客户指定的hash函数计算索引值

	switch(mode)//根据插入模式处理重复的数据
	{
		case HTABLE_INSERT_REPLACE :	//覆盖已存在的数据
			llist_delete(h->bucket[idx], data, cmp);//在对应的桶中删除重复的数据
		case HTABLE_INSERT_NOSAME :	//不允许重复
			find_data = llist_find(h->bucket[idx], data, cmp);//在对应的桶中查找重复的数据
			if(find_data != NULL)
				return -2;//由于查找到了重复的数据，结束函数，返回-2
		case HTABLE_INSERT_SAME :	//允许重复
				break;
		default: return -2;//由于插入模式失败，结束函数，并且返回-2;
	}	
	return llist_insert(h->bucket[idx], data, TAILINSERT);//将数据尾插的形式插入到对应的桶中	
}


void htable_display(HTABLE *handler, htable_op op)
{
	struct htable_head *h = handler;//转换指针
	int i = 0;
	if(h == NULL || op == NULL)
		return ;//由于参数不合理结束函数
	for(i = 0; i < h->capacity; i++)//循环遍历哈希表桶
	{
		printf("bucket[%d]:",i);//打印哈希表桶的编号
		llist_display(h->bucket[i],op);//遍历下标为i的链表
		printf("\n");
	}
}


void htable_destroy(HTABLE *handler)
{
	struct htable_head *h = handler;//转换指针
	int i = 0;
	if(h == NULL)
		return ;
	
	for(i = 0; i < h->capacity; i++)//循环释放哈希表桶中的每条链表
		llist_destroy(h->bucket[i]);
	free(h->bucket);//释放哈希表桶
	free(h);//释放哈希表结构
	printf("Destroied!\n");
}

//用int的原因，返回的是桶的索引
static int _find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;//转换指针
	unsigned int idx = 0;//idx变量存储计算出的索引号
	
	if(h == NULL || find_data == NULL || cmp == NULL || hash == NULL)//判断参数是否合理
	return -1;
	
	idx = hash(find_data,h->capacity);//计算索引值
	return idx;
	
}

void *htable_find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;//转换指针
	unsigned int idx = 0;//idx变量存储计算出的索引号
	idx = _find(handler, find_data, cmp, hash);//调用内部的函数,计算索引值
	if(idx < 0)//判断是否没有计算出索引值
		return NULL;//由于没有计算出索引值,结束函数,并且返回NULL
	return llist_find(h->bucket[idx],find_data,cmp);
}


int htable_delete(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;

	idx = _find(handler,find_data,cmp,hash);
	if(idx < 0)
	return -1;
	return llist_delete(h->bucket[idx],find_data,cmp);
}


int htable_fetch(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash, void *save)
{
	struct htable_head *h = handler;
	unsigned int idx = 0;
	
	idx = _find(handler,find_data,cmp,hash);
	
	if(idx < 0)
	return -1;
	
	return llist_fetch(h->bucket[idx], find_data, cmp, save);

}








