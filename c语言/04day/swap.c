#include<stdio.h>
int swap(int *x, int *y)
{
	int tmp = 0;
	tmp = *x;
	*x = *y;
	*y = tmp;
}
int main(void)

{
	int a,b;
	scanf("%d-%d",&a,&b);
	swap(&a,&b);
	printf("%d   %d\n",a,b);
}
