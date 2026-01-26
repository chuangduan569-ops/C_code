#include<stdio.h>
//输入一行英文，已知各单词之间用1个空格相隔（设第一个单词前没有空格）
//统计这行英文有多少个单词。
int main(void)
{
	int sum = 0;
	char c;
	int last_char;//用来记住上一个字符是否为空格
	printf("请输入一行英文，各单词之间用1个空格相隔\n");
	c = getchar();
	if(c != '\n'){//不等于换行符进入循环，循环前先判断首个是否为空格，并且记录，用于下次对比
		last_char = c;//记住字符，用于下次作比较
			if(c != ' ')
			sum = 1;//判断第一个字符是否为空格
	while((c = getchar()) != '\n')
	{
		if(c != ' ' && last_char == ' ')
			sum++;
		last_char = c;//记住字符，用于下一次循环判断
	}
		printf("%d个单词\n",sum);
		



	}
	return 0;
}



