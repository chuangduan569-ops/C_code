#include "iot_gateway.h" 
#include "daemon.h"      
#include "log.h"        

/*
 * 函数名称: daemonize
 * 功能描述: 将当前进程变为守护进程(后台服务进程)
 *          实现标准守护进程化流程: fork → setsid → 二次fork → chdir → umask → 重定向
 * 文档要求: 实现标准守护进程化流程
 * 参数: 无
 * 返回值: 成功返回0，失败返回-1
 */
int daemonize(void)
{
    pid_t pid;      /* 进程ID变量，存储fork返回值 */
    int fd;         /* 文件描述符，用于打开/dev/null */
    
    /* [步骤1] 第一次fork，脱离终端控制 */
    pid = fork();   /* 创建子进程 */
    if (pid < 0) {  /* fork失败，返回负数 */
        LOG_ERROR("First fork failed: %s", strerror(errno));
        return -1;
    }
    if (pid > 0) {  /* 父进程执行此分支 */
        exit(0);    /* 父进程退出，子进程成为孤儿进程被init收养 */
    }
    /* 子进程继续执行 */
    
    /* [步骤2] 创建新会话，成为会话组长和进程组组长 */
    /* setsid()创建新会话，使子进程脱离原终端控制 */
    if (setsid() < 0) {
        LOG_ERROR("setsid failed: %s", strerror(errno));
        return -1;
    }
    
    /* [步骤3] 第二次fork，确保不会意外获得终端 */
    /* 再次fork保证守护进程不是会话首进程，防止获取控制终端 */
    pid = fork();
    if (pid < 0) {
        LOG_ERROR("Second fork failed: %s", strerror(errno));
        return -1;
    }
    if (pid > 0) {
        exit(0);    /* 第一次fork的子进程退出 */
    }
    /* 第二次fork的孙子进程继续执行，成为最终的守护进程 */
    
    /* [步骤4] 改变工作目录到根目录 */
    /* 防止守护进程占用某个目录导致无法卸载文件系统 */
    if (chdir("/") < 0) {
        LOG_ERROR("chdir failed: %s", strerror(errno));
        return -1;
    }
    
    /* [步骤5] 设置文件掩码 */
    /* 清除文件模式创建掩码，确保守护进程创建文件时有完整的权限 */
    umask(0);
    
    /* [步骤6] 重定向标准输入、输出、错误到/dev/null */
    /* 守护进程不应与终端交互，关闭标准IO流 */
    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        LOG_ERROR("open /dev/null failed: %s", strerror(errno));
        return -1;
    }
    
    /* dup2()函数: 将标准文件描述符复制到新打开的文件描述符 */
    dup2(fd, 0);   /* 标准输入(0)重定向到/dev/null */
    dup2(fd, 1);  /* 标准输出(1)重定向到/dev/null */
    dup2(fd, 2);  /* 标准错误(2)重定向到/dev/null */
    
    /* 关闭不再需要的文件描述符 */
    if (fd > 2) {
        close(fd);
    }
    
    LOG_INFO("Daemonized successfully");
    return 0;
}

/*
 * 函数名称: create_pid_file
 * 功能描述: 创建PID文件并加锁，防止程序多实例运行
 *          将当前进程的PID写入文件，使用O_EXCL标志确保唯一性
 * 文档要求: 生成PID文件并加锁，防止多实例运行
 * 参数: 无
 * 返回值: 成功返回0，失败返回-1
 */
int create_pid_file(void)
{
    int fd;                 /* 文件描述符 */
    char pid_str[16];       /* 存储PID字符串的缓冲区 */
    ssize_t written;        /* 写入字节数 */
    
    /*
     * O_WRONLY: 只写方式打开
     * O_CREAT:  文件不存在则创建
     * O_EXCL:   如果文件已存在则返回错误(实现互斥)
     * 0644:     文件权限: 所有者可读写，组用户和其他用户只读
     */
    fd = open(PID_FILE_PATH, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        /* 文件已存在，说明已有实例在运行 */
        LOG_ERROR("Failed to create PID file, daemon may already be running");
        return -1;
    }
    
    /* 将当前进程ID格式化为字符串 */
    snprintf(pid_str, sizeof(pid_str), "%d\n", getpid());
    
    /* 将PID字符串写入文件 */
    written = write(fd, pid_str, strlen(pid_str));
    if (written < 0) {
        LOG_ERROR("Failed to write PID to file: %s", strerror(errno));
        close(fd);
        return -1;
    }
    
    /* 关闭文件描述符 */
    close(fd);
    
    LOG_INFO("PID file created: %s, PID=%d", PID_FILE_PATH, getpid());
    return 0;
}

/*
 * 函数名称: remove_pid_file
 * 功能描述: 删除PID文件，程序退出时调用
 *          删除后允许下次启动新的实例
 * 参数: 无
 * 返回值: 无
 */
void remove_pid_file(void)
{
    /* unlink()删除文件，如果文件不存在则静默失败 */
    if (unlink(PID_FILE_PATH) == 0) {
        LOG_INFO("PID file removed: %s", PID_FILE_PATH);
    } else {
        LOG_WARN("Failed to remove PID file: %s", strerror(errno));
    }
}
