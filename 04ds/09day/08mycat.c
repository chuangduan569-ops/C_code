#include <stdio.h>

static int mycat(const char *pathname)
{
	FILE *fp = NULL;//fp
	int ch = 0;


	fp = fopen(pathname, "r");
	if(fp == NULL)
	{
		perror("fopen");
		return -1;
	}
	while(1)//循环读写文件
	{
		ch = fgetc(fp);
		if(ch == EOF)//判断是否读取到EOF
		{//
			if(ferror(fp))
			{
				fclose(fp);
				return -2;
			}
			break;//读到文件结尾，跳出死循环
		}
		fputc(ch,stdout);//把ch存储的字符数据写到标准输出上
	}
	fclose(fp);
	fp = NULL;

}



int main(int argc, char *argv[])
{

}



