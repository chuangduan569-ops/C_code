#include<stdio.h>

int main()

{
	int a[20];
	int count = 0;
	int sum = 0;
	printf("请输入你的20个值\n");
	for(int i = 0;i<20;i++)
		{
			scanf("%d",&a[i]);//取数组的输入值(取地址)
			if(a[i]>0)
				{
					count++;//正数自增1
					sum+=a[i];//正数的和
				}
			printf("正数个数:%d\n",count);
			printf("正数之和:%d\n",sum);
		}


}


