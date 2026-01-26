//30.从键盘输入10个战士的身高，输出平均身高，并找出哪些身高低于平均身高。
//31.从键盘输入10个战士的身高，输出平均身高，并找出哪些身高高于平均身高。
//32.从键盘输入10个战士的身高，输出最高、最低的身高。
#include<stdio.h>
void init(int *arr,int size){
	int i = 0;
	for(i=0;i<size;i++)
		scanf("%d",&arr[i]);
}

void print(int *arr, int size){
	 int i = 0;
	 for(i=0;i<size;i++)
		printf("arr[%d] = %-3d",i,arr[i]);
	printf("\n");
}

int avge(int *arr,int size){
	int i = 0;
	int n = 0;
	for(i=0;i<size;i++)
		n = n + arr[i];
	return n / size;

}
void gao(int *arr, int size, int age){
	int i = 0;
	for(i=0;i<size;i++){
		if(arr[i]>age)
		printf("a[%d] = %-3d",i,arr[i]);
	}
	printf("\n");
}
void di(int *arr, int size, int age){
    int i = 0;
    for(i=0;i<size;i++){
        if(arr[i]<age)
        printf("a[%d] = %-3d",i,arr[i]);
    }
    printf("\n");
}
void max(int *arr,int size){
	int i = 0,j=0;
	int t = 0;
	for(i=0;i<size;i++){
		for(j=0;j<size-i-1;j++){
			if(arr[j]>arr[j+1]){
				t = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = t;
				
			}
		}
//		printf("max=%d\n",arr[size]);
//		printf("min=%d\n",arr[0]);
	}
	 printf("max=%d\n",arr[size-1]);
	 printf("min=%d\n",arr[0]);



}
int main(void)
{
	int arr[10] = {0};
	int age = 0;
	int size = sizeof(arr) / sizeof(*arr);
	printf("请输入10个人的身高\n");
	init(arr,size);//输入函数
	print(arr,size);//输出函数
	age = avge(arr,size);//调用平均值函数
	printf("平均值:%d\n",age);
	gao(arr,size,age);
	di(arr,size,age);
	max(arr,size);
	return 0;
}
