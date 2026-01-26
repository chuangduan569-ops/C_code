#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	int a,b;
	char c;
	a = atoi(argv[1]);
	b = atoi(argv[3]);
	if((c=*argv[2])=='+')
		printf("a + b = %d\n",a+b);
	else
		printf("a - b = %d\n",a-b);
	return 0;
}
