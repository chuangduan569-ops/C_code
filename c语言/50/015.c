#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main(void)
{	int n;
	srand(getpid());
	printf("请输入要产生随机三位自然数的个数\n");
	scanf("%d",&n);
//先随机产生n个然数输出，然后再输出其中同时是3、5、7倍数的数。（设N为100）
	for(int i=1;i<=n;i++){
	int num = rand()%900+100;
	printf("第%d个随机自然数是:%d\n",i,num);
	if(num%3==0&&num%5==0&&num%7==0)
	printf("能同时被3、5、7整除的数是:%d\n",num);
	}





	return 0;
}
