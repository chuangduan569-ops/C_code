#include<stdio.h>
//从键盘输入长度不等的两个字串，将长串连接于短串之后输出。
size_t mystrlen(const char *s)
{
	int i = 0;
	for(i=0;s[i]!='\0';i++);//空循环，计算长度
	return i;//返回值即是长度
}
char *mystrcat(char *dest, char *src)
{
	int i = 0, j = 0;
	for(i = 0; dest[i] != '\0'; i++);//空函数，找到dest的\0并跳出
	for(j = 0; src[j] != '\0'; j++)//便利src所有字符
		dest[i+j] = src[j];//拼接字符，但也\0
	dest[i+j] = src[j];//	\0
	return dest;
}

int main(void)

{
	int long1 = 0;//用来接收数组1的长度
	int long2 = 0;//用来接收数组2的长度
	char str1[20];//定义一个数组1
	char str2[20];//定义一个数组2
	printf("请输入数组1\n");
	scanf("%s",str1);
	printf("请输入数组2\n");
	scanf("%s",str2);//从键盘赋值
	long1 = mystrlen(str1);//调用数组的长度函数
	long2 = mystrlen(str2);
	if(long1>long2)//判断条件
	{
		mystrcat(str2,str1);//把长串连接在短串后面
		printf("%s\n",str2);
	}
	else
	{

	mystrcat(str1,str2);//把长串连接在短串后面
	printf("%s\n",str1);
	}


	return 0;
}
