#include<stdio.h>
void init (int *p,int size,int *sum){//需要修改；不需要的话用const修饰
	for(int i = 0;i<size;i++){
		scanf("%d",&p[i]);//p+i指针加偏移量
		*sum+=p[i];
	}
	*sum/=size;
}
void print(int *p,int size,int sum){
		for(int j=0;j<size;j++){
			if(p[j]<sum)
			printf("p[%d] = %d\n",j,p[j]);
		}
}
int main(void)

{
	int arr[10] = {0};
	int size = sizeof(arr)/sizeof(*arr);
	int sum = 0;//平均身高
	init(arr,size,&sum);//录入十个人的身高
	print(arr,size,sum);//打印小于平均身高的值
	return 0;
}
