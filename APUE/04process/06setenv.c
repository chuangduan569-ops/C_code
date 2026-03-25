#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	setenv("TEST","Are you kidding?", 1);//添加一个环境变量
	
	puts(getenv("TEST"));

	return 0;
}


