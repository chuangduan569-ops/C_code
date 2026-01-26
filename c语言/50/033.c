//“百钱百鸡”问题。百钱买百鸡，
//鸡翁一值钱三，鸡母一值钱二，鸡雏三值钱一，
//扩大三倍1 9, 1 6,1 1
//问鸡翁、鸡母、鸡雏各几何？
#include<stdio.h>

int main(void)
{
	int i = 0, j = 0, k = 0;
	for(i=0;i<=33;i++){
		for(j=0;j<=50;j++){
			for(k=0;k<=300;k++){
				if(i*9 + j*6 + k == 300 && i+j+k == 100)
					printf("i = %d	j = %d	k = %d\n",i,j,k);
			}
		}
	}

	return 0;
}
