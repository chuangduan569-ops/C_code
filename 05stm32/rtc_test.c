/**
 * @file rtc_test.c
 * @brief Linux下测试RTC时间转换函数
 * @compile gcc -o rtc_test rtc_test.c
 * @run ./rtc_test
 */

#include <stdio.h>
#include <stdint.h>

/* 时间结构体，与STM32保持一致 */
struct time_st
{
    int Y;  // 年份
    int M;  // 月份
    int D;  // 日期
    int W;  // 星期
    int h;  // 小时
    int m;  // 分钟
    int s;  // 秒数
};

/* 闰年判断宏 */
#define IS_LEAP_YEAR(y) (((y) % 4 == 0 && (y) % 100 != 0) || ((y) % 400 == 0))

/* 每月天数表（非闰年） */
static const int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* 模拟RTC计数器（用于Linux测试） */
static uint32_t mock_rtc_counter = 0;

/* 模拟STM32的RTC操作函数 */
void RTC_SetCounter(uint32_t cnt)
{
    mock_rtc_counter = cnt;
    printf("[模拟RTC] 时间戳已设置为: %u\n", cnt);
}

uint32_t RTC_GetCounter(void)
{
    return mock_rtc_counter;
}

void RTC_WaitForLastTask(void)
{
    /* Linux测试下什么都不做 */
}

/**
 * @brief 设置时间：将日历时间转换为时间戳并写入RTC
 */
void set_time(struct time_st *t)
{
    int i;
    int total_days = 0;
    uint32_t timestamp;
    
    /* 1. 计算1970年到目标年份前一年的总天数 */
    for (i = 1970; i < t->Y; i++) {
        total_days += IS_LEAP_YEAR(i) ? 366 : 365;
    }
    
    /* 2. 加上当年已过月份的天数 */
    for (i = 0; i < t->M - 1; i++) {
        total_days += month_days[i];
        if (i == 1 && IS_LEAP_YEAR(t->Y)) {
            total_days += 1;
        }
    }
    
    /* 3. 加上当月已过天数 */
    total_days += t->D - 1;
    
    /* 4. 计算总秒数并写入RTC */
    timestamp = total_days * 86400UL + t->h * 3600UL + t->m * 60UL + t->s;
    RTC_SetCounter(timestamp);
    RTC_WaitForLastTask();
}

/**
 * @brief 转换时间：从RTC读取时间戳并转换为日历时间
 */
void conv_time(struct time_st *t)
{
    uint32_t timestamp;
    uint32_t days;
    uint32_t seconds_today;
    int year;
    int month;
    uint32_t total_days;
    
    /* 1. 从RTC读取当前时间戳 */
    timestamp = RTC_GetCounter();
    
    /* 2. 分解为天数和当天秒数 */
    days = timestamp / 86400UL;
    seconds_today = timestamp % 86400UL;
    
    /* 3. 计算年份 */
    year = 1970;
    while (1) {
        int days_in_year = IS_LEAP_YEAR(year) ? 366 : 365;
        if (days >= (uint32_t)days_in_year) {
            days -= days_in_year;
            year++;
        } else {
            break;
        }
    }
    t->Y = year;
    
    /* 4. 计算月份和日期 */
    month = 0;
    while (1) {
        int days_in_month = month_days[month];
        if (month == 1 && IS_LEAP_YEAR(year)) {
            days_in_month = 29;
        }
        if (days >= (uint32_t)days_in_month) {
            days -= days_in_month;
            month++;
        } else {
            break;
        }
    }
    t->M = month + 1;
    t->D = (int)days + 1;
    
    /* 5. 计算时分秒 */
    t->h = (int)(seconds_today / 3600);
    seconds_today %= 3600;
    t->m = (int)(seconds_today / 60);
    t->s = (int)(seconds_today % 60);
    
    /* 6. 计算星期（1970-01-01是星期四） */
    total_days = timestamp / 86400UL;
    t->W = (int)((4 + total_days) % 7);
    if (t->W == 0) t->W = 7;
}

/**
 * @brief 打印时间
 */
void print_time(struct time_st *t, const char *title)
{
    const char *week[] = {"", "一", "二", "三", "四", "五", "六", "日"};
    printf("%s: %04d-%02d-%02d %02d:%02d:%02d 星期%s\n", 
           title, t->Y, t->M, t->D, t->h, t->m, t->s, week[t->W]);
}

/**
 * @brief 测试用例
 */
int main(void)
{
    struct time_st t1, t2;
    
    printf("========== RTC时间转换函数测试 ==========\n\n");
    
    /* 测试1：基本时间转换 */
    printf("[测试1] 基本时间转换\n");
    t1.Y = 2024; t1.M = 3; t1.D = 9; t1.h = 14; t1.m = 30; t1.s = 45;
    print_time(&t1, "设置时间");
    set_time(&t1);
    conv_time(&t2);
    print_time(&t2, "读取时间");
    printf("\n");
    
    /* 测试2：闰年测试（2024-02-29） */
    printf("[测试2] 闰年测试\n");
    t1.Y = 2024; t1.M = 2; t1.D = 29; t1.h = 12; t1.m = 0; t1.s = 0;
    print_time(&t1, "设置时间");
    set_time(&t1);
    conv_time(&t2);
    print_time(&t2, "读取时间");
    printf("\n");
    
    /* 测试3：非闰年2月28日 */
    printf("[测试3] 非闰年测试\n");
    t1.Y = 2023; t1.M = 2; t1.D = 28; t1.h = 23; t1.m = 59; t1.s = 59;
    print_time(&t1, "设置时间");
    set_time(&t1);
    conv_time(&t2);
    print_time(&t2, "读取时间");
    printf("\n");
    
    /* 测试4：跨年测试 */
    printf("[测试4] 跨年测试\n");
    t1.Y = 2023; t1.M = 12; t1.D = 31; t1.h = 23; t1.m = 59; t1.s = 55;
    print_time(&t1, "设置时间");
    set_time(&t1);
    
    /* 模拟时间走动5秒 */
    printf("等待5秒...\n");
    for (int i = 1; i <= 5; i++) {
        mock_rtc_counter += 1;
        conv_time(&t2);
        printf("第%d秒: %04d-%02d-%02d %02d:%02d:%02d 星期%d\n", 
               i, t2.Y, t2.M, t2.D, t2.h, t2.m, t2.s, t2.W);
    }
    printf("\n");
    
    /* 测试5：Unix纪元时间 */
    printf("[测试5] Unix纪元时间\n");
    t1.Y = 1970; t1.M = 1; t1.D = 1; t1.h = 0; t1.m = 0; t1.s = 0;
    print_time(&t1, "设置时间");
    set_time(&t1);
    conv_time(&t2);
    print_time(&t2, "读取时间");
    printf("\n");
    
    /* 测试6：回环测试 */
    printf("[测试6] 回环测试\n");
    t1.Y = 2024; t1.M = 6; t1.D = 1; t1.h = 8; t1.m = 30; t1.s = 0;
    print_time(&t1, "原始时间");
    set_time(&t1);
    conv_time(&t2);
    print_time(&t2, "转换结果");
    
    if (t1.Y == t2.Y && t1.M == t2.M && t1.D == t2.D &&
        t1.h == t2.h && t1.m == t2.m && t1.s == t2.s) {
        printf("✓ 测试通过：时间一致\n");
    } else {
        printf("✗ 测试失败：时间不一致\n");
    }
    
    printf("\n========== 测试完成 ==========\n");
    return 0;
}
