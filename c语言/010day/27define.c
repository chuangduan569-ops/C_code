#include<stdio.h>
#define INIT_VAR(n) var##n
int main(void)
{
	int var1 = 100;
	int var2 = 200;
	int var3 = 300;
	int var4 = 400;
	int var5 = 500;
	
	printf("INIT_VAR = %d\n",INIT_VAR(1));
	printf("INIT_VAR = %d\n",INIT_VAR(2));
	printf("INIT_VAR = %d\n",INIT_VAR(3));
	printf("INIT_VAR = %d\n",INIT_VAR(4));
	printf("INIT_VAR = %d\n",INIT_VAR(5));
	printf("INIT_VAR = %d\n",INIT_VAR(2));
	return 0;
}
