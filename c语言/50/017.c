#include<stdio.h>
int main(void)
{
	int a,b,c;
	printf("请输入三个正实数( - - )\n");
	scanf("%d-%d-%d",&a,&b,&c);
	if(a*a+b*b==c*c||a*a+c*c==b*b||b*b+c*c==a*a)
	printf("这三个数能够成直角三角形，分别为：%d %d %d\n",a,b,c);
	else
	printf("这三个数不能构成直角三角形\n");
	return 0;
}

