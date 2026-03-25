#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[] = "dc:x:1000:1000:dc,,,:/home/dc:/bin/bash";
	char *str = buf;	//使用str代替buf做操作
	char *ch = ":";	//存储分割的字符串
	char *p = NULL;	//存储解析出来的字串

	while(1)
	{
		p = strsep(&str, ch);	//通过strsep(3)分割字符串
		if(p == NULL)//判断是否分割完毕
			break;	//由于分割完毕，跳出死循环
		printf("%s\n",p); //打印分割的字串
	}


	return 0;
}
