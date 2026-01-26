#include<stdio.h>

int a = 9527;

void func(void)
{
		printf("%s->a = %d\n",__FUNCTION__,a);
}
int main(void){
	int a =12345;
	printf("%s->a = %d\n",__FUNCTION__,a);
	func();
	return 0;
}
