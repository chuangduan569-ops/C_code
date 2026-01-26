#include<stdio.h>


int main(void)
{
	int year = 0;
	printf("请输入年份\n");
	scanf("%d",&year);//取地址
	if(year%400==0||(year%100!=0&&year%4==0))
		printf("是闰年\n");
	else
		printf("不是闰年\n");
	return 0;
}
