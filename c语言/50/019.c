//输入一个字串，判断它是否是对称串。
//如”abcdcba”是对称串，”123456789”不是。
//{前提char类型1个字节}fgets(str,sizeof(str),stdin);从终端读取字符包括空格，存储到（数组名）数组，
//数组大小最多读取size0f(str)-1个字符，第三个固定值；  
#include<stdio.h>
#include<string.h>
/*
int mystrcmp(const char *s1,const char *s2)
{
	int i = 0;
	for(i = 0;s1[i]==s2[i]&&s1[i]!='\0';i++);//空循环字符串相同到\0跳出，字符串不同不满足循环就跳出
		return s1[i] - s2[i];
}

size_t mystrlen(const char*s)
{
	size_t i = 0;
	for(i=0;s[i]!='\0';i++)//空循环，用来遍历字符串所有数组并计数，直到\0跳出
	return i;
}
*/
int main(void)
{
	char str[1023];//定义一个字符串用于存储输入
	int left = 0, right = 0;
	int l1 = 0;
	printf("请输入一个字符串\n");
	fgets(str, sizeof(str), stdin);//从标准输入读取sizeof(str)-1个字符。
	l1 = strlen(str);//调用函数
	right = l1 - 2;
	//是一个数组，用while循环来实现从左到右，和从右到左，不必要用到strcmp
	while(right>left)
	{
//		printf("%d,%d\n",str[left],str[right]);
		if(str[left]!=str[right])
		{
			printf("no\n");
			break;
		}
		left++;
		right--;
	}
	if(str[left]==str[right])
		printf("yes\n");
	return 0;
}




