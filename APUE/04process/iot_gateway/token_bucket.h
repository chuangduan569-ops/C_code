#ifndef __TOKEN_BUCKET_H
#define __TOKEN_BUCKET_H

/*==================== 令牌桶配置常量 ====================*/
#define TBF_MAX 1024                /* 最大支持1024个令牌桶 */
#define DEFAULT_TOKEN_RATE 1000     /* 默认令牌生成速率: 1000个/秒 */
#define DEFAULT_TOKEN_BURST 2000    /* 默认令牌桶容量: 2000个 */


int tbf_init(int cps, int burst);
/*
功能 : 初始化令牌桶

参数 : cps 客户指定的速率

burst 客户指定的容量

返回值 : 成功返回令牌桶库的下标(>=0);失败返回负数(<0)
*/
int tbf_fetch_token(int td, int n);
/*
功能 : 取令牌

参数 : td 令牌桶库的描述符(令牌桶库的表示)

n 客户要取走的令牌数

返回值 : 成功取走的令牌数
*/
int tbf_destroy(int td);
/*
功能 : 销毁相应的令牌桶

参数 : td 令牌桶库的描述符(令牌桶库的表示)

返回值 : 成功返回0;失败返回负数
*/

int tbf_get_tokens(int td);  // 添加：获取当前令牌数

#endif
