#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
int da(int n1, int num,int count)
{
	if(n1 < num)
	{
		printf("第%d次:您输入的值大了\n",count);
	}
	return 0;
}
int xiao(int n1, int num, int count)
{
	if(n1 > num)
	{
		printf("第%d次:您输入的值小了\n",count);
	}
	return 0;
}
int deng(int n1, int num, int count)
{
	if(n1 == num)
	{
		printf("第%d次：恭喜您，猜对了\n",count);
	}
	return 0;
}
int main(void)
{	
	srand(getpid());//调用进程获取随机种子
	int num = 0;//用来接受用户输入的值
	int n1 = 0;//用来接收随机种子的值
	int count = 1;//用来计算次数
	printf("请输入您的数字(1000以内)\n");
	n1 = rand()%1000;//1000以内的随机数
	printf("%d\n",n1);
	for(;;)
	{
		scanf("%d",&num);
		da(n1,num,count);
		xiao(n1,num,count);
		deng(n1,num,count);
		
		if(n1 == num)
		{
			break;//猜对跳出循环	
		}
		
		count++;	
		
	
	}
	//scanf("%d",num);
	//da(n1,num);//调用函数
	//xiao(n1,num);//调用函数
	//deng(n1,num);//调用函数
	

	
	
	return 0;
}


