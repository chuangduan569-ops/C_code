#ifndef __PASSWORD_AUTH_H
#define __PASSWORD_AUTH_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <crypt.h>

#define NAMESIZE 32      // 用户名字符串最大长度
#define MAX_ATTEMPTS 3   // 最大密码尝试次数（3次）

// 函数声明
extern int authenticate_user(void);  // 用户认证函数（最多尝试3次）

#endif
