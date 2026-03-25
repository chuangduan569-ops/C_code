#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#define HTABLE_DEFAULT_CAPACITY 10//定义哈希表的默认容量
#define HTABLE_INSERT_REPLACE 0//覆盖已有数据的标识
#define HTABLE_INSERT_NOSAME 1//不允许重复数据的标识
#define HTABLE_INSERT_SAME 2//允许重复数据的标识

typedef void(*htable_op)(const void *);//定义函数指针类型(用于哈希表的打印)
	
typedef int(*htable_cmp)(const void *, const void *);//定义函数指针类型(用于哈希表数据的比较)

typedef unsigned int(*htable_hash)(const void *, int);//定义函数指针类型(用于计算哈希索引)

typedef void HTABLE;//定义哈希表结构(隐藏底层结构体的实现)

/*
功能：创建哈希表
参数：size 每个数据节点的数据域大小
		capacity 哈希表的容量
返回值：成功返回哈希表结构的地址；失败返回NULL；
*/
HTABLE *htable_create(int size, int capacity);

/*
功能：往哈希表中插入数据
参数：handler指向哈希表结构的指针
		data 指向要插入的数据
		hash 存储要使用的哈希函数
		cmp 存储比较函数
		mode 存储插入模式
返回值：成功返回0,失败返回<0
*/
int htable_insert(HTABLE *handler,const void *data,htable_hash hash,htable_cmp cmp, int mode);

/*
 功能：遍历哈希表
参数：handler 指向哈希表结构的指针
		op 存储打印的函数

返回值：空
 */

void htable_display(HTABLE *handler, htable_op op);

/*
功能 : 销毁哈希表
参数 : handler 指向哈希表结构的指针
返回值 : 空
*/  
void htable_destroy(HTABLE *handler);


/*  
功能 : 查找数据
参数 : handler 指向哈希表结构的指针
        find_data 要查找的数据
        cmp 存储比较函数 
        hash 存储哈希函数
返回值 : 成功返回查找到的数据节点的数据域地址;失败返回NULL
*/
void *htable_find(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash);

/*
功能 : 删除数据
参数 : handler 指向哈希表结构的指针
        find_data 要删除的数据
        cmp 存储比较函数
        hash 存储哈希函数
返回值 : 成功返回0;失败返回<0
*/
int htable_delete(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash);

/*
功能 : 取出数据
参数 : handler 指向哈希表结构的指针
        find_data 要取出的数据
        cmp 存储比较函数
        hash 存储哈希函数
返回值 : 成功返回0;失败返回<0
*/

int htable_fetch(HTABLE *handler, const void *find_data, htable_cmp cmp, htable_hash hash, void *save);









#endif
