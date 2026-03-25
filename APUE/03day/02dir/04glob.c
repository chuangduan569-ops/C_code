#include <stdio.h>
#include <glob.h>



int main(int argc,char *argv[])
{
	glob_t gs;	//gs变量用于存储解析到目录个数以及目录名
	int i = 0;

//	if(glob("/etc/*", 0, NULL, &gs) != 0)	//判断解析目录是否失败
	if(glob("/home/dc/*", GLOB_NOSORT, NULL, &gs) != 0)
	{
		fprintf(stderr,"glob() Is Failed!\n");	//打印错误信息
		return -1;
	}

	glob("/home/dc/.*", GLOB_APPEND, NULL, &gs);  //追加解析的隐藏文件	

	for(i = 0; i < gs.gl_pathc; i++)	//循环遍历目录中的子文件
	{
		printf("%s\n", gs.gl_pathv[i]);	//
	}
	
	globfree(&gs);	//释放glob(3)开辟的空间

	return 0;
}




