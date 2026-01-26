#include<stdio.h>
#define PRINT_STR(n) #n
int main(void)
{
	printf("PRINT_STR = %s\n",PRINT_STR(9527));
	printf("PRINT_STR = %s\n",PRINT_STR(3.1415926));
	printf("PRINT_STR = %s\n",PRINT_STR(0x12345678));
	printf("PRINT_STR = %s\n",PRINT_STR(123+456-789));
	printf("PRINT_STR = %s\n",PRINT_STR(段闯));
	printf("PRINT_STR = %s\n",PRINT_STR(hello 段闯));

	return 0;
}
