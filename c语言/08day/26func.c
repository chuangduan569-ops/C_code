#include<stdio.h>
void itoa(int num, char *buf)
{
	int i = 0;
	if(num<10)
	{
		buf[0] = num + '0';
		buf[1] = '\0';
		return ;
	}
	itoa(num/10,buf);//递归调用
	for(i=0;buf[i]!='\0';i++);//空循环，找出\0的位置
	buf[i] = num % 10 + '0';
	buf[i+1] = '\0';
}
int main(void)
{
	int num = 0;//存储录入的十进制整数
	char buf[10] = {0};//存储转换后的字符串	
	scanf("%d",&num);
	itoa(num,buf);
	printf("buf = %s\n",buf);
}
