#ifndef __TBF_H
#define __TBF_H

extern int tbf_init(int cps, int burst);
/*
功能 : 初始化令牌桶
参数 : cps  用户指定的速率
		burst 用户指定的令牌数上限
返回值 : 令牌桶的下标(因为我们实现的库是支持1024个令牌桶)
*/

extern int tbf_fetch_token(int td, int n);
/*
功能 : 取令牌
参数 : td 令牌桶的描述符(令牌桶的下标)
		n 要取走的令牌数
返回值 : 成功取走的令牌数
*/

extern int tbf_destroy(int td);
/*
功能 : 销毁令牌桶
参数 : td  令牌桶的描述符(令牌桶的下标)
返回值 : 成功返回0； 失败返回-1
*/




#endif
