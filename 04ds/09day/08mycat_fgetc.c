#include <stdio.h>

static int mycat(const char *pathname)
{
	FILE *fp = NULL;
	int ch = 0;//ch变量存储从文件流中读取的字符
	
	
	fp = fopen(pathname, "r");//通过fopen(3)以r的形式打开文件
	if(fp == NULL)
	{
		perror("fopen()");//打印错误信息
		return -1;//由于打开文件失败vai，结束函数，并且返回-1
	}

	while(1)//循环读写文件
	{
		ch = fgetc(fp);
		if(ch == EOF)//判断文件是否读到了EOF
		{//因为读到文件结尾会返回EOF，读取错误也会返回EOF
		if(ferror(fp))//判断fp指向的文件流是否出错
			//如果返回非0说明出错,没有到达文件结尾
		{
			fclose(fp);//关闭fp指向的文件流
			return -2;//由于fp指向的文件流出错，结束函数，并且返回-2	
		}
		//	读到了文件结尾，跳出死循环;
		break;
		}
		//int fputc(int c, FILE *stream)
		//把形参c存储的字符写入到stream文件流指针指向的文件中
		fputc(ch,stdout);//把ch存储的字符数据写入到stdout中
	
	}

	fclose(fp);//关闭fp指向的文件流

	return 0;

}



int main(int argc, char *argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"usag: %s + filename\n", argv[0]);//打印使用说明
		return -1;//由于命令行参数个数少于2个，结束程序，并且返回-1
	}
	
	mycat(argv[1]);//调用实现的内部函数，完成cat的功能
	return 0;
}
