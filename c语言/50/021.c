#include<stdio.h>
//	找出三位数中的所有素数，要求判断x素数由函数data(x)实现
int data(int x){//定义函数
	int j = 2;//循环变量
	for(j = 2; j <= x / j;j++){
		if(x % j== 0)
			return 0;
	}
	return 1;
}

int main(void)
{
	int i = 100;//循环变量的值
	int sum = 0;
	for(i=100;i<1000;i++){
		if(data(i))//判断如果符合上边函数且返回值是1,打印是素数
		{
	
			printf("%d是素数\n",i);
			sum++;}
	}
	printf("sum = %d\n",sum);


}
