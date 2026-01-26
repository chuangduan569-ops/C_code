#include<stdio.h>
int main(void)

{	int n = 0;
	int qian,bai,shi,ge;
	printf("请输入一个四位数\n");
	scanf("%d",&n);
	qian = n/1000;
	bai = n/100%10;
	shi = n/10%10;
	ge = n%10;
	printf("个%d	十%d	百%d	千%d\n",ge,shi,bai,qian);


	return 0;
}
