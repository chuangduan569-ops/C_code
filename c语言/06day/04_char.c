#include<stdio.h>


int main(void)

{
	char arr[10];
	int i = 0;
	
	for(i = 0;i < 10 ;i++)
	{
//		scanf("%hhd",&arr[i]);
	scanf("%c",&arr[i]);
	}
	for(i= 0;i<10;i++)
	{
		printf("arr[%d] = %d\n",i,arr[i]);
	}


	return 0;
}
