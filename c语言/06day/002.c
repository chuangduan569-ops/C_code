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
void sort(int *p, int size)
{
	int i = 0,j = 0;
	int k = 0;
	int tmp = 0;
	if(size<=1)
	return ;
	for(i = 0;i < size - 1;i++)
	{
		k= i;//标记i的位置
		for(j = i+1;j<size;j++)
		{
			if(p[k] > p[j])
			k = j;//获取新位置
		}
	if(i!=j){
		tmp = p[i];
		p[i] = p[k];
		p[k] = tmp;
	}
	}


}

/*
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
*/

int main(void)
{
	int arr[5] = {100,201,50,70,30,};
	int size = sizeof(arr) / sizeof(arr[0]);
	print(arr,size);
	//sort(arr,size);
	sort(arr,size);
	print(arr,size);

	return 0;
}
