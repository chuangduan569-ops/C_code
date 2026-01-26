#include<stdio.h>
int main(void)
{	//判断三角形类型
	int a,b,c;
	printf("请输入三角形三边长( - - )\n");//输入三角形三边长
	scanf("%d-%d-%d",&a,&b,&c);
	if(a+b>c&&a+c>b&&b+c>a){
		printf("可以构成三角形\n");
		if(a==b||a==c||b==c){
			printf("等腰三角形\n");
			if(a==b&&a==c){
			printf("等边三角形\n");
			}
		}
		if(a*a+b*b==c*c||a*a+c*c==b*b||a*a+c*c==b*b)
			printf("直角三角形\n");
	}
	else
		printf("Error\n");

	return 0;
}



