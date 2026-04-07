#ifndef __FSM_H
#define __FSM_H

#define BUFSIZE 1024

//定义状态枚举
enum {STATE_R, STATE_W, STATE_E, STATE_T};

//定义状态机数据类型
typedef struct
{
    int rfd;            //读文件的文件描述符
    int wfd;            //写文件的文件描述符
    char buf[BUFSIZE];  //存储读取到的数据的存储空间
    int count;          //成功读取到的字节个数
    int pos;            //已写入的字节个数
    int state;          //状态机的当前状态
    char *errmsg;       //记录出错函数的函数名
}fsm_t;

//定义接口
/*
功能 : 初始化有限状态机
参数 : f 要初始化的状态机
       rfd 读文件的文件描述符(客户指定)
       wfd 写文件的文件描述符(客户指定)
返回值 : 成功返回0;失败返回<0
[1]给有限状态机开辟空间
[2]给有限状态机的结构成员初始化
注意 ： 如何保证客户传过来 rfd 和 wfd 是非阻塞的
(rfd和wfd是非阻塞的文件描述符(fcntl(2)))
*/
extern int fsm_init(fsm_t **f, int rfd, int wfd);

/*
功能 : 推动有限状态机
参数 : f 要改变状态的有限状态机
返回值 : 成功返回0;失败返回<0
判断有限状态机的状态
[r] 把rfd文件里的数据读到buf中,判断read(2)返回值
        >0  读成功了,推到[w]
        ==0 读到了EOF,推到[t]
        <0  读错误了,推到[e](判断是否是假错,如果是假错继续读)
[w] 把buf里面的数据写到wfd,判断write(2)返回值
        >0  写成功了,推到[r](判断是否把要写的数据都写完,如果没有继续写)
        <0  写错误了,推到[e]
[e] perror打印错误信息,推到[t]
[t] 进程结束
*/
extern int fsm_drive(fsm_t *f);

/*
功能 : 销毁有限状态机
参数 : f 要销毁的有限状态机
返回值 : 成功返回0;失败返回<0
把开辟出的有限状态机释放了
*/
extern int fsm_destroy(fsm_t *f);

#endif


