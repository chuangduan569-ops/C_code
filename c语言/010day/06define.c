#include<stdio.h>

//#define BUG//定义BUG宏

int main(void)
{	
#ifdef BUG
	int a = 10;
	printf("调试模式 : a = %d\n",a);
#else
	int a = 20;
	printf("发布模式 : a = %d\n",a);
#endif

	return 0;
}
