#include<stdio.h>
void iton(int n1, int n2)
{	
	int tmp = n2;
	while(tmp > 0)
	{
	printf("%X",tmp % n1);
	tmp /= n1;	
	}
	printf("\n");
}
int main(void)

{
	int n1 = 0;//用来接受用户想要转的进制数
	int n2 = 0;//用来接受用户输入的十进制数
	printf("请输入你想要转换的进制(2~16)\n");//
	scanf("%d",&n1);
	
	printf("请输入你想要转换的十进制数\n");//
	scanf("%d",&n2);

	iton(n1,n2);//调用函数


	return 0;
}
