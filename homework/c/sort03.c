#include<stdio.h>
void print(const int *arr, int size)
{
	for(int i = 0;i<size;i++)
		printf("%d	",arr[i]);
	printf("\n");
}
void sort(int *arr,int size)
{
	int i = 0, j = 0;
	int	flag = 0;
	if(size<=1)
	return;
	for(i = 1;i<size;i++)
	{
		flag = arr[i];//保存当前等待插入的元素数据
		j = i - 1;//指向当前元素的前一个位置，已排序的最后一个位置
		while(j>=0&&arr[j] > flag)//内层循环，将已排序的大于flag的元素向后移动
		{
			arr[j+1] = arr[j];
			j--;
		}
		//循环结束j+1的就算flag应该插入的正确位置
		arr[j+1] = flag;
	}
}
int main(void)
{
	int arr[5]={100,201,50,70,30};
	int size = sizeof(arr) / sizeof(*arr);
	print(arr,size);

	sort(arr,size);
	
	print(arr,size);

	return 0;
}
