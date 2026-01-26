#include<stdio.h>

int main(void)
{
	int i = 0, j = 0;//循环变量
	printf("   1 2 3 4 5 6 7 8 9 A B C D E F\n");//列的提示
	for(i = 0;i < 15; i ++)//行数
	{
		printf(" %X ",i + 1);
		for (j = 0; j< 15; j++)//列数
		{	
//			printf(" %x ",j);
			printf("+ ");
		}
		printf("\n");
	}


	return 0;
}
