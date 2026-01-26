#include<stdio.h>
int fxs(int j){
	int a = 0,b = 0,c= 0,d=0;
	a =j/1000;
	b=j/100%10;
	c=j/10%10;
	d=j%10;
	if(9*(a*1000+b*100+c*10+d)==(d*1000+c*100+b*10+a)){
		return 1;
	}
	else{
		return 0;
	}
}
int main(void)

{	int i = 0,a=0,b=0,c=0;
	int result = 0;
	for(i = 1000 ;i<10000;i++){
		result = fxs(i);
//		a= i/1000;
//		b = i/100%10;
//		c = i/10%10;
//		d= i/100;
//		if(9*(a*1000+b*100+c*10+d)==(d*1000+c*100+b*10+a))
		if(result==1){
			printf("九倍反序数:%d\n",i);
		}
		}

}


