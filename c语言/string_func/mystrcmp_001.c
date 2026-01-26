#include<stdio.h>
int mystrcmp(const char *s1,const char *s2);
int mystrcmp(const char *s1,const char *s2)
{
	int i = 0;//循环变量
	for(i = 0; s1[i] ==  s2[i] && s1[i] !='\0'; i++);//空循环，字符串不相同的时候跳出。
	return s1[i] - s2[i];//返回值ascll码差值
}

int main(void)
{
	char arr[10] = "hello";
	char *p = "word";
	printf("arr = %s\n",arr);

	mystrcmp(arr,p);//调用函数
	printf("mystrcmp = %d\n", mystrcmp(arr, p));
//	printf("arr = %s\n",arr);
	
	
	
	return 0;
}


