#include<stdio.h>


int main(void)
{
	int arr[5];
	int i = 0;
	for(i=0;i<5;i++)
		arr[i] = 100 + i;
	for(i=0;i<5;i++)
	{
		printf("arr[%d] = %d\n",i,arr[i]);
	}
	for(i=0;i<5;i++)
	{
		scanf("%d",&arr[i]);
	}
	   for(i=0;i<5;i++)
   {
       printf("arr[%d] = %d\n",i,arr[i]);
   }

}
