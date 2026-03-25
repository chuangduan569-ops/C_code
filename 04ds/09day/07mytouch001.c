#include <stdio.h>

int mytouch(const char *pathname)
{
	FILE *fp = NULL;//fp指向打开的文件
	
	fp = fopen(pathname, "a+");//通过fopen(3)以a+的形式打开文件
	if(fp == NULL)
	{
		perror("fopen()");//打印错误信息
		return -1;//由于打开文件失败，结束函数，并且返回-1
	}
	
	fclose(fp);//关闭文件
	fp = NULL;

	return 0;
}


int main(int argc, char *argv[])

{
	if(argc < 2)
	{
		fprintf(stderr,"%s :缺少要操作的文件\n",argv[0]);
		fprintf(stderr,"清尝试执行 \"%s --help \" 来获取更多的信息\n", argv[0]);
		return -1;//由于命令行参数个数少于2个，结束程序，并且返回-1
	}
	
	for(int i = 1; i < argc; i++)
	 mytouch(argv[i]);//调用自己实现的mytouch()进行创建文件


	return 0;
}



