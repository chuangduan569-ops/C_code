#include<stdio.h>
#define VERSION 1
int main(void)

{
#if VERSION < 1
#warning 版本过低，建议升级到1.0版本以上
	int arr[10];
#else 
	int arr[20];
#endif
	printf("sizeof(arr) = %ld\n",sizeof(arr));


	return 0;
}
