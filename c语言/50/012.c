#include<stdio.h>
int main(void)
{
	int age = 0;
	for(int i=1;i<=5;i++){
		age = 10+2*(i-1);//第五个人的年龄
	}
	printf("age = %d\n",age);
}
