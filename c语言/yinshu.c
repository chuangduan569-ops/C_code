#include<stdio.h>
//函数的声明,封装
// 11111111111111111111111
void func(int num);//函数的声明
void func (int num){
	for(int i = 1;i < num / i;i++)
		{
			if(num%i==0)
			printf("%d  %d  ",i,num / i);//打印公因数
		}
		printf("\n");
}//函数的功能
//主函数
int main(void)
{
	int num =0;
	printf("请输入数字\n");
	scanf("%d",&num);//把客户输入的值赋给num;num拿到函数中去使用;
	func(num);//调用函数
	return 0;
}
