#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MIN 100
#define MAX 300

int is_primer(int num)
{
	int i = 0;

    sleep(1);

    if(num <= 1)//判断num是否 <= 1
        return 0;
    if(num == 2 || num == 3)
        return 1;
    for(i = 2; i <= num / i; i++)
        if(num % i == 0)
            return 0;
    return 1;
}

int main(void)
{
	pid_t pid;
	int i = 0;
	int j = 0;	

	for(i = 0; i < 4; i++)
	{
		pid = fork();
		if(pid == -1)
		{
			perror("fork()");
			return -1;
		}
		if(pid == 0)
			break;
	}
	for(j = MIN; j <= MAX; j++)
	{
		if(j % 4 == i)
			if(is_primer(j))
				printf("[%d]%d is a Primer Number!\n",i, j);
	}
	sleep(1000);

	switch(i)
	{
		case 0 :
		case 1 :
		case 2 :
		case 3 : exit(0);
	}
	
	//父进程的操作
	for(i = 0; i < 4; i++)
		wait(NULL);

	return 0;
}


