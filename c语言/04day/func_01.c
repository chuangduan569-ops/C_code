#include<stdio.h>
int max(int a, int b)
{
	int tmp = 0;
	if(a > b)
		return a;
	else
		return b;
}

int main(void)
{
	int a = 0, b = 0;
	int max1 = 0;
	scanf("%d-%d",&a,&b);
	max1 =	max(a,b);
	printf("max = %d\n",max1);
	return 0;
}
