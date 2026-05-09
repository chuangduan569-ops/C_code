#include "iot_gateway.h"
#include "log.h"

static FILE *g_log_file = NULL; //日志文件指针
static log_level_t g_log_level = LOG_INFO; //当前日志输出级别，默认INFO，低于此级别不输出
static char g_log_path[256]; //日志文件存储目录路径，如"./log"
static char g_current_date[16]; //当前日期字符串 用于滚动检测
static pthread_mutex_t g_log_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥量

static void get_date_str(char *buf, int size)
{
	time_t now;//当前时间戳
	struct tm *tm_info;//时间结构体指针 存储分解后的年/月/日/时/分/秒
	time(&now);//time()函数获取当前时间戳，参数为NULL或指针
	tm_info = localtime(&now);//localtime()将时间戳转换为本地时间结构体
	strftime(buf, size, "%Y%m%d", tm_info);
	//strftime()格式化时间为字符串，%Y=年(4位)，%m=月(2位)，%d=日(2位)
}

// log.c - 修复 rotate_log_file 函数
static void rotate_log_file(void)
{
    char new_date[16];      // 添加：新日期字符串缓冲区
    char filename[512];     // 添加：文件名缓冲区
    
    // 获取当前日期
    get_date_str(new_date, sizeof(new_date));
    
    // 检查日期是否变更
    if (strcmp(g_current_date, new_date) != 0)
    {
        // 关闭旧文件
        if (g_log_file && g_log_file != stderr)
        {
            fclose(g_log_file);
        }
        
        // 更新日期并创建新文件
        strcpy(g_current_date, new_date);
        snprintf(filename, sizeof(filename), "%s/iot_gateway_%s.log", 
                 g_log_path, g_current_date);
        
        g_log_file = fopen(filename, "a");
        if (g_log_file == NULL)
        {
            g_log_file = stderr;  // 降级到stderr
        }
    }
}


static const char* level_to_string(log_level_t level)
{
	//switch语句根据级别返回对应的字符串
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";   
        case LOG_WARN:  return "WARN";  
        case LOG_ERROR: return "ERROR";
        case LOG_FATAL: return "FATAL"; 
        default:        return "UNKNOWN"; 
    }
}



void log_init(const char *log_path, log_level_t level)
{
	char filename[512];//日志文件完整路径缓冲区
	pthread_mutex_lock(&g_log_mutex);//加锁
	//保存日志路径和级别到全局变量
	strncpy(g_log_path, log_path, sizeof(g_log_path) - 1);//复制日志路径
	g_log_path[sizeof(g_log_path) - 1] = '\0';//确保字符串以'\0'结尾
	g_log_level = level; //设置日志输出级别
//创建日志目录(如果不存在)
//mkdir()系统调用创建目录，0755权限: 所有者可读写执行(7)，组用户和其他用户可读执行
	mkdir(log_path, 0755); //如果目录已存在，则静默失败
//获取当前日期，用于构造日志文件名
	get_date_str(g_current_date, sizeof(g_current_date));
//构造日志文件路径
	snprintf(filename, sizeof(filename), "%s/iot_gateway_%s.log", \
			log_path, g_current_date);
//以追加模式打开日志文件
	g_log_file = fopen(filename, "a");
	if(g_log_file == NULL)//如果打开失败，降级使用stderr输出到终端
		g_log_file = stderr;//stderr是标准错误输出，不缓冲，直接输出到终端
	pthread_mutex_unlock(&g_log_mutex);//解锁
	//输出初始化完成日志
	LOG_INFO("Log system initialized, level=%d, path=%s", level, log_path);  //使用日志宏输出
}

void log_write(log_level_t level, const char *format, ...)
{
	va_list args; //可变参数列表类型，用于遍历可变参数
	time_t now; //当前时间戳(秒数)
	struct tm *tm_info;//时间结构体指针
	char time_str[32];//格式化后的时间字符串
	const char *level_str;//日志级别的字符串表示

	//级别过滤: 如果当前日志级别高于本条日志级别，则不输出 
	if(level < g_log_level)
		return ;//级别太低，不输出
	pthread_mutex_lock(&g_log_mutex);
	//获取当前时间
	time(&now);
	tm_info = localtime(&now);//转换为本地时间结构体
	//strftime格式化时间字符串
	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
	//获取日志级别字符串
	level_str = level_to_string(level);
	//检查并执行日志滚动(按天
	rotate_log_file();
	//写入日志头部: [时间] [级别] 
	fprintf(g_log_file, "[%s] [%s] ", time_str, level_str);
	//写入用户格式化的消息内容
	va_start(args, format);//初始化可变参数列表，指向format后面的第一个参数
	vfprintf(g_log_file, format, args);//vfprintf格式化输出到文件
	va_end(args);//清理可变参数列表
	fprintf(g_log_file, "\n");//输出换行符
	fflush(g_log_file);//刷新缓冲区
	//ERROR和FATAL级别同时输出到stderr，便于调试和监控
	if(level >= LOG_ERROR && g_log_file != stderr)
	{
		fprintf(stderr, "[%s] [%s] ", time_str, level_str);
		//输出到标准错误
		va_start(args, format);//重新初始化可变参数列表
		vfprintf(stderr,format, args);//格式化输出到标准错误
		va_end(args);//清理可变参数列表
		fprintf(stderr, "\n");//输出换行符
	}
	pthread_mutex_unlock(&g_log_mutex);
}

void log_close(void)
{
	pthread_mutex_lock(&g_log_mutex);
	//如果日志文件已打开且不是stderr，则关闭 
	if(g_log_file && g_log_file != stderr)
	{
		fclose(g_log_file);//关闭文件，自动刷新缓冲区
		g_log_file = NULL;//将指针置空，避免野指针
	}
	pthread_mutex_unlock(&g_log_mutex);
}







