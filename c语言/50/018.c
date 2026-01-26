#include<stdio.h>
//输入一行字符，分别统计出其中英文字母、空格、数字和其它字符的个数。
int main(void)
{

	char c;
	int num = 0,english = 0,kongge = 0,other = 0;
	//getchar每次只能读取一个字节，读取到换行符结束
	while((c = getchar()) != '\n')
	{
		//		printf("%c",c);//这里测试不要打换行，换行就连不到一块
		if(c == ' ')
			kongge++;
		else if((c >='a'&&c<='z')||(c >='A'&&c<='z'))
			english++;
		else if(c >='0'&& c<='9')
			num++;
		else
			other++;
	}
	printf("数字:%d 空格:%d 字母:%d 其他:%d\n",num,kongge,english,other);
	return 0;
}



