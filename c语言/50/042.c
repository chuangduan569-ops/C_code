#include<stdio.h>
/*
设N是一个四位数，它的9倍恰好是其反序数，求N。反序数就是将整数的数字倒过来形成的整数。
例如：1234的反序数是4321。
 */
int main(void)
{
	int qian,bai,shi,ge;
	for(int i = 1000; i < 10000; i++){
		qian = i / 1000;//千位
		bai = i / 100 % 10;//百位
		shi = i / 10 % 10;//十位
		ge  = i % 10;//个位
		if(9*(qian*1000+bai*100+shi*10+ge)==ge*1000+shi*100+bai*10+qian)//判断条件
				printf("这个四位数是:%d%d%d%d\n",qian,bai,shi,ge);
	}



	return 0;


}


