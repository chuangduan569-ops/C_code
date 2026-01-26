#include<stdio.h>
void init(int *p,int size)
{
	int i = 0;
	for(i= 0; i < size; i++)
	scanf("%d",p + i);
//	scanf("%d",&p[i]);
}
int avag(int p[],int size)
{
	int i = 0;
	int num = 0;
	for(i=0;i<size;i++)
		num += p[i];
	return num / size;
//  num = num /size//可以直接返回num/size然后去用一个变量在主函数接受num的值
//	printf("%d\n",num);
//	printf("平均成绩是：%d\n", num);  // 在函数内直接输出
}
int check(const int *p,int size,int num1)
{
	int i = 0;
	for(i=0;i<size;i++)
	{
		if(p[i]<num1)
		printf("p[%d]:%d\n",i,*(p + i));//p[i] = *(p + 1)
	}
}
int main(void)
{
	int arr[10] = {0};
	int num1 = 0;
	int size = sizeof(arr) / sizeof(*arr);
	printf("请输入10个学生的成绩:\n");
	init(arr,size);//用来输入十个学生的成绩
	num1 = avag(arr,size);
	printf("平均成绩是：%d\n", num1);
	check(arr,size,num1);
	return 0;
}




