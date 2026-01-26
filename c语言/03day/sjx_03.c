#include<stdio.h>
int main(void)
{
	int i = 0, j = 0;
	int n = 0;
	scanf("%d",&n);
	for(i = 0;i < n; i++)
	{
		for(j = 0;j < i; j++)
		{
		printf(" ");
		}
		for(j = 0;j < n-i; j++){
			printf("*");
		}
		printf("\n");
	}
/*
行号i:   空格数   星号数    输出
i=0:     0空格    4星号    ****
i=1:     1空格    3星号     ***
i=2:     2空格    2星号      **
i=3:     3空格    1星号       *
*/
}
