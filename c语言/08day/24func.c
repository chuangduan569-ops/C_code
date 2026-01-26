#include<stdio.h>
void func(int num)
{
	if(num==0)
		return;
	printf("%d",num%10);
	func(num / 10);
}

int main(void)
{
	int num = 0;
	scanf("%d",&num);
	func(num);
	printf("\n");
	
	

	return 0;
}
