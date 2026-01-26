#include<stdio.h>
int func(int n)
{
	if(n==1)
		return 1;
	return n + func(n - 1);
}
int main(void)
{
	int sum = 0;
	
	sum = func(100);

	printf("sum = %d\n", sum);

	return 0;
}
