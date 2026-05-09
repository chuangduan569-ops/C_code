#include "password_auth.h"

/*
 * 功能：用户认证（最多尝试3次密码）
 * 参数：无
 * 返回值：0表示登录成功，-1表示失败
 */
int authenticate_user(void)
{
    char name[NAMESIZE] = {0};     // 用来存储录入的用户名
    char *pwd = NULL;               // pwd指针指向输入密码的首地址
    struct spwd *tp = NULL;         // tp指针指向从/etc/shadow文件中读取的信息
    char *cp = NULL;                // cp指针指向输入的密码加密以后的密码字符串
    int attempts = 0;               // 记录当前尝试次数
    
    // [1] 输入登录的用户名（只输入一次）
    printf("请输入用户名 : ");
    fgets(name, NAMESIZE, stdin);           // 从标准输入流中读取用户名
    *strchr(name, '\n') = '\0';             // 把name数组中'\n'替换为'\0'
    
    // [2] 循环输入密码，最多尝试 MAX_ATTEMPTS 次
    for(attempts = 1; attempts <= MAX_ATTEMPTS; attempts++)
    {
        printf("\n--- 第 %d 次尝试 ---\n", attempts);
        
        // 输入密码
        pwd = getpass("请输入密码 : ");      // 输入密码（不回显）
        if(pwd == NULL)                     // 判断获取密码是否失败
        {
            perror("getpass()");            // 打印错误信息
            return -1;                      // 输入密码失败，结束程序
        }
        
        // [3] 读真正的密码
        tp = getspnam(name);                // 根据用户名从shadow文件获取用户信息
        if(tp == NULL)                      // 判断获取用户信息是否失败
        {
            fprintf(stderr, "获取shadow文件中的用户信息失败!\n");
            return -1;                      // 获取用户信息失败，结束程序
        }
        
        // [4] 将输入的密码进行加密
        cp = crypt(pwd, tp->sp_pwdp);       // 把输入的密码根据加密算法和盐值进行加密
        if(cp == NULL)                      // 判断加密是否失败
        {
            perror("crypt()");              // 打印错误信息
            return -1;                      // 加密失败，结束程序
        }
        
        // [5] 对比密码
        if(!strcmp(tp->sp_pwdp, cp))        // 判断密码是否相同
        {
            printf("\n恭喜! 登陆成功!\n");
            return 0;                       // 登录成功，返回0
        }
        else
        {
            printf("对不起, 密码错误!");
            
            // 显示剩余尝试次数
            if(attempts < MAX_ATTEMPTS) {
                printf(" 还剩 %d 次机会\n", MAX_ATTEMPTS - attempts);
            } else {
                printf(" 已用完 %d 次机会, 登录失败\n", MAX_ATTEMPTS);
            }
        }
    }
    
    // 所有尝试都失败
    return -1;
}
