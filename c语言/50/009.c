#include<stdio.h>
int main(void)
{	int a;
	int count = 0;
	for(int i=100;i<1000;i++){
		a=i%10;//个位
		if(a==6){

		if(i%3==0&&i%5!=0){//被3整除不被五整除
			count++;//数量加以
			printf("%d  ",i);//打印符合条件的数
		}	
	}
	}
	printf("\n");//换行
	printf("共有%d\n个",count);//计数
	return 0;
}
