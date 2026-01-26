/*
有1000发子弹 要提前装道10箱子里面，接收键盘输入，
要取多少颗子弹数，只能显示整箱的个数，
问这10个箱子怎么装
（定义一个数组10个元素，分别装子弹的个数，比如取走100发子弹 程序运行结果，比如2箱） 
二进制存储；
*/

#include<stdio.h>
void init(int box[],int size){
	int i = 0;
	int num = 1;
	int sum = 0;//计数
	for(i=0;i<size;i++){
		box[i] = num;
		num <<= 1;//num = num * 2与二进制左移一是一样的
		sum+=box[i];
		if(sum > 257)//因为只有100发子弹，所以256-512
			break;
	}
		box[i+1] = 1000 - sum;
}
void print(const int box[],int size){
		for(int i = 0;i<size;i++)
		{
			printf("box[%d] = %d\n",i,box[i]);
		}
}
void take_out(int box[10],int size,int bullet){
	int i = 0;	
	for(i = size-1;i>=0;i--){//从最后一个数往前判断,size-1因为数组下标是从个数减一开始的
		if(box[i] <= bullet){
			printf("box[%d] = %d\n",i,box[i]);//输出符合条件的箱子及对应箱子的子弹数
			bullet-=box[i];//总数减去已经取走的子弹
		}
	}
}
int main(void)

{
	int box[10] = {0};//存储子弹箱子
	int size = sizeof(box)/sizeof(*box);//箱子个数
	int bullet = 0;//子弹个数
	init(box,size);//存储函数,只和箱子有关，和子弹无关，不传子弹值
	print(box,size);//打印函数    函数用来判断存储函数的正确性，也就是十个箱子所对应的存储的子弹（二进制存储法）
					//可以把打印函数注释，不会展示出每个箱子存了多少子弹	
	printf("请输入你要取的子弹个数(1-1000):");
	scanf("%d",&bullet);
	if(bullet<=1000)
		take_out(box,size,bullet);//取子弹函数
	else
		printf("Error\n");
}
