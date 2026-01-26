#include<stdio.h>
int mystrcmp(const char *s1, const char *s2){
	int i = 0;
	for(i = 0;s1[i]==s2[i]&&s1[i]!='\0';i++)//空循环，字符串不同跳出
		return s1[i] - s2 [i];
}
int main(void)
{
	char str1[20];//定义字符串1
	char str2[20];//定义字符串2
	printf("请输入字符数组1\n");
	scanf("%s",str1);//数组本来存的就是地址
	printf("请输入字符数组2\n");
	scanf("%s",str2);
	//printf("%s,%s\n",str1,str2);
	mystrcmp(str1,str2);//mystrcmp函数
	if(mystrcmp(str1,str2) == 0)
		printf("两个字符串相同:%s = %s \n",str1,str2);
	else
		printf("两个字符串不同:str1 = %s,str2 = %s\n",str1,str2);
	return 0;
}




