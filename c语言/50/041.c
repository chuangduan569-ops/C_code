/*
求s=a+aa+aaa+aaaa+......+aa...a的值，其中a是1~9之间的一个数字，
表达式项数从键盘输入（设不超过10项）。
例如2+22+222+2222+22222(此时共有5个数相加)，
几个数相加由键盘控制。
int num = rand() % (m - n + 1) + n;随机数公式
 */
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int main(void)

{	
	srand(time(NULL));//时间种子
	int i = 0;
	int a;
	int s = 0;//总值
	a = rand() % 10;
	int n;
	int sum = 0;//a->aa->aaa的值
	printf("请输入(1~9)\n");
	scanf("%d",&n);
	for(i = 0; i < n; i++){
		sum = 10 * sum + a;//每次*10加本身获得a->aa->aaa...
		s += sum;//累计值
	}
	printf("s = %d\n",s);


	return 0;
}



