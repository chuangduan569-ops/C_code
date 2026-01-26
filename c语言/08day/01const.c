#include<stdio.h>

int main(void)
{
	const int a = 123;
	int *p = &a;
	printf("a = %d\n",a);
//	a = 9527;	错	因为有const修饰
	*p = 9527;

	printf("a = %d\n",a);
	return 0;
}
