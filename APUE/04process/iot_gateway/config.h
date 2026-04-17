#ifndef __CONFIG_H  
#define __CONFIG_H 

#include "iot_gateway.h"

#define DEFAULT_CONFIG_PATH "./iot_gateway.conf"  //默认配置文件路径，当前目录下的iot_gateway.conf
#define CONFIG_LINE_MAX 512      //配置文件每行最大长度，足够容纳一行配置 
#define CONFIG_KEY_MAX 128       //配置项名称最大长度，如"tcp_port"
#define CONFIG_VALUE_MAX 256     //配置项值最大长度，如"8080" 

// 新增：广播相关宏定义
#define DEFAULT_UDP_BROADCAST_PORT 8082  // UDP广播默认端口
/*
功能  从文件加载配置   解析key=value格式的配置文件，更新配置结构体
 参数 filename - 配置文件路径
 	cfg - 指向配置结构体的指针(输出参数
 返回值   成功返回0  失败返回-1
*/
int config_load(const char *filename, gateway_config_t *cfg);
/*
功能 打印当前配置信息到日志
参数 cfg - 指向配置结构体的指针
返回值 无
*/
void config_print(const gateway_config_t *cfg); 

/*
功能 设置配置结构体为默认值  当配置文件不存在或某项配置缺失时使用默认值
参数  cfg  指向配置结构体的指针
返回值 无
*/

void config_set_default(gateway_config_t *cfg);






#endif

