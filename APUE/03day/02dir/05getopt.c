#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	const char *optstring = "lhim";	//选项字符组成的字符串(可以根据自己的需求去定义)
	int ret = 0;	//用来接收返回值	

	while(1)//循环解析命令行选项
	{
		ret = getopt(argc, argv, optstring); //通过getopt(3)解析命令行参数
		if(ret == -1)	//判断是否解析完成
			break ;
	
		switch(ret)
		{
			case 'l' : printf("breakfast\n"); break;
			case 'h' : printf("lunch\n"); break;
			case 'm' : printf("dinner\n"); break;
			case 'i' : printf("supper\n"); break;
			case '?' : printf("i don't know\n"); break;
			default : break;
		}
		
	}

	return 0;
}



