#ifndef __ALARM_SCHEDULER_H
#define __ALARM_SCHEDULER_H

//回调函数类型:参数为void*,用户可以传递任意类型的数据
typedef void (*HANDLER)(void *);//定义函数指针类型

extern int anytimer_init(int second, HANDLER handler, void *arg);
/*
功能 : 初始化闹钟库
参数 : seconds 定时秒数
		handler 定时后调用的函数(要干的活)
		arg 传递给 func 的参数 (可以为NULL)
返回值 : 成功返回闹钟库的下标ID， 失败返回负数
*/

extern int anytimer_destroy(int timer_id);

/*
功能 : 销毁指定标识的闹钟 
参数 : timer_id 闹钟库下标的ID (由 anytimer_init 返回)
返回值 : 成功返回0, 失败返回负数
*/


#endif



