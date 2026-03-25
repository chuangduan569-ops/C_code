#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024
#define FREAD


static int mycp(const char *srcfile, const char *destfile)
{
	FILE *fps = NULL;//fps指向源文件
	FILE *fpd = NULL;//fpd指向目标文件
	int ch = 0;//ch变量存储从文件流中读取的数据
	char buf[BUFSIZE];
	size_t count = 0;//接收成功读到的成员个数
	
	fps = fopen(srcfile, "r");//通过fopen(3)以r的形式打开源文件
	if(fps == NULL)
	{
		perror("fopen()");//打印错误信息
		return -1;//由于打开源文件失败，结束函数，并且返回-1
	}

	fpd = fopen(destfile, "w");//通过fopen(3)以w的形式打开目标文件
	if(fpd == NULL)//判断打开目标文件是否失败
	{
		perror("fopen()");//打印错误信息
		fclose(fps);//关闭源文件的文件流
		return -2;//由于打开文件失败，结束函数，并且返回-2
	}

	while(1)//循环读取数据
	{
#if defined(FGETC)
		ch = fgetc(fps);//从源文件流中读取数据
		if(ch == EOF)//判断是否读到了文件末尾或者读取错误
		{
			if(ferror(fps))//返回非0说明出错
			{
				fclose(fpd);//关闭目标文件的文件流
				fclose(fps);//关闭源文件的文件流
				return -3;//由于读取错误，结束函数，并且返回-3
			}	
		break;//读到了文件末尾，跳出循环
		}
		fputc(ch,fpd);//把ch存储的数据写入到目标文件的文件流中
#elif defined(FGETS)
	memset(buf,0,BUFSIZE);
	if(fgets(buf,BUFSIZE,fps) == NULL)
		break;
	fputs(buf,fpd);
#elif defined(FREAD)
	memset(buf, 0, BUFSIZE);
	if(count = fread(buf, 1, BUFSIZE, fps) == 0)
	{
		if(ferror(fps))//非0出错，没有到达末尾
		{
			fclose(fpd);
			fclose(fps);
			return -3;
		}
		break;
	}
	fwrite(buf,1,BUFSIZE,fpd);

#endif
	}

	fclose(fpd);
	fclose(fps);
	return 0;

}




int main(int argc, char *argv[])

{
	if(argc < 3)
	{
		fprintf(stderr, "usag : %s + srcfile + destfile\n",argv[0]);
		return -1;//由于命令行参数的个数少于三个，结束程序，并且返回-1
	}

	mycp(argv[1], argv[2]);//调用实现的内部函数，完成cp的命令功能

	return 0;
}
