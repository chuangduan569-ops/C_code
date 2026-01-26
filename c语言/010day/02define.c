#include<stdio.h>
#define SUB
int main(void)
{
	int a = 13, b = 7;
#ifdef SUb
	printf("%d - %d = %d\n",a,b,a-b);
#else
	printf("%d + %d = %d\n",a,b,a+b);
#endif


	return 0;
}
