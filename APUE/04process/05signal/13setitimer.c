/*使用系统调用IO函数实现mycat*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#define BUFSIZE 16

#define CPS		BUFSIZE//速率(积攒令牌和取走令牌的速率)
#define	BURST	(CPS) * 20 //令牌桶中令牌数量的上限(；尽量是速率的整数倍)

//static int flag = 0;//用来标记是否执行IO操作

static int token = 0;//以token表示令牌的个数

static void sig_handler(int none)//SIGALRM信号的行为
{   
    //alarm(1);//设置1s的闹钟
    //flag = 1;//设置flag的标记为1
	token += CPS; //每当SIGLARM信号到来，按照速率积攒令牌
	if(token >= BURST)//判断令牌桶中的token是否达到上限
		token = BURST;//把令牌数定格在上限制
}

static int mycat(int fd)
{   
    char buf[BUFSIZE] = {0};//存储读取到的数据
    int count = 0;//存储成功读取到的字节数
	int n = 0;//取走的令牌数(要读取的字节数)    

    while(1)
    {
        //while(!flag);//死等,等待flag==1
        //flag = 0;//恢复标记的初始值
		while(token == 0)//令牌桶中没有令牌        
			pause();//通知法等待
		if(token >= CPS)//判断令牌桶中的令牌数是否大于速率(想要取令牌的数量)
			n = CPS;//当token充足时，按照速率大小取令牌
		else
			n = token;	//当token不充足时,有多少取多少
		token -= n;//更新令牌桶中token的数量
		
		memset(buf, 0, BUFSIZE);//清空脏数据
        count = read(fd, buf, n);//从文件中读取数据(按照取走的令牌数量读取数据)
        if(count == 0)//判断是否读到了文件结尾的位置
            break;//跳出死循环
        else if(count < 0)//判断是否读取失败
        {
            perror("read()");//打印错误信息
            return -1;//由于读取失败,结束函数,并且返回-1
        }
        write(1, buf, count);//把数据写入到标准输出文件中
    }
}

int main(int argc, char *argv[])
{   
    int fd = 0;//fd变量用来保存打开文件的文件描述符
	struct itimerval itv;//存储间隔时间
	
	struct sigaction oldact, newact;//要设置的行为

	oldact.sa_handler = SIG_DFL;//给oldact设置默认行为(其实没用上oldact可以注释)
	sigemptyset(&oldact.sa_mask);//给oldact的信号集清空
	oldact.sa_flags = 0;//oldact无特殊需求

	newact.sa_handler = sig_handler;//给newact设置新的行为
	sigemptyset(&newact.sa_mask);//给newact的信号集清空
    sigaddset(&newact.sa_mask, SIGALRM);//将SIGALRM信号添加到newact信号集中
	newact.sa_flags = 0;//oldact无特殊需求

    if(argc < 2)//判断命令行参数个数是否少于2个
    {
        fprintf(stderr, "Usage : %s + filename\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
    }
	
	sigaction(SIGALRM, &newact, NULL);// 给SIGALRM设置新的行为
//    signal(SIGALRM, sig_handler);//给SIGALRM信号设置行为
   // alarm(1);//设置1s的闹钟
	itv.it_interval.tv_sec = 0;//设置重载时间
	itv.it_interval.tv_usec = 100000; //设置重载时间0.1s(单位微妙)
	itv.it_value.tv_sec = 3;//设置当前时间3s
	itv.it_value.tv_usec = 0;//设置当前时间3s
	setitimer(ITIMER_REAL, &itv, NULL);//设置闹钟时间

    fd = open(argv[1], O_RDONLY);//通过open(2)以只读的形式打开文件
    if(fd < 0)//判断打开文件是否失败
    {
        perror("open()");//打印错误信息
        return -2;//由于打开文件失败,结束程序,并且返回-2
    }
    
    mycat(fd);//调用自己实现的mycat()

    close(fd);//通过close(2)关闭文件

    return 0;
}

