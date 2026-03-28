#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void handler(int none)//注册的新行为()
{
	write(1, "!", 1);
}

int main(void)
{
	
	
	if((signal(SIGINT, handler)) == SIG_ERR)
	{
		perror("signal()");
		return -1;
	}

	while(1)
	{
		write(1, "*", 1);
		sleep(1);
	}

	return 0;
}

