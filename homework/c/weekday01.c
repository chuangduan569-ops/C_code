#include<stdio.h>
//1.输入一行字符，分别统计出其中英文字母、空格、数字和其它字符的个数。
int main(void)
{
	char arr;
	int english = 0, kongge = 0, num = 0,other = 0;
	while((arr = getchar()) != '\n')//getchar()从键盘输入每次读取一个字符串
	{
		if(arr == ' ')
			kongge++;
		else if((arr >= 'a'&&arr <= 'z')||(arr >='A'&&arr <='Z'))
			english++;
		else if(arr >= '0'&&arr <= '9')
			num++;
		else
			other++;
	}

	printf("english = %d,num = %d,kongge = %d,other = %d\n",english,num,kongge,other);
	return 0;

}



