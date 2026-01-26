#include<stdio.h>
int itob(int n)
{	int tmp = n;
	while(tmp)
	{
		printf("%d",tmp%2);
		tmp /=2;
	}
	printf("\n");
}


int main(void)
{
	int n = 0;
	scanf("%d",&n);//录入十进制整数
	itob(n);
}
