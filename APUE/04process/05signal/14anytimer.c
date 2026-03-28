#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void sig_handler(int none)
{   
    write(1, "!", 1);
}

int main(void)
{
	struct sigaction act;//定义了设置行为的结构体
    
    act.sa_handler = sig_handler;//设置新的行为
    sigemptyset(&act.sa_mask);//清空信号集
    act.sa_flags = 0;//没有特殊需求
    
    sigaction(SIGALRM, &act, NULL);//为SIGALRM设置新的行为
    
    alarm(1);
    alarm(10);
    alarm(5);
     while(1)
    {
        write(1, "*", 1);
        sleep(1);
    }

	return 0;
}



