#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler(int none)
{
	write(1, "!", 1);//往标准输出文件中写一个!
}

int main(void)
{
	int i = 0, j = 0;//循环变量
//	sigset_t old, new;//old存储原有的信号集,new存储设置的信号集	
	struct sigaction oldact, newact;//要设置的行为

	oldact.sa_handler = SIG_DFL;//给oldact设置默认行为额
	sigemptyset(&oldact.sa_mask);//给oldact的信号集清空
	oldact.sa_flags = 0;//oldact无特殊需求
	
	newact.sa_handler = sig_handler;//给newact设置新的行为
	sigemptyset(&newact.sa_mask);//给newact的信号集清空
	sigaddset(&newact.sa_mask, SIGINT);//将SIGINT信号添加到newact信号集中
	sigaddset(&newact.sa_mask, SIGRTMIN);//将SIGRTMIN信号添加到newact信号集中
	newact.sa_flags = 0;//oldact无特殊需求

	sigaction(SIGINT, &newact, NULL);//	给SIGINT设置新的行为
	sigaction(SIGRTMIN, &newact, NULL);// 给SIGRTMIN设置新的行为	

//	sigemptyset(&new);//把new信号集清空
//	sigaddset(&new, SIGINT);//将SIGINT信号添加到new信号集中
//	sigaddset(&new, SIGRTMIN);//将SIGRTMIN信号添加到new信号集中	
//	signal(SIGINT, sig_handler);//给SIGINT设置新的行为
//	signal(SIGRTMIN, sig_handler);//给SIGRTMIN设置新的行为

	sigprocmask(SIG_BLOCK, &newact.sa_mask, &oldact.sa_mask);
	//在打印每一行内容期间不被SIGINT信号干扰
	for(i = 0; i < 10; i++)
	{
		//给当前调用进程阻塞new信号集中的信号,并且把原有的信号屏蔽字存储到old中
//		sigprocmask(SIG_BLOCK, &new, &old);
		for(j = 0; j < 10; j++)
		{
			write(1, "*", 1);//往标准输出文件中写一个*
			sleep(1);
		}
		write(1, "\n", 1);//往标准输出文件中写一个'\n'
//		sigprocmask(SIG_SETMASK, &old, NULL);
		//给当前调用进程恢复之前的信号屏蔽字
	sigsuspend(&oldact.sa_mask);//当执行这一条语句相当于在执行
	/*
	 [1] 
	 
	 */
	
	}


	return 0;
}







