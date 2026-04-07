//日志输出模块
//从消息队列接收日志，令牌桶限流，写入文件

#include <stdio.h>
#include <time.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>
#include "msgq.h"
#include "tbf.h"

#define OUTPUT_FILE "/tmp/loghawk.log" //输出日志文件路径

// 定义三个输出文件
#define INFO_FILE  "/var/log/loghawk_info.log"
#define WARN_FILE  "/var/log/loghawk_warn.log"
#define ERROR_FILE "/var/log/loghawk_error.log"

// 备用路径（无权限时）
#define INFO_FILE_TMP  "/tmp/loghawk_info.log"
#define WARN_FILE_TMP  "/tmp/loghawk_warn.log"
#define ERROR_FILE_TMP "/tmp/loghawk_error.log"

#define RATE_LIMIT 1000 //限流速率：每秒最多处理1000条日志
#define BURST_SIZE 2000 //突发容量：最多允许2000条突发流量

static void log_msg(const char *msg)
{
    time_t now;                 // 当前时间戳
    struct tm *tm_info;         // 时间结构体 
    char time_str[32];          // 时间字符串缓冲区 
    
    time(&now);
    tm_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(stderr, "[%s] [INFO] %s\n", time_str, msg);
    fflush(stderr);
}

//输出函数接口
//连接消息队列，初始化令牌桶，循环接收消息并写入文件
int main(void)
{
	int msqid;//消息队列ID
	int info_fd, warn_fd, error_fd;
	int fd;//输出文件描述符
	char buf[MSG_MAX];//接收消息缓冲区
	int ret;//接收到的消息长度
	int tbf_id;//令牌桶ID

	log_msg("Outputr 启动");
	//[1] 打开输出文件
	// 打开主输出文件（备用）
    fd = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(fd < 0) {
        fd = open("/tmp/loghawk.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd < 0) {
            perror("open()");
            return -1;
        }
        log_msg("使用备用文件 : /tmp/loghawk.log");
    } else {
        log_msg("使用日志文件 : /tmp/loghawk.log");
    }
    
    // 打开INFO日志文件
    info_fd = open(INFO_FILE_TMP, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(info_fd < 0) {
        perror("open info");
        return -1;
    }
    
    // 打开WARN日志文件
    warn_fd = open(WARN_FILE_TMP, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(warn_fd < 0) {
        perror("open warn");
        return -1;
    }
    
    // 打开ERROR日志文件
    error_fd = open(ERROR_FILE_TMP, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(error_fd < 0) {
        perror("open error");
        return -1;
    }
    
    log_msg("三个日志文件已打开: info/warn/error");
	//[2] 初始化令牌桶
	tbf_id = tbf_init(RATE_LIMIT, BURST_SIZE);
	if(tbf_id < 0)
	{
		log_msg("令牌桶初始化失败");
		close(info_fd);
        close(warn_fd);
        close(error_fd);
		close(fd);
		return -2;
	}
	log_msg("令牌桶初始化成功,限流 : 1000条/秒");
	//[3] 连接消息队列
	msqid = msgq_create(MSG_KEY);
	if(msqid < 0)
	{
		log_msg("消息队列连接失败");
		tbf_destroy(tbf_id);//销毁令牌桶
		close(info_fd);
        close(warn_fd);
        close(error_fd);
		close(fd);
		return -3;
	}
	//[4] 循环接收消息队列并写入文件
	while(1)
	{
		ret = msgq_recv(msqid, buf, 1);//从消息队列接收消息(阻塞模式)
		if(ret > 0)//成功收到消息
		{
			tbf_fetch_token(tbf_id, 1);//获取令牌 令牌不足时等待
			// 根据日志级别写入不同文件
        if (strstr(buf, "\"level\":\"ERROR\"")) {
            write(error_fd, buf, strlen(buf));
            write(error_fd, "\n", 1);
        } else if (strstr(buf, "\"level\":\"WARN\"")) {
            write(warn_fd, buf, strlen(buf));
            write(warn_fd, "\n", 1);
        } else {
            write(info_fd, buf, strlen(buf));
            write(info_fd, "\n", 1);
		}
		}
	}	
	close(info_fd);
    close(warn_fd);
    close(error_fd);
	close(fd);//关闭文件
	return 0;
}










