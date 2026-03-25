#include <stdio.h>

extern char **environ; //全局变量，接收main函数的第三个参数

int main(int argc, char *argv[])
{
	int i = 0;
	printf("argc = %d\n", argc);

	for(i = 0; i < argc; i++)
		puts(argv[i]);
	printf("\n");

	for(i = 0; environ[i] != NULL; i++)
		puts(environ[i]);
	printf("\n");


	return 0;
}





