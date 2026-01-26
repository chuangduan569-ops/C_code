#include<stdio.h>

int main(void)
{
	int arr[5];
	int i = 0;
	for(i=0;i<5;i++)
		printf("sizeof(arr[%d]) = %ld\n",i,sizeof(arr[i]));
}
