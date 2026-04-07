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

//输出文件路径（使用/tmp目录，普通用户可写）
#define WARN_FILE   "/tmp/loghawk_warn.log"   //WARN日志输出文件
#define ERROR_FILE  "/tmp/loghawk_error.log"  //ERROR日志输出文件

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
	char buf[MSG_MAX];//接收消息缓冲区
	int ret;//接收到的消息长度
	int tbf_id;//令牌桶ID

	log_msg("Outputr 启动");
    
    // 打开WARN日志文件
    warn_fd = open(WARN_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(warn_fd < 0) {
        perror("open warn");
        return -1;
    }
    
    // 打开ERROR日志文件
    error_fd = open(ERROR_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(error_fd < 0) {
        perror("open error");
        close(warn_fd);   
        return -1;
    }
    
    log_msg("日志文件已打开: warn/error");
	//[2] 初始化令牌桶
	tbf_id = tbf_init(RATE_LIMIT, BURST_SIZE);
	if(tbf_id < 0)
	{
		log_msg("令牌桶初始化失败");
        	close(warn_fd);
        	close(error_fd);
		return -2;
	}
	log_msg("令牌桶初始化成功,限流 : 1000条/秒");
	//[3] 连接消息队列
	msqid = msgq_create(MSG_KEY);
	if(msqid < 0)
	{
		log_msg("消息队列连接失败");
		tbf_destroy(tbf_id);//销毁令牌桶
        	close(warn_fd);
        	close(error_fd);
		return -3;
	}
	//[4] 循环接收消息队列并写入文件
	while(1)
	{
	//先接收ERROR类型（类型=3）
        ret = msgq_recv(msqid, buf, 3);
        if(ret > 0)             //收到ERROR日志
        {
            tbf_fetch_token(tbf_id, 1);           //获取令牌（限流）
            write(error_fd, buf, strlen(buf));    //写入ERROR文件
            write(error_fd, "\n", 1);             //写入换行符
        }
        
        //接收WARN类型（类型=2）
        ret = msgq_recv(msqid, buf, 2);
        if(ret > 0)             //收到WARN日志
        {
            tbf_fetch_token(tbf_id, 1);           //获取令牌（限流）
            write(warn_fd, buf, strlen(buf));     //写入WARN文件
            write(warn_fd, "\n", 1);              //写入换行符
        }
        
        //没有消息时短暂休眠，避免CPU占用过高
        usleep(10000);          //10毫秒
    }
    
    //正常不会执行到这里
    close(warn_fd);
    close(error_fd);
	return 0;
}










