#include<stdio.h>
//输入一行小写英文字母，按如下规则加密：a→c、b→d、…、x→z、y→a、z→b，试写加密程序。
int main(void)
{
	char c;
	while((c = getchar())!='\n')
		{
			if(c>='a'&&c<='x')
				printf("%c",c+2);//小写+3
			else if(c=='y')
				printf("%c",'a');//特殊定义
			else if(c=='z')
				printf("%c",'b');//特殊定义
			else
				printf("%c",c);//保持不变
		}
		printf("\n");

	return 0;
}
