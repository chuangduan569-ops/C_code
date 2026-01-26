#include<stdio.h>
int main(void)

{
	int a,b,c,d,count = 0;
	for(a=1;a<10;a++){
		for(b=1;b<25;b++){
			for(c=1;c<50;c++){
				for(d=1;d<100;d++){
					if(a*50+b*20+c*10+d*5==500){
						count++;
					 	printf("第%d种：50:%d  20:%d  10:%d  5:%d\n",count,a,b,c,d);
					}
				}
			}
		}
		
	}

}


