#include<stdio.h>
char *mystrcat(char *dest,const char *src);
char *mystrcat(char *dest, const char *src)
{
	int i = 0, j = 0;//循环变量
	for(i = 0; dest[i] != '\0'; i++);//空循环，到'\0'跳出循环。目的就是找到dest的'\0'位置，接着把p拼接上
	for(j = 0; src[j] != '\0'; j++)
		dest[i+j] = src [j];//拼接所有的有效字符，但不包含'\0'
	dest[i+j] = src[j];//拼接上'\0'
	return dest;


}

int main(void)
{
	char arr[10] = "hello";
	char *p = "word";
	printf("arr = %s\n",arr);


	mystrcat(arr,p);//调用函数
	printf("arr = %s\n",arr);

	return 0;
}



