#ifndef __ANYTIMER_H
#define __ANYTIMER_H

//回调函数类型:参数为void*,用户可以传递任意类型的数据
typedef void (*handler_t)(void *);//定义函数指针类型，指向返回值为void、参数为void*的函数

extern int anytimer_init(int sec, handler_t func, void *arg);

/*
功能 : 初始化闹钟
参数 : sec 定时秒数
		func 定时后调用的函数
		arg 传递给 func 的参数 (可以为NULL)
返回值 : 成功返回闹钟的ID， 失败返回负数
*/

extern int anytimer_destroy(int id);
/*
功能 : 销毁闹钟 
参数 : id 闹钟ID (由 anytimer_init 返回)
返回值 : 成功返回0, 失败返回负数

*/




#endif
