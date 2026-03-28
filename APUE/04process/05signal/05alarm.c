#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int none)
{
	alarm(1);//形成了alarm链(可以周期性的产生SIGALRM信号了)
	write(1, "!", 1);//往标准输出中写一个!
}

int main(void)
{
	signal(SIGALRM, sig_handler);//为SIGALRM信号设置新行为
	alarm(1);//设置1s的闹钟(注意:只设置了一次闹钟)

	while(1);//死循环

	return 0;
}



