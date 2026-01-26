#include<stdio.h>
//1+12+123+...+123456789(1-9)

int main()

{
	int n;
	int num = 0;//循环计数，累加值，用于关系转接
	int sum = 0;//接收总结果
	printf("请输入一个数字(1~9) : ");
	scanf("%d",&n);
	if(n>0&&n<10)
	{

	
	for(int i = 0;i <= n;i++){//要取到n值，因为n值也要取到
		num = num * 10 + i;
		sum+=num;
	}
	
	printf("总和为 : %d\n",sum);
	}
	else
		printf("Error\n");
	return 0;
}


