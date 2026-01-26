#include<stdio.h>
#define FLAG 1
int main(void)
{
	int a = 13, b = 7;
#if FLAG == 0
	printf("%d + %d = %d\n",a,b,a+b);
#elif FLAG == 1
	printf("%d - %d = %d\n",a,b,a-b);
#elif FLAG == 2
	printf("%d * %d = %d\n",a,b,a*b);
#elif FLAG == 3
	printf("%d / %d = %d\n",a,b,a/b);
#endif 


	return 0;
}
