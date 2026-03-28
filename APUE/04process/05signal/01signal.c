#include <stdio.h>
#include <unistd.h>
int main(void)

{
	while(1)
	{
		write(1, "*", 1);
		sleep(1);
	}

	return 0;
}

