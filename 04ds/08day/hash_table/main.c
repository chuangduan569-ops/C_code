#include <stdio.h>
#include <string.h>
#include "hash_table.h"

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
	printf("  %d %s %d %d->", p->id, p->name, p->math, p->tel);
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

/*
功能：基于学生ID的哈希函数，计算数据应存入的索引值 
参数：data指向学生结构体的指针
		capacity 哈希表的容量
返回值：计算出哈希索引
*/
unsigned int id_hash(const void *data, int capacity)
{
	const struct stu *p = data;//转换指针
	return p->id % capacity;//计算哈希索引
}


/*
功能：基于学生TEL的哈希函数，计算数据应存入的索引值 
参数：data指向学生结构体的指针
        capacity 哈希表的容量
返回值：计算出哈希索引
*/
unsigned int tel_hash(const void *data, int capacity)
{
    const struct stu *p = data;//转换指针
    return p->tel % capacity;//计算哈希索引
}

 /*
 功能：基于学生NAME的哈希函数，计算数据应存入的索引值 
 参数：data指向学生结构体的指针
         capacity 哈希表的容量
 返回值：计算出哈希索引
 */
unsigned int name_hash(const void *data, int capacity)
{
	const char *name = data;
	unsigned int hash = 0;//存储哈希索引值
	
	while(*name)
	{
		//字符串哈希函数 hash * 31 + 字符ASCII值
		hash = hash * 31 + *name++;
	}
	return hash % capacity;//
}




int main(void)
{
	HTABLE *ht = NULL;//ht指针指向哈希表的结构
	
	struct stu data;//存储客户录入的数据
	struct stu save;
	int i = 0;

	struct stu *find = NULL;
	int find_id = 104;
	char *find_name = "stu8";
	int find_tel = 10016;
	
	int ret = 0;

	ht = htable_create(sizeof(struct stu), 8);//创建爱你哈希表(每个节点的数据域大小，哈希表的容量)
	
	if(ht == NULL)
	{
		fprintf(stderr,"Failed!\n");
		return -1;
	}

	for(i = 0; i < 10; i++)//循环录入数据
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.tel = 10010 + i;
//(指向哈希表的指针,插入的数据,哈希函数,对比函数,插入模式)
		htable_insert(ht, &data, id_hash,  id_cmp, HTABLE_INSERT_REPLACE);
	//	htable_insert(ht, &data, id_hash,  id_cmp, HTABLE_INSERT_SAME);
		//htable_insert(ht, &data, id_hash,  id_cmp, HTABLE_INSERT_NOSAME);
	}


	printf("-------------------------------\n");
	htable_display(ht,print);//遍历(指向哈希表的指针，打印函数)

	find = htable_find(ht, &find_id, id_cmp, id_hash);
	if(find == NULL)
		printf("NOT!\n");
	else
		{
		printf("Find It!\n");
		print(find);
		printf("\n");
		}

#if 0
	ret = htable_delete(ht, &find_id, id_cmp, id_hash);
	if(ret != 0)
	printf("Not Delete!\n");
	else
	printf("Delete It!\n");
#else
	ret = htable_fetch(ht, &find_id, id_cmp, id_hash, &save);
	if(ret != 0)
	printf("NOT Fetch!\n");
	else
	{
		printf("Fetch!\n");
		print(&save);
		printf("\n");
	}
#endif



	printf("-------------------------------\n");



	htable_display(ht,print);//遍历

	htable_destroy(ht);//销毁(指向哈希表的指针)

	return 0;

}





