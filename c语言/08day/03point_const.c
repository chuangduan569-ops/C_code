#include<stdio.h>

int main(void)
{
	int a = 9527;
	int * const p =&a;
	printf("a = %d &a = %p\n",a,&a);
	printf("*p = %d p = %p\n",*p,p);

	*p = 12345;
	
	printf("a = %d &a = %p\n",a,&a);
	printf("*p = %d p = %p\n",*p,p);
	
	return 0;
}
