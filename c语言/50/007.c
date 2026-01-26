#include<math.h>
#include<stdio.h>
int main(void)

{
	int a,b,c,d;
	int sum = 0;
	int result = 0;
	for(int i=1000;i<=9999;i++){
	a=i/1000;//千位
	b=i/100%10;//百位
	c=i/10%10;//十位
	d=i%10;//个位
	if(a==b&&c==d)
	{
		int e = sqrt(i);
		if(e*e==i){
			result = i;
			break;
		}
	}
	}
	
	printf("车牌号为：%d\n",result);
}
