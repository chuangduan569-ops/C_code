#include<stdio.h>
void print(const int *arr, int size)
{
	for(int i = 0;i<size; i++)
	{
		printf("%d ",arr[i]);
//	printf("\n");
	}
	printf("\n");
}
void sort(int *arr, int size)

{
	int i = 0, j = 0;
	int tmp = 0;
	if(size<=1)
	return ;

	for(i=0;i<size-1;i++)
	{
		for(j=0;j<size-i-1;j++)
		{
			if(arr[j]>arr[j+1])
			{
				tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
	
}


int main(void)
{
	int arr[5] = {100,201,50,70,30,};
	int size = sizeof(arr) / sizeof(arr[0]);
	print(arr,size);
	sort(arr,size);
	
	print(arr,size);

	return 0;
}
