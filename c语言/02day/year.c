#include<stdio.h>
int main(void)
{
	int year = 0;
	scanf("%d",&year);
	if((year%400==0)&&((year%100!=0)||(year%4==0)))
	printf("是闰年\n");
	else
	printf("不是闰年\n");
}
