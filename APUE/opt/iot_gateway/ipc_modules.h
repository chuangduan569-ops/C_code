#ifndef __IPC_MODULES_H         
#define __IPC_MODULES_H          

#include <sys/ipc.h>   
#include <sys/msg.h>   
#include <sys/shm.h>   
#include <sys/sem.h>   

//IPC Key值定义 
#define IPC_KEY_MSGQ    0x1234  //消息队列key值，用于不同业务模块间异步消息传递 
#define IPC_KEY_SHM     0x5678  //共享内存key值，用于实时数据缓存(传感器最新数据) 
#define IPC_KEY_SEM     0x9ABC  //信号量key值，用于多进程同步互斥 

//共享内存配置
#define SHM_SIZE (1024 * 1024 * 4)  /*/共享内存大小: 4MB，用于存储传感器数据环形缓冲区 

//消息结构体 
 /* 
 IPC消息结构体
  用于消息队列的收发，支持多类型消息分类接收
 */
typedef struct {
    long mtype;           //消息类型(必须>0)
    char mtext[1024];     //消息内容缓冲区，最大1024字节 
} ipc_msg_t;              //定义IPC消息类型为ipc_msg_t 

//共享内存缓冲区结构体
/*
 共享内存缓冲区结构体
 用于多进程共享数据，实现高性能数据交换
 采用环形缓冲区设计，支持一写多读
 */
typedef struct {
    int write_pos;        //写指针位置
    int read_pos;         //读指针位置
    char data[SHM_SIZE];  //环形数据缓冲区，实际存储数据的数组 
} shm_buffer_t;           //定义共享内存缓冲区类型为shm_buffer_t 

//消息队列函数声明
/*
 
 功能描述: 创建或获取消息队列
 参数: key - IPC键值，用于唯一标识消息队列
 返回值: 成功返回消息队列ID，失败返回-1
 */
int msgq_create(key_t key);  

/*
 
 功能描述: 发送消息到消息队列
参数: msqid - 消息队列ID
      msg - 消息内容指针
      size - 消息数据大小(不包括消息类型)
      type - 消息类型(>0)
 返回值: 成功返回0，失败返回-1
 */
int msgq_send(int msqid, const void *msg, size_t size, long type);  

/*
功能描述: 从消息队列接收消息(阻塞)
 参数: msqid - 消息队列ID
      msg - 接收缓冲区
      size - 缓冲区大小
      type - 消息类型(0表示接收任意类型，>0表示接收指定类型)
 返回值: 成功返回接收字节数，失败返回-1
 */
int msgq_recv(int msqid, void *msg, size_t size, long type);  

/*

功能描述: 销毁消息队列
 参数: msqid - 消息队列ID
 返回值: 成功返回0，失败返回-1
 */
int msgq_destroy(int msqid);  

//共享内存函数声明 
/*
功能描述: 创建或获取共享内存
 参数: key - IPC键值
      size - 共享内存大小(字节)
 返回值: 成功返回共享内存ID，失败返回-1
 */
int shm_create(key_t key, size_t size);  

/*

功能描述: 将共享内存映射到当前进程地址空间
 参数: shmid - 共享内存ID
返回值: 成功返回映射地址，失败返回NULL
 */
void *shm_attach(int shmid); 

/*
功能描述: 解除共享内存与当前进程的映射
参数: addr - 共享内存映射地址
返回值: 成功返回0，失败返回-1
 */
int shm_detach(void *addr);  

/*
功能描述: 销毁共享内存
参数: shmid - 共享内存ID
返回值: 成功返回0，失败返回-1
 */
int shm_destroy(int shmid); 

//信号量函数声明 
/*
功能描述: 创建或获取信号量数组
 参数: key - IPC键值
       nsems - 信号量数量(数组大小)
       init_val - 信号量初始值数组，长度为nsems
 返回值: 成功返回信号量ID，失败返回-1
 */
int sem_create(key_t key, int nsems, unsigned short *init_val);  

/*
功能描述: P操作(加锁/申请资源)
 参数: semid - 信号量ID
      sem_num - 信号量在数组中的索引
返回值: 成功返回0，失败返回-1
 */
int sem_wait(int semid, int sem_num);  

/*
 功能描述: V操作(解锁/释放资源)
 参数: semid - 信号量ID
      sem_num - 信号量在数组中的索引
返回值: 成功返回0，失败返回-1
 */
int sem_signal(int semid, int sem_num);  

/*
 功能描述: 销毁信号量
参数: semid  信号量ID
返回值: 成功返回0，失败返回-1
 */
int sem_destroy(int semid);  

#endif  
