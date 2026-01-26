#include<stdio.h>
#include<math.h>
int main()

{	
	int n;
	int count = 0;
	float s;
	printf("请输入\n");
	scanf("%d",&n);
	for(int i =1;i<n;i++)
	{
		if(i%3==0&&i%7==0)
		{
			count = count + i;
			//printf("i=%d\n count:%d",i,count);
		}
	}
	s = sqrt(count);

	printf("s = %f\n",s);
	return 0;//编译用gcc 文件名 -lm 
}


