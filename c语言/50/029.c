//从键盘输入两个字串，输出其中较长的那个字串，并输出它的长度。
#include<stdio.h>
#include<string.h>
int main(void)
{
	char str1[32];
	char str2[32];
	int leng1 = 0,leng2 = 0;
	printf("请输入你的字串(有空格自动中断)\n");
	scanf("%s",str1);
	printf("请输入你的字串(有空格自动中断)\n");
	scanf("%s",str2);
	leng1 = strlen(str1);
	leng2 = strlen(str2);
	if(leng1>leng2)
		printf("字串:%s		长度:%d\n",str1,leng1);//输出较长的字串
	else if(leng1==leng2)
		printf("字串长度相等:%s  %s     长度(一样长):%d\n",str1,str2,leng1);//输出较长的字串
	else
		printf("字串:%s     长度:%d\n",str2,leng2);//输出较长的字串
	return 0;
}
