#include "iot_gateway.h"
#include "config.h" 
#include "log.h" 
#include "network.h" 
#include "token_bucket.h"

void config_set_default(gateway_config_t *cfg)
{
	//网络配置默认值  来自network.h
	cfg->tcp_port = DEFAULT_TCP_PORT;//TCP端口默认值 8080
	cfg->udp_port = DEFAULT_UDP_PORT;//UDP端口默认值 8081
	cfg->udp_broadcast_port = DEFAULT_UDP_BROADCAST_PORT;  // 新增
	cfg->max_connections = MAX_CONNECTIONS;//最大连接数默认值 5000
	//令牌桶默认值 来自token_bucket.h 
	cfg->token_rate = DEFAULT_TOKEN_RATE;//令牌速率: 1000个/秒 
	cfg->token_burst = DEFAULT_TOKEN_BURST;//令牌桶容量: 2000个令牌
	//日志配置默认值
	strcpy(cfg->log_path, DEFAULT_LOG_PATH);//日志路径: ./log 目录 
	//认证令牌默认值 - 来自network.h 
	strcpy(cfg->auth_token, DEFAULT_AUTH_TOKEN);//默认认证令牌
	//云平台配置默认值 - 来自network.h
	strcpy(cfg->cloud_ip, DEFAULT_CLOUD_IP);//云平台IP地址默认值
	cfg->cloud_port = DEFAULT_CLOUD_PORT;//云平台端口默认值
	//日志级别默认值
	cfg->log_level = LOG_INFO;//INFO级别，普通信息
}
//从文件加载配置
//参数: filename - 配置文件路径
	//cfg - 配置结构体指针
int config_load(const char *filename, gateway_config_t *cfg)
{
	FILE *fp;//文件指针，用于打开配置文件
	char line[CONFIG_LINE_MAX];//行缓冲区，存储读取的每一行
	char key[CONFIG_KEY_MAX];//键缓冲区，存储配置项名称
	char value[CONFIG_VALUE_MAX];//值缓冲区，存储配置项值 
	char *p, *end;//临时指针，用于去除空格
	//先用默认值初始化
	config_set_default(cfg);//先填充默认值，再覆盖用户配置
	//打开配置文件
	fp = fopen(filename, "r");//只读模式打开配置文件
	if(fp == NULL)
	{
		LOG_WARN("无法打开配置文件 %s，使用默认配置", filename);
		return 0;//返回0表示成功 使用默认值
	}
	//逐行读取 
	while(fgets(line, sizeof(line), fp))//循环读取每一行
	{
		line[strcspn(line, "\n")] = '\0';//去除行尾的换行符
		//跳过空行和注释
		if(line[0] == '\0' || line[0] == '#')
			continue;//空行或以#开头的注释行，直接跳过

		//解析key=value格式
		
		if(sscanf(line, "%127[^=]=%255s", key, value) == 2)//判断是否成功读取到了key 和 value
		{
			p = key;//指向key字符串开头
			while(*p == ' ') p++;//跳过开头的空格
			end = p + strlen(p) - 1;//指向字符串末尾
			while(end > p && *end == ' ') end--;//跳过末尾空格
			*(end + 1) = '\0';//在末尾添加字符串结束符

			if(strcmp(p, "tcp_port") == 0)//TCP端口配置
				cfg->tcp_port = atoi(value);//字符串转整数
			else if(strcmp(p, "udp_port") == 0)//UDP端口配置
				cfg->udp_port = atoi(value);//字符串转整数
			else if (strcmp(p, "udp_broadcast_port") == 0)  // 新增：UDP广播端口配置
   				 cfg->udp_broadcast_port = atoi(value);
			else if (strcmp(p, "max_connections") == 0)//最大连接数
				cfg->max_connections = atoi(value);
			else if (strcmp(p, "token_rate") == 0)//令牌速率
				cfg->token_rate = atoi(value);
			else if (strcmp(p, "token_burst") == 0)//令牌桶容量
				cfg->token_burst = atoi(value);
			else if (strcmp(p, "log_path") == 0)//日志路径
				{
					strncpy(cfg->log_path, value, sizeof(cfg->log_path) - 1);
					cfg->log_path[sizeof(cfg->log_path) - 1] = '\0';//以尾0结尾
				}
			else if (strcmp(p, "auth_token") == 0)//认证令牌
				{
					strncpy(cfg->auth_token, value, sizeof(cfg->auth_token) - 1);
					cfg->auth_token[sizeof(cfg->auth_token) - 1] = '\0';
				}
			else if (strcmp(p, "cloud_ip") == 0)//云平台IP
				{
					strncpy(cfg->cloud_ip, value, sizeof(cfg->cloud_ip) - 1);
					cfg->cloud_ip[sizeof(cfg->cloud_ip) - 1] = '\0';
				}
			else if (strcmp(p, "cloud_port") == 0)//云平台端口 
				cfg->cloud_port = atoi(value);
			else if (strcmp(p, "log_level") == 0)//日志级别
				cfg->log_level = atoi(value);
		}
	}
	fclose(fp);//关闭配置文件 
	return 0;
}

//打印配置信息
void config_print(const gateway_config_t *cfg)
{
    LOG_INFO("=== 网关配置信息 ===");
    LOG_INFO("TCP端口: %d", cfg->tcp_port);
    LOG_INFO("UDP端口: %d", cfg->udp_port);
    LOG_INFO("最大连接数: %d", cfg->max_connections);
    LOG_INFO("令牌速率: %d个/秒, 突发容量: %d个", cfg->token_rate, cfg->token_burst);
    LOG_INFO("云平台地址: %s:%d", cfg->cloud_ip, cfg->cloud_port);
    LOG_INFO("日志路径: %s", cfg->log_path);
    LOG_INFO("日志级别: %d", cfg->log_level);
    LOG_INFO("=============================");
}












