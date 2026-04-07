//采集日志
//打开系统日至文件 读取新增内容 写入共享内存
#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>    
#include <unistd.h>     /* UNIX系统调用，用于read、lseek、usleep */
#include <fcntl.h>      /* 文件控制，用于open */
#include <sys/stat.h>   /* 文件状态，用于stat获取文件信息 */
#include <time.h>       // 时间函数，用于log_msg
#include "ipc.h"       

int shmid; //共享内存ID shm_create()返回值
int semid; //信号量ID sem_create()返回值
struct shm_buf *shm; //共享内存指针 shm_attach()返回值

static char filename[256] = "/var/log/syslog"; //要监控的系统日志文件路径
static long offset = 0; //当前读取位置（字节偏移量），用于断点续传
static ino_t last_inode = 0; //上次文件的inode号，用于检测文件是否被轮转

/*
log_msg 打印带时间戳的日至到stderr
参数msg  要打印的信息字符串
功能 用于调式和状态输出 
*/
static void log_msg(const char *msg)
{
	time_t now; //当前时间戳（秒数） 
	struct tm *tm_info; //时间结构体，存储分解后的时间
	char time_str[32]; //格式化后的时间字符串缓冲区

	time(&now); //获取当前时间戳 
	tm_info = localtime(&now); //转换为本地时间结构体
	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
	//打印到stderr（标准错误），格式：[时间] [INFO] 消息
	fprintf(stderr, "[%s] [INFO] %s\n", time_str, msg);
	fflush(stderr); //刷新缓冲区
}
/*
save_offse 保存当前读取位置到文件（断点续传）
功能：将offset和last_inode写入磁盘，程序重启后可恢复
 文件路径：/tmp/collector.offset
*/
static void save_offset(void)
{
	FILE *fp; //文件流指针
	fp = fopen("/tmp/collector.offset", "w"); //写模式打开，不存在则创建
	if(fp)//如果打开成功
	{
		//写入格式：偏移量 空格 inode号 换行
		fprintf(fp, "%ld %lu\n", offset, last_inode);
		fclose(fp);
	}
}
/*
load_offset - 加载上次保存的读取位置（断点续传） 
功能：从磁盘读取上次的offset和last_inode，恢复读取位置
*/
static void load_offset(void)
{
	FILE *fp; //文件流指针
	fp = fopen("/tmp/collector.offset", "r"); //读模式打开 
	if(fp)//文件打开成功
	{
		//读取格式：偏移量 空格 inode号
		fscanf(fp, "%ld %lu", &offset, &last_inode);
		fclose(fp);
	}
}

/*
 main 采集模块入口函数
功能：连接IPC资源，打开日志文件，循环读取新增内容并写入共享内存
返回值：0成功，1失败
 */

int main(void)
{
	int fd;	//日志文件描述符 
	char buf[8192]; //读取缓冲区，8KB大小
	ssize_t n; //实际读取的字节数
	struct stat st; //文件状态结构体，存储文件大小、inode等信息

	log_msg("Collector 启动，监控文件: /var/log/syslog");//提示信息
	
	//[1] 连接共享内存和信号量
	shmid = shm_create(SHM_KEY, sizeof(struct shm_buf));//获取已存在的共享内存（主进程已创建） 
	shm = (struct shm_buf *)shm_attach(shmid);//将共享内存映射到当前进程地址空间
	semid = sem_create(SEM_KEY);//获取已存在的信号量（主进程已创建）
	//[2] 加载上次读取位置（断点续传
	load_offset(); //从/tmp/collector.offset读取偏移量
	//[3] 打开系统日志文件
	fd = open(filename, O_RDONLY); //只读模式打开 
	if(fd < 0)
	{
		perror("open()");
		return 1;
	}
	//[4] 定位到上次读取位置 
	lseek(fd, offset, SEEK_SET); //lseek: 移动文件读写指针，SEEK_SET表示从文件开头偏移offset字节
	//[5] 主循环 - 持续监控文件变化
	while(1)
	{//获取文件状态（大小、inode、修改时间等）
		if(stat(filename, &st) == 0)//0表示成功
		{
			//检测文件轮转
			if(last_inode != 0 && last_inode != st.st_ino)//如果last_inode不为0（不是第一次）且inode发生了变化
			{
				log_msg("检测到文件轮转，重置偏移量");
				offset = 0; //重置偏移量
				lseek(fd, 0, SEEK_SET); //文件指针移到开头 
			}
			last_inode = st.st_ino; //更新inode号为当前值
		//检查是否有新内容
		if(st.st_size > offset)//如果文件当前大小大于已读偏移量，说明有新数据
		{
			n = read(fd, buf, sizeof(buf));//读取新增内容 
			if(n > 0)//读取成功
			{
				//写入共享内存
				sem_wait(semid); //P操作（加锁），防止多个进程同时写
				memcpy(shm->data + shm->write_pos, buf, n);//将数据复制到共享内存的当前写位置 
				shm->write_pos += n;//动写指针
				sem_signal(semid);//V操作（解锁)
				//更新偏移量并保存（断点续传）
				offset += n;//更新内存中的偏移量
				save_offset();//保存到磁盘文件
			}
		}
		}
		usleep(100000); //100000微秒 = 0.1秒 = 100毫秒
	}
	close(fd); //关闭文件（实际不会执行）
	return 0;
}











