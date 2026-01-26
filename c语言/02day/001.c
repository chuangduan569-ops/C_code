#include<stdio.h>

//交换两个变量的值
int main(void)
	
{	int t = 0;
	int a = 0; 
	int b = 0;
	printf("请输入两个数(-)\n");
	scanf("%d-%d",&a,&b);
	printf("a = %d b = %d \n",a,b);
/*	a = a + b;
	b = a - b;
	a = a - b;
*/
	t = a;
	a = b;
	b = t;
	
	printf("a = %d b = %d\n",a,b);
	return 0;
}
