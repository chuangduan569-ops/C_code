#include<stdio.h>

int main(void)
{
	int a = 0,b = 0, c = 0;
	printf("请输入三角形三边长（ - - ）\n");
	scanf("%d-%d-%d",&a,&b,&c);
	if(a+b>c&&b+c>a&&a+c>b)
	{
		if(a==b||b==c||a==c)
		{
			if(a==b&&b==c)
			{
				printf("等边三角形\n");
			}
			else
				printf("可以构成等腰三角形\n");
		}
		else if(a*a+b*b==c*c||a*a+c*c==b*b||b*b+c*c==a*a)
		{
			printf("直角三角形\n");
		}
		else
		{
			printf("可以组成三角形\n");
		}
	}
	else
	{
		printf("Error\n");
	}

return 0;

}

