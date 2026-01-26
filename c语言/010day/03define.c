#include<stdio.h>
#define ADD

int main(void)
{
	int a = 13, b = 7;
#ifndef ADD//如果没有定义过这个宏
	printf("%d - %d = %d\n",a,b,a-b);
#else
	printf("%d + %d = %d\n",a,b,a+b);
#endif


	return 0;
}
