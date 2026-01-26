#include<stdio.h>

int main()

{
	int num[5];
	int num2[5];
	printf("输入五个数\n");
	for(int i = 0;i<5;i++)
		{
			scanf("%d",&num[i]);
			num2[4-i] = num[i];
			//printf("num:%d\n",num[i]);
			//printf("num2:%d\n",num2[i]);
		}
		for(int j=0;j<5;j++)
			{
				printf("%d	",num2[j]);

			}
			printf("\n");
		//num2[i-1] = num [i];
		//printf("num2[5]:%d\n",num2[5]);
}


