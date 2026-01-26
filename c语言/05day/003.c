#include<stdio.h>


int main(void)
{
	int arr[5];
	int i = 0;
	int	size = sizeof(arr) / sizeof(*arr);
/*	for(i=0;i<5;i++)
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
*/
	for(i=0;i<size;i++)
		scanf("%d", arr + i);
	for(i=0;i<size;i++)
//	printf("arr[%d] = %d\n", i, *(arr + i) );
	printf(" *(arr + %d) = %d\n",i, *(arr + i));
}
