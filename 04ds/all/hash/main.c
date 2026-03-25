#include <stdio.h>
#include <string.h>
#include "hashtable.h"

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)
{
	const struct stu *p = data;
	printf("	%d %s %d %d->", p->id, p->name, p->math, p->tel);
}

int id_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	return !(p->tel - *k);
}

int name_cmp(const void *data, const void *key)
{//传过来的两个参数做比较
	const struct stu *p = data;
	const char *k = key;
	return !strcmp(p->name,k);
}

unsigned int id_hash(const void *data, int capacity)
{
	const struct stu *p = data;//转换指针
	return p->id % capacity;//计算哈希索引
}

unsigned int tel_hash(const void *data, int capacity)
{
	const struct stu *p = data;
	return p->tel % capacity;//计算哈希索引
}

unsigned int name_hash(const void *data, int capacity)
{
	const char *name = data;//转换指针
	unsigned int hash = 0;
	while(*name)
	{
		//字符串哈希函数hash * 31 + 字符ASCII值
		hash = hash * 31 + *name++;
		//先取当前字符值，然后指针后移
		//31能很好地混合字符串中每个字符的信息，减少哈希冲突，而且计算速度快。
	}
	return hash % capacity;//计算哈希索引值
}


int main(void)
{
	HTABLE *ht = NULL;//ht指针指向哈希表的结构

	struct stu data;
	struct stu save;
	struct stu *find = NULL;//find指针指向查找到的数据节点的数据域

	int find_id = 104;
	char *find_name = "stu8";
	int find_tel = 10015;

	int i = 0;
	int ret = 0;
	

	//创建哈希必表(每个节点的数据域大小， 哈希表的容量)
	ht = htable_create(sizeof(struct stu), 8);
	
	if(ht == NULL)
	{
		fprintf(stderr,"FAILED!\n");
		return -1;
	}

	for(i = 0; i < 10; i++)
	{
		data.id = 100 + i;
		sprintf(data.name, "stu%d", i);
		data.math = 50 + i;
		data.tel = 10010 + i;
		//(指向哈希表的指针，插入的数据，哈希函数，对比函数，插入模式)
		htable_insert(ht, &data, id_hash, id_cmp, HTABLE_INSERT_SAVE);
	}

	htable_display(ht,print);//遍历(指向哈希表的指针，打印函数)

	printf("---------------------------------\n");
	find = htable_find(ht, &find_id, id_cmp, id_hash);
	if(find == NULL)
	printf("NOT FIND!\n");
	else
	{
		printf("Find It!\n");
		print(find);
		printf("\n");
	}
#if 0
	ret = htable_delete(ht, &find_id, id_cmp, id_hash);
	if(ret != 0)
	printf("Delete Failed!\n");
	else
		printf("Delete0 It\n");
#else
	ret = htable_fetch(ht, &find_id, id_cmp, id_hash, &save);
	if(ret != 0)
	printf("Fetch Failed!\n ");
	else
	{
		printf("Fetch!\n");
		print(&save);
		printf("\n");
	}

#endif


	
	
	printf("---------------------------------\n");
	
	htable_display(ht,print);//遍历(指向哈希表的指针，打印函数)

	htable_destroy(ht);//销毁(指向哈希表的指针)	
	htable_display(ht,print);//遍历(指向哈希表的指针，打印函数)

	return 0;


}





