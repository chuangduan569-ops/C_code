#include<stdio.h>

int main()

{
	int n;
	int sum=0;
	printf("请输入你的数\n");
	scanf("%d",&n);
	for(int i = 1;i<n+1;i++)
		{
			sum+=i;
			printf("每次的结果为:%d\n",i);
		}
	printf("1-n的结果为：%d\n",sum);
}


