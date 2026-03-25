#include <stdio.h>

static int mycp(const char *srcfile, const char *destfile)
{
	FILE *fps = NULL;
	FILE *fpd = NULL;
	int ch = 0;
	
	fps = fopen(srcfile, "r");
	if(fps == NULL)
	{
		perror("fopen()");
		return -1;
	}

	fpd = fopen(destfile, "w");
	if(fpd == NULL)
	{
		perror("fopen");
		fclose(fps);
		fps = NULL;
		return -2;
	}

	while(1)
	{
		ch = fgetc(fps);//从源文件读取数据
	if(ch == EOF)
	{
		if(ferror(fps))//
		{
			fclose(fpd);
			fpd = NULL;
			fclose(fps);
			fps = NULL;
			return -3;
		}
		break;
	}
		fputc(ch,fpd);
	}
}



int main(int argc, char *argv[])
{

	if(argc < 3)//判断命令行参数的个数是否少于3个
	{
		printf("Usage:< %s srcfile + destfile>\n",argv[0]);
		return -1;
	}
	
	mycp(argv[1],argv[2]);

	return 0;
}




