#include<stdio.h>
//从键盘输入长度不等的两个字串，将短串连接于长串之后输出。
size_t mystrlen(const char *s1)//字符串长度函数
{
	int i = 0;//定义循环变量
	for(i = 0;s1[i]!='\0';i++)//空循环，到\0跳出
	{
	}
	return i;//返回值到i
}
char *mystrcat(char *dest,char *src){//字符串拼接函数
	int i = 0, j = 0;//定义循环变量
	for(i = 0;dest[i]!='\0';i++);//空循环，目的为了找出dest的\0
	for(j = 0;src[j]!='\0';j++)
		dest[i+j] = src[j];//拼接有效字符
	dest[i+j]=src[j];
		
	return dest;

	
}
	int main(void)
	{	
		int long1 = 0;//数组1长度
		int long2 = 0;//数组2长度
		char str1[20];//定义数组1
		char str2[20];//定义数组2
		printf("请输入数组1\n");
		scanf("%s",str1);//从键盘存储到数组
		printf("请输入数组2\n");
		scanf("%s",str2);//从键盘存储到数组
		long1 = mystrlen(str1);
		//	printf("%d",long1);验证long1结果正确
		long2 = mystrlen(str2);
		if(long1<long2){
			mystrcat(str1,str2);//调用函数；str2连接str1
			printf("%s\n",str1);//打印最终值
		}
		else
		{
			mystrcat(str2,str1);//调用函数str1连接在str2
			printf("%s\n",str2);//打印最终值
		}
		return 0;
	}
