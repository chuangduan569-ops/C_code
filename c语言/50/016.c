#include<math.h>
#include<stdio.h>
int main(void)
{	//用for循环找出100-200的完全平方数
	int a;
	for(int i=100;i<=200;i++){
		a=sqrt(i);
		if(a*a==i)
		printf("100-200的完全平方数有:%d\n",i);
	}

	return 0;
}
