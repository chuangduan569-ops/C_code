#include<stdio.h>
int main(void)
{
	char c;
	while((c = getchar())!='\n')
		{
			if(c>='a'&&c<='z')
				printf("%c",c-32);//小写转大写
			else if(c>='A'&&c<='Z')
				printf("%c",c+32);//大写转小写
			else
				printf("%c",c);//保持不变
		}
		printf("\n");

	return 0;
}
