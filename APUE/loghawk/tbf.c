#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "tbf.h"

#define TBFMAX 1024  /* 定义令牌桶库最大容量，最多支持1024个令牌桶 */

/* 
 * 令牌桶结构体
 * token: 当前令牌桶中的令牌数
 * cps:   每秒生成的令牌数（速率）
 * burst: 桶的最大容量（突发流量上限）
 */
typedef struct tbf_st
{
    int token;   /* 当前令牌桶的令牌数 */
    int cps;     /* 每秒生成令牌数（速率） */
    int burst;   /* 桶的最大容量 */
} tbf_t;

static tbf_t *tbf_libs[TBFMAX];  /* 令牌桶库数组，存储所有令牌桶的指针 */
static int initd = 0;            /* 模块是否已初始化（0=未启动，1=已启动） */


/*
 * 函数名称：get_tbf_pos
 * 功能描述：循环遍历令牌桶库，找到当前可用的最小令牌桶标记
 * 参数说明：无
 * 返回值：
 *     成功返回可用的下标（0 ~ TBFMAX-1）
 *     失败返回-1（令牌桶库已满）
 */
static int get_tbf_pos(void)
{
    int i = 0;  /* 循环变量 */

    for(i = 0; i < TBFMAX; i++)  /* 循环遍历整个令牌桶库 */
    {
        if(tbf_libs[i] == NULL)  /* 判断当前位置是否可用（未被占用） */
            return i;            /* 返回当前可用位置的下标 */
    }
    return -1;  /* 遍历完都没有找到可用位置，返回-1表示已满 */
}


/*
 * 函数名称：alarm_handler
 * 功能描述：SIGALRM信号的行为函数
 *           每秒被调用一次，给所有令牌桶添加令牌
 * 参数说明：none - 信号编号（本函数不使用）
 * 返回值：无
 */
static void alarm_handler(int none)
{
    int i = 0;  /* 循环变量 */

    alarm(1);   /* 重新设置1秒的闹钟，实现周期性调用 */

    for(i = 0; i < TBFMAX; i++)  /* 循环遍历整个令牌桶库 */
    {
        if(tbf_libs[i] != NULL)  /* 判断当前令牌桶是否正在使用 */
        {
            /* 给当前令牌桶添加cps个令牌（每秒生成速率） */
            tbf_libs[i]->token += tbf_libs[i]->cps;

            /* 判断当前令牌桶中令牌数是否超过该令牌桶指定的上限 */
            if(tbf_libs[i]->token > tbf_libs[i]->burst)
            {
                /* 超过上限，设置为上限值 */
                tbf_libs[i]->token = tbf_libs[i]->burst;
            }
        }
    }
}


/*
 * 函数名称：module_load
 * 功能描述：启动令牌桶模块
 *           设置SIGALRM信号的处理函数，并启动定时器
 * 参数说明：无
 * 返回值：无
 */
static void module_load(void)
{
    signal(SIGALRM, alarm_handler);  /* 给SIGALRM信号设置行为函数 */
    alarm(1);                        /* 设置1秒的闹钟，触发第一次SIGALRM */
}


/*
 * 函数名称：tbf_init
 * 功能描述：初始化一个令牌桶
 * 参数说明：
 *     cps   - 客户指定的速率（每秒生成的令牌数）
 *     burst - 客户指定的容量（桶的最大容量）
 * 返回值：
 *     成功返回令牌桶库的下标(>=0)
 *     失败返回负数(<0)
 *     -1 : cps或burst参数无效（<=0）
 *     -2 : 令牌桶库已满（最多1024个桶）
 *     -3 : 内存分配失败
 */
int tbf_init(int cps, int burst)
{
    int pos = 0;  /* pos变量存储查询到的当前可用最小的令牌桶库的标记 */

    /* 判断形参是否没有意义（速率和容量必须为正数） */
    if(cps <= 0 || burst <= 0)
        return -1;  /* 由于形参没有意义，结束函数，并且返回-1 */

    /* 判断令牌桶库是否没有工作（首次调用需要初始化） */
    if(!initd)
    {
        module_load();  /* 启动令牌桶模块（设置定时器） */
        initd = 1;      /* 切换状态为工作状态 */
    }

    /* 获取当前令牌桶库中可用的最小的令牌桶标记 */
    pos = get_tbf_pos();
    if(pos < 0)  /* 判断是否没有找到可用位置 */
        return -2;  /* 由于没有找到位置，结束函数，并且返回-2 */

    /* 为令牌桶开辟空间（动态分配内存） */
    tbf_libs[pos] = malloc(sizeof(tbf_t));
    if(tbf_libs[pos] == NULL)  /* 判断为令牌桶开辟空间是否失败 */
        return -3;  /* 由于为令牌桶开辟空间失败，结束函数，并且返回-3 */

    /* 存储客户指定的速率 */
    tbf_libs[pos]->cps = cps;
    /* 存储客户指定的上限（桶容量） */
    tbf_libs[pos]->burst = burst;
    /* 初始化令牌桶中的令牌数为0（一开始没有令牌） */
    tbf_libs[pos]->token = 0;

    /* 返回当前初始化成功的令牌桶下标（令牌桶ID） */
    return pos;
}


/*
 * 函数名称：tbf_fetch_token
 * 功能描述：从令牌桶获取令牌（阻塞版本）
 *           如果令牌不足，会阻塞等待直到有令牌可用
 * 参数说明：
 *     td - 令牌桶库的描述符（由tbf_init返回的令牌桶ID）
 *     n  - 客户要取走的令牌数
 * 返回值：
 *     成功返回实际取走的令牌数（可能小于n）
 *     失败返回负数
 *     -1 : td或n参数无效
 *     -2 : td对应的令牌桶不存在
 */
int tbf_fetch_token(int td, int n)
{
    int fetch_token = 0;  /* 存储取走的令牌数 */

    /* 判断参数是否没有意义 */
    if(td < 0 || td >= TBFMAX || n <= 0)
        return -1;  /* 由于参数没有意义，结束函数，并且返回-1 */

    /* 判断td标记的令牌桶是否不存在 */
    if(tbf_libs[td] == NULL)
        return -2;  /* 由于td标记的令牌桶不存在，结束函数，并且返回-2 */

    /* 等待td令牌桶内积攒令牌（如果令牌数为0，则阻塞等待） */
    while(tbf_libs[td]->token <= 0)
        pause();  /* 等待信号（定时器信号会唤醒pause()） */

    /* 判断td令牌桶内的令牌是否充足（大于等于客户需要的数量） */
    if(tbf_libs[td]->token >= n)
        fetch_token = n;  /* 取走客户指定的令牌个数 */
    else
        fetch_token = tbf_libs[td]->token;  /* 有多少，取多少 */

    /* 更新td令牌桶内的令牌数（减去取走的令牌） */
    tbf_libs[td]->token -= fetch_token;

    /* 返回成功取走的令牌数 */
    return fetch_token;
}



/*
 * 函数名称：tbf_get_tokens
 * 功能描述：获取当前令牌桶中的令牌数（用于调试和监控）
 * 参数说明：
 *     td - 令牌桶库的描述符（由tbf_init返回的令牌桶ID）
 * 返回值：
 *     成功返回当前令牌桶中的令牌数
 *     失败返回-1
 */
int tbf_get_tokens(int td)
{
    /* 判断参数是否没有意义 */
    if(td < 0 || td >= TBFMAX)
        return -1;  /* 参数无效，返回-1 */

    /* 判断td标记的令牌桶是否不存在 */
    if(tbf_libs[td] == NULL)
        return -1;  /* 令牌桶不存在，返回-1 */

    /* 返回当前令牌桶中的令牌数 */
    return tbf_libs[td]->token;
}


/*
 * 函数名称：tbf_destroy
 * 功能描述：销毁相应的令牌桶，释放内存
 * 参数说明：
 *     td - 令牌桶库的描述符（由tbf_init返回的令牌桶ID）
 * 返回值：
 *     成功返回0
 *     失败返回负数
 *     -1 : td参数无效
 *     -2 : td对应的令牌桶不存在
 */
int tbf_destroy(int td)
{
    /* 判断参数是否没有意义 */
    if(td < 0 || td >= TBFMAX)
        return -1;  /* 由于参数没有意义，结束函数，并且返回-1 */

    /* 判断td标记的令牌桶是否不存在 */
    if(tbf_libs[td] == NULL)
        return -2;  /* 由于td标记的令牌桶不存在，结束函数，并且返回-2 */

    /* 释放td令牌桶占用的内存 */
    free(tbf_libs[td]);
    /* 将指针设置为NULL，避免出现野指针 */
    tbf_libs[td] = NULL;

    /* 释放成功，返回0 */
    return 0;
}
