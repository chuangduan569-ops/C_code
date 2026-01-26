#include<stdio.h>
#include<string.h>
//字符串转十进制
int myatoi(const char *nptr)
{
	int i = 0, op = 1,num = 0;
	 while(nptr[i] == ' ')
     i++;//判断空格
 if(nptr[i] == '+' || nptr[i] == '-')//判断正负
 {
     op = nptr[i] == '+' ? 1 : -1;
     i++;
 }

	if(nptr[i]==' '||nptr[i]=='+'||nptr[i]=='-'||(nptr[i]>='0'&&nptr[i]<='9')){
	//int i = 0, op = 1,num = 0;
	//while(nptr[i] == ' ')
	//	i++;//判断空格
	//if(nptr[i] == '+' || nptr[i] == '-')//判断正负
	//{
	//	op = nptr[i] == '+' ? 1 : -1;
	//	i++;
	//}
	for(;nptr[i] != '\0';i++)
	{
		if(nptr[i] == '+'||nptr[i] == '-')
			break;
		switch(nptr[i])
		{
			//case '0'...'9'
			//case '0' ... '9':num = num*10 + nptr[i] - '0';break;
			case '0' ... '9' : num *= 10; num += nptr[i] - '0'; break;//内核写法
			default : num*op;  
		}
	}
	return num*op;
	}
	else
	return 0;
}
int main(void)

	{
    int num = 0;
	char input[100];
    //num = atoi("123");123
    //num = atoi("abc");0
    //num = atoi("123abc");123
    //num = atoi("abc123");0
    //num = atoi("-123");-123
    //num = atoi("+123");123
    //num = atoi("123+456");123
    //num = atoi("0123");123
    //num = atoi("    123");123
	printf("请输入你的值：\n");
	fgets(input,sizeof(input),stdin);//  读取整行,缓冲区有换行符；
	input[strcspn(input, "\n")] = '\0';//string函数，去掉换行符

    num = myatoi(input);//123

    printf("num = %d\n", num);


	return 0;
}
