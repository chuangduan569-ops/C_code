#include<stdio.h>
/*从键盘输入两个字串，输出其中较短的那个字串，并输出它的长度。*/
size_t mystrlen(const char *s){
	size_t i = 0;
	for(i = 0; s[i]!='\0';i++)//空循环
		{

		}
		return i;
}

int main()
{	
	int long1 = 0;
	int long2 = 0;
	char str1[20];//定义字符串1
	char str2[20];//定义字符串2
	printf("请输入数组1\n");
	scanf("%s",str1);//字符串数组存的就是地址,限制最多输入九位
	printf("请输入数组2\n");
	scanf("%s",str2);//字符串数组存的就是地址
	long1 = mystrlen(str1);//数组1的长度
	long2 = mystrlen(str2);//数组2的长度
	if(long1>long2)
		printf("str2 = %s,字符长度为:%d\n",str2,long2);
	else if(long1==long2)
		printf("str1:%s = str2:%s,字符长度为:%d\n",str1,str2,long2);
	else if(long1<long2)
		printf("str1 = %s,字符长度为:%d\n",str1,long1);

		
	
	
	return 0;
}




