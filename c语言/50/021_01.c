#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
//随机产生N个大写字母输出，然后统计其中共有多少个元音字符。（设N为50）
void count(int n)
{
	int i = 0;
	int sum = 0;
	for(i = 0;i < n; i++)
	{
	char randow = 'A' + rand() % 26;
	if(randow=='A' || randow=='E' || randow=='I' || randow=='O' || randow=='U')
		sum++;
	printf("%c",randow);	
	}
	printf("\n");
	printf("有%d个元音字符!",sum);
	printf("\n");
}
int main(void)
{
	int n = 0;
	srand(getpid());//调用随机函数
	//char a = 'A' + rand()%26;
	printf("请输入产生的随机数：\n");
	scanf("%d",&n);
	count(n);
	return 0;
}
