#include<stdio.h>
#include<math.h>//-lm
//若ax2+bx+c=0有实根，则求之。用户输入abc


int main(void)
{
	//三种情况
	//有相同根，有不相同两个根，无根,a!=0;a=0,不是一元二次方程
	int a = 0, b = 0, c = 0, t = 0;
	float x1 = 0, x2 = 0;
	printf("请输入a-b-c的值\n");
	scanf("%d-%d-%d",&a,&b,&c);
	if(a==0&&b==0&&c==0){
		printf("Error\n");
		return 0;		
	}
	if(a == 0)
    {
        printf("a不能为0，这不是一元二次方程！\n");
        return 0;
    }
	t = b*b - 4*a*c;
	if(t>=0)
	{
		if(t>0)
		{
			x1 = (-b + sqrt(t)) / (2*a);
			x2 = (-b - sqrt(t)) / (2*a);
			 printf("两个不同实根: x1=%.2f, x2=%.2f\n", x1, x2);
		}
	else
	{
		x1 = x2 = (float)(-b) / (2*a);
		printf("重根: x1=x2=%.2f\n", x1);  
	}
	}
	else
		printf("无实根\n");
}
