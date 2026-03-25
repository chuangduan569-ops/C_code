#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


int main(int argc, char *argv[])

{
	struct stat fs;//用来存储到的文件元信息
	int ret = 0;	
	DIR *dp = NULL;//指向打开的目录流
	struct dirent *entry = NULL;//entry指针指向目录项结构

	if(argc < 2)
	{
		fprintf(stderr,"Usage : %s + filename\n", argv[0]);
		ret = -1;
		goto ERR_1;
	}
	
	if(stat(argv[1], &fs) == -1)	//判断获取文件元信息是否失败
	{
		perror("stat()");
		ret = -2;
		goto ERR_1;	
	}
	//判断是否不是目录文件
	if(!S_ISDIR(fs.st_mode))
	{
		printf("%s Not A Directory\n", argv[1]);//打印提示信息
		ret = -3;
		goto ERR_1;
	}

	dp = opendir(argv[1]);//打开arg[1]的目录文件
	if(dp == NULL)
	{
		perror("opendir()");
		ret = -4;
		goto ERR_1;
	}

	while(1)//循环读取目录项结构
	{
		errno = 0;//为了防止errno存储其他的错误码，进行清空处理
		entry = readdir(dp);//读取目录流中的目录项结构
		if(entry == NULL)	//是否读取结束或者读取失败
		{
			if(errno != 0)
			{
				perror("readdir()");
				ret = -5;
				goto ERR_2;
			}
			break;//跳出死循环
		}
		printf("%ld-%s\n",entry->d_ino, entry->d_name);	//打印inode号和子文件名
	}

ERR_2 :
	closedir(dp);//关闭目录流
ERR_1 :
	return ret;
}






