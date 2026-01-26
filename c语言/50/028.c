//随机产生N个两位自然数，降序排列后输出。（设N为20）
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
void paixu(int n)
{
	int arr[64] = {0};
	int i = 0, j = 0;
	int num = 0, tmp = 0;
	
	for(i = 0;i < n; i++)
		arr[i] = rand() % 90 + 10;//存入数组
	for(i=0;i<n;i++)
		printf("%d  ",arr[i]);//输出原始数据
	printf("\n");
		for(i=0;i<n-1;i++){
			for(j=0;j<n-i-1;j++){
				if(arr[j]<arr[j+1]){
					tmp = arr[j];
					arr[j] = arr[j+1];
					arr[j+1] = tmp;
				}
				//printf("%d",arr[j]);
			}			
//		printf("%d\n",num);
	}
		for(i=0;i<n;i++)
		printf("%d  ",arr[i]);//输出排序后的结果
	printf("\n");
}	




int main(void)
{
	int n = 0;
	srand(getpid());//调取随机种子
	printf("请输入你要随机产生两位数的个数：\n");
	scanf("%d",&n);
	//rand() % 90 + 10;
	paixu(n);
	return 0;
}
