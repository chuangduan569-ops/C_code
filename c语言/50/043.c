#include<stdio.h>
//爱因斯坦

int main(void)
{
	//for死循环;
	int x = 0;
	for(x = 0;;x++){
		if(x%2==1&&x%3==2&&x%5==4&&x%6==5&&x%7==0){
			printf("最少有%d阶楼梯\n",x);
			break;
		}
	}



	return 0;
}





