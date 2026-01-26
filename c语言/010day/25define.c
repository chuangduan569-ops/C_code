#include<stdio.h>

int main(void)

{
	int a = 2,b = 3, c = 4;
	if(a+b>c&&\
	  (a+c>b)&&\
	  (b+c>a))
	{
		printf("可以构成三角形\n");
	}

	return 0;
}
