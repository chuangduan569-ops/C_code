#include<stdio.h>
int sxh(int j){
	int a = 0,b = 0,c= 0;
	a=j/100;
	b=j/10%10;
	c=j%10;
	if(j==a*a*a+b*b*b+c*c*c){
		return 1;
	}
	else{
		return 0;
	}
}
int main(void)

{	int i = 0,a=0,b=0,c=0;
	int result = 0;
	for(i = 100 ;i<1000;i++){
		result = sxh(i);
//		a = i%10;
//		b = i/10%10;
//		c= i/100;
//		if(a*a*a+b*b*b+c*c*c==i)
		if(result==1){
			printf("水仙花数:%d\n",i);
		}
		}

}


