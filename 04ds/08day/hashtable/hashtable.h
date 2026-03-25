#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#define HTABLE_DEFAULT_CAPACITY 10//定义哈希表的默认容量
#define HTABLE_INSERT_REPLACE 0//覆盖已存在的标识
#define HTABLE_INSERT_NOSAVE 1//不允许重复数据的标识
#define HTABLE_INSERT_SAVE 2//允许重复的标识


typedef void HTABLE;//定义哈希表结构(隐藏底层结构体的实现)
typedef void (*htable_op)(const void *);//定义函数指针类型(用于哈希表的打印)
typedef int (*htable_cmp)(const void *, const void*);//定义函数指针类型(用于哈希表的比较)
typedef unsigned int (*htable_hash)(const void *, int);//定义函数指针类型(用于计算哈希函数的索引)


HTABLE *htable_create(int size, int capacity);

int htable_insert(HTABLE *handler, const void *data, htable_hash hash, htable_cmp cmp, int mode);

void htable_display(HTABLE *handler, htable_op op);

void htable_destroy(HTABLE *handler);

void *htable_find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash);

int htable_delete(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash);

int htable_fetch(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash, void *save);





#endif
