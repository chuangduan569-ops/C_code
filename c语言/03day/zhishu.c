#include<stdio.h>

int main(void)
{
	int i = 0, j = 0;//
	int num = 1;//计数变量
	for(i = 2;i < 10000000;i++)//；遍历2-100以内的所有数
	{	
		for(j = 2; j <= i/j; j++)//j取出2-根号i中所有的数
		{
			if(j>2 && j%2==0)
			{
				continue;//跳过质数
			}
			if(i % j  == 0)
				break;
		}
		if(j > i/j)
		printf("[%d]	%d\n", num++, i);
	}

	return 0;
}
