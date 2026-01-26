#include<stdio.h>
#define V 2
int main(void)
{
#if V==1
	printf("one\n");
#elif     V==2
	printf("two\n");
#else
	printf("ERRor\n");
#endif
	return 0;
}
