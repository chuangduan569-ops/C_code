//输入一行英文，已知各单词之间用1个空格相隔（设第一个单词前没有空格）
//统计这行英文有多少个单词。
#include<stdio.h>

int main(void)
{
	char c;
	int sum = 0;
	int last_char;//记住上一个字符
	printf("请输入单词，中间用空格个开\n");
	c = getchar();//读取第一个字符
	if(c != '\n'){	
		last_char = c;//记住字符，用于下次对比
		if(c != ' ')
			sum = 1;//判断第一个字符是否为空格
		
	//进入循环，开始读取后面的，并和前一个作对比
	while((c=getchar()) != '\n')
	{
		//当前是字母，且上一个是空格，就是新单词
		if( c!=' ' && last_char ==' ')
			sum++;
		last_char = c;//记住字符。进入下一次循环作对比
	}
}
/*	while((c = getchar()) != '\n' )
	{	
		if(c == ' ')
			sum++;
	}
*/
	printf("一共有%d个单词\n",sum);


	

	return 0;
}
