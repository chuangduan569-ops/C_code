#include<stdio.h>
#include"add.h"
int main(void)
{

	int a = 0, b = 0, sum = 0;
	scanf("%d-%d",&a,&b);
	sum = add(a,b);
	printf("sum = %d\n",sum);
	return 0;

	return 0;
}
