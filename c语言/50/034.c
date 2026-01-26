#include<stdio.h>
//有三个小孩，
//一个比一个大2岁，
//已知其年龄之和为39，问这三个小孩各几岁？
int main(void)

{
	int i = 0;
	int a = 0, b = 0, c = 0;
	for(i=0;i < 39; i++){
		a = i + 2;
		b = a + 2;
		c = b + 2;
		if(a+b+c==39)
		printf("%d	%d	%d\n",a,b,c);
	}

	return 0;
}


