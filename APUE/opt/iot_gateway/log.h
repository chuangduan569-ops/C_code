#ifndef __LOG_H  
#define __LOG_H

#define DEFAULT_LOG_PATH "./log" //默认日志文件存储目录，当前目录下的log文件夹

//日志级别枚举
typedef enum {
    LOG_DEBUG = 0,  //调试信息: 开发阶段使用，生产环境可关闭，级别0最低 
    LOG_INFO = 1,   //一般信息: 正常业务流程日志，如启动、停止、连接等 
    LOG_WARN = 2,   //警告信息: 异常但可恢复的情况，如重试、超时等 
    LOG_ERROR = 3,  //错误信息: 功能异常但服务继续运行，如连接失败等 
    LOG_FATAL = 4   //致命错误: 服务即将退出，级别4最高 
} log_level_t;      //定义日志级别类型为log_level_t 

/*
 功能   初始化日志系统
 参数 log_path 日志文件存储目录路径，如"./log"或"/var/log"
	level - 日志输出级别
 返回值   无
*/
void log_init(const char *log_path, log_level_t level);

/*
功能 写入一条日志记录
参数 level - 本条日志的级别，决定是否输出
	format - 格式化字符串，如"Value=%d"
	... - 可变参数，对应格式化字符串中的占位符
返回值    无
*/
void log_write(log_level_t level, const char *format, ...);
/*
功能 关闭日志系统
参数 返回值 无
*/
void log_close(void);

//日志宏定义
//简化日志调用，自动传入级别和格式化参数，使用变参宏##__VA_ARGS__ *
#define LOG_DEBUG(fmt, ...) log_write(LOG_DEBUG, fmt, ##__VA_ARGS__) //DEBUG级别日志宏
#define LOG_INFO(fmt, ...)  log_write(LOG_INFO,  fmt, ##__VA_ARGS__)  //INFO级别日志宏
#define LOG_WARN(fmt, ...)  log_write(LOG_WARN,  fmt, ##__VA_ARGS__)  //WARN级别日志宏 
#define LOG_ERROR(fmt, ...) log_write(LOG_ERROR, fmt, ##__VA_ARGS__)  //ERROR级别日志宏 
#define LOG_FATAL(fmt, ...) log_write(LOG_FATAL, fmt, ##__VA_ARGS__)  //FATAL级别日志宏 


#endif






