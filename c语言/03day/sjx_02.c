#include<stdio.h>
int main(void)
{
	int i = 0, j = 0;
	int n = 0;
	scanf("%d",&n);
	for(i = 0;i < n;i++)
	{
		for(j = 5;j >= n - i ; j--)//lieshu
		{
		printf("*");
		}
		printf("\n");
	}
		
	}
