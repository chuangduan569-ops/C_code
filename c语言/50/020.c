#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
//随机产生N个大写字母输出，然后统计其中共有多少个元音字符。（设N为50）
void init(int n)
{
	int i = 0;
	int num = 0;
	char row;
	for(i=0;i<n;i++)
	{
	 row = 'A' + rand() % 26;
			printf("%c",row);
		if(row=='A'||row=='E'||row=='I'||row=='O'||row=='U')
			num++;
	}
	printf("\n");
	printf("元音字符个数：%d\n",num);
//	printf("\n");
}
int main(void)
{
	int n = 0;
	srand(getpid());//随机种子
	printf("请输入随机产的个数：\n");
	scanf("%d",&n);
	init(n);
	return 0;
}
