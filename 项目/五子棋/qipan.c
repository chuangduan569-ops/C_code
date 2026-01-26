#include<stdio.h>


int main(void)
{
	int i = 0, j = 0;
	for(i=0; i<= 15; i++)//行数
	{	
		printf("%X ",i);//打印提示
		for(j = 0;j <15; j++)//列数
		{
			if(i==0)
			printf("%X ",j+1);
			else
			printf("+ ");//打印棋盘
		}
			printf("\n");
	}


	return 0;
}
