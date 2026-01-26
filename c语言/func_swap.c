#include<stdio.h>
void swap (int a, int b);
void swap (int a,int b){
	printf("swap交换前:a=%d b=%d\n",a,b);
	int tmp = 0;
	tmp = a;
	a = b;
	b = tmp;
	printf("swap交换后:a=%d b=%d\n",a,b);
}



int main(void)
{
	int a = 0, b =0;
	scanf("%d-%d",&a,&b);
	printf("main函数前:a=%d b=%d\n",a,b);
	swap(a,b);
	


	printf("main函数后:a=%d  b=%d\n",a,b);






	return 0;
}
