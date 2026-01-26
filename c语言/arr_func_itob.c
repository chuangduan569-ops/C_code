#include<stdio.h>
void itob(int *buf,int size,int num){//int *buf可以替换成buf[] buf[16]
	int i = 0;
	for(i=size-1;i>=0;i--)
	{
		buf[i]=num%2;//存储二进制（因为系统是倒着输出的）
		num/=2;//
	}
}

void print(const int buf[],int size){
	int i = 0;
	for(i=0;i<size;i++){
		printf("%d",buf[i]);
	}
	printf("\n");
	
}
int main(void)
{
	int buf[16]={0};//定义数组用来存储二进制数据
	int num = 0;//用来接收用户的值
	int size = sizeof(buf)/sizeof(*buf);//存储数组成员个数
	printf("请输入要转换的十进制数\n");
	scanf("%d",&num);//用来录入客户输入的十进制数据
	
	itob(buf,size,num);
	print(buf,size);


	return 0;
}
