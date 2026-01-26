#include<stdio.h>

int main(void)
{
	int num = 0, num1 = 0, tmp = 0;
	printf("请输入您的数字\n");
	scanf("%d",&num);
	while(num != 0)
	{
		tmp = num % 10;//取个位
		num1 = num1 *10 + tmp;//把每一次值赋值给num1
		num /= 10;
	}
		printf("您输入的数的反序数为:%d\n",num1);
	

	return 0;
}
