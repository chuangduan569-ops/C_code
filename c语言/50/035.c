//键盘输入长方体的长宽高l、w、h，求其体积及表面积。
//要求用自定义函数val(l,w,h)计算长方体的体积和表面积，计算结果使用全局变量带回主函数。
#include<stdio.h>
int val(int l, int w, int h)
{
	int s = 0, v = 0;
	s = 2 * (l*w + l*h + w*h);
	v = l * w * h;
	printf("s = %d	",s);
	printf("v = %d\n",v);
}


int main(void)
{
	int l = 0, w = 0, h = 0;
	int result = 0;
	printf("请输入长宽高( - - )\n");
	scanf("%d-%d-%d",&l,&w,&h);
	val(l,w,h);//调用函数
	return 0;
}


