
#include<stdio.h>
char *mystrcpy(char *dest,const char *src);
char *mystrcpy(char *dest, const char *src)
{
	int i = 0;//循环变量
	for(i = 0; src[i] != '\0'; i++)//遍历所有数组（这里不是空循环）
		dest[i] = src[i];//复制所有字符，但不包含'\0'
	dest[i] = src[i];//跳出循环后，复制'\0'.因为strcpy(3)包含'\0'
	return dest;
}


int main(void)
{
	char arr[10] = "hello";
	char *p = "word nihao";
	printf("arr = %s\n",arr);
	

	mystrcpy(arr,p);
	printf("arr = %s\n",arr);

	return 0;
}
