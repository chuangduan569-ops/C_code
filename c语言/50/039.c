//有一个数组{1,4,80,23,47,76,14,52}。
//现输入一个数，
//要求按原来的规律将它插入数组中并从大到小输出（排序用冒泡排序）
#include<stdio.h>
void print(int *arr, int size)
{
	int i = 0;
	for(i=0;i<size;i++)
		printf("arr[%d] = %-3d",i,arr[i]);
	printf("\n");
}
void sort(int *arr, int size)
{
	int i = 0, j = 0, t = 0;
	for(i=0;i<size-1;i++){
		for(j=0;j<size-i-1;j++){
			if(arr[j] < arr[j+1]){
				t = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = t;
			}
		}
	}
}
int main(void)
{
	int arr[9] = { 1 , 4, 80, 23, 47, 76, 14, 52, };
	int size = 8;
	int i = 0, num = 0, pos = 0;
	print(arr,size);
	sort(arr,size);
	print(arr,size);
	printf("请输入你要插入的值:\n");
	scanf("%d",&num);
	 // 找到插入位置（保持从大到小的顺序）
    // 找到第一个比num小的位置
	for(pos=0;pos<size;pos++){
		if(num > arr[pos])
			{
				break;
			}
		
	}
	// 将元素后移，腾出插入位置
	for(i=size;i>pos;i--){
		arr[i] = arr[i-1];
	}
	// 插入新数
	arr[pos] = num;
	size++;
	printf("插入 %d 后的数组（从大到小）：\n", num);
	print(arr,size);
	return 0;
}


