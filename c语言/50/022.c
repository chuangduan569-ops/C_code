//输出n行由“#”组成的矩形，每行“#”的个数为m个，
//其中n、m由终端（键盘）输入。
//要求输出m个“#”功能由函数satr(m)函数实现。
#include<stdio.h>
void init(int n,int m)
{
	int i = 0, j = 0;
	for(i = 0;i < n; i++)//行
		{
			for(j = 0; j < m; j++)//列
				printf("# ");
			printf("\n");
		}
//		printf("\n");
}
void star(int m)
{
	int i = 0;
	for(i = 0; i < m; i++)
		printf("# ");
		printf("\n");
}

int main(void)
{
	int n = 0, m = 0;
	printf("请输入行数\n");
	scanf("%d",&n);
	printf("请输入列数\n");
	scanf("%d",&m);
	star(m);
	printf("---------------------------------------\n");
	init(n,m);
	return 0;
}
