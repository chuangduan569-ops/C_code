#include<stdio.h>

int main(void)
{
	int num = 0, num1 = 0, tmp = 0;
	printf("请输入您的数\n");
	scanf("%d",&num);
	while(num!=0)
	{
		tmp = num % 10;
		num1 = num1 *10 + tmp;
		num /= 10;
	}
	printf("%d\n",num1);


	return 0;
}
