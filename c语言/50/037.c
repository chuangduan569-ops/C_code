//计算：1/2+1/4+……+1/n，设n是由键盘输入的正偶数。
//计算：1/1+1/3+……+1/n，设n是由键盘输入的正奇数。
//计算：1/2-2/3+3/4-4/5……，前100项

#include<stdio.h>
void oushu(int n)
{
	int i = 0;
	double sum = 0.0;
	if(n<0 || n%2 != 1){
		printf("Error\n");
		return;}
	for(i=1;i<=n;i+=2){
		sum += 1.0 / i;
	}
	printf("sum = %.6f\n",sum);
	
}
int main(void)
{	
	int n = 0;
	printf("请输入n的值:\n");
	scanf("%d",&n);
	oushu(n);

	return 0;
}
