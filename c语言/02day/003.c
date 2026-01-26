#include<stdio.h>

int main(void)

{
	int a,b,c;
	printf("请输入三角形三边长( - - )\n");
	scanf("%d-%d-%d",&a,&b,&c);
	if(a+b>c&&a+c>b&&b+c>a)
	{
		if(a==b&&b==c)
		{
			printf("等bian三角形\n");
			if(a==b||a==c||b==c)
			printf("等yao三角形\n");
		}
		else if(a*a+b*b==c*c||a*a+c*c==b*b||b*b+c*c==a*a)
			printf("直角三角形\n");
		else
			printf("可以构成三角形\n");
	}
	else
		printf("Error\n");


	return 0;
}
