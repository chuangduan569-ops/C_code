#include<stdio.h>
int func(int n)
{
	if(n==0||n==1)
		return 1;
	return func(n-1) + func(n-2);
}
int main(void)

{
	int i = 0;
	for(i=0;i<20;i++)
		printf("%d	",func(i));
	printf("\n");


	return 0;
}
