//有1000个CPU,要提前装到10个箱子里面,将来想要取CPU的人通过键盘录入想要取走CPU的个数,都可以整箱的给这个人,编写程序实现10个箱子的存储,以及取CPU的过程
#include<stdio.h>
void init(int box[], int size)
{	
	
	int i = 0;//变量
	int num1 = 1;//第一个箱子数量,以此类推
	int	sum = 0;//计算总数
	for(i=0;i<size;i++)
		{
			box[i] = num1;
			num1 <<= 1;//补码坐移，2的进制
			sum += box[i];
			if(sum>500)//只要大于255就符合
				break;
		}
		box[i+1] = 1000 - sum;//第十个箱子
}
void print(int box[],int size)
{
	int i = 0;
	for(i=0;i<size;i++){
		printf("b[%i] : %d\n",i,box[i]);//依次打印十个箱子存储的cpu
	}
}
void take_out(int box[],int size,int num){
	int i = 0;//循环变量
	for(i=size;i>0;i--)//从最后一个数往前判断,size-1因为数组下标是从个数减一开始的
		if(box[i]<=num){
			printf("box[%d] : %d\n",i,box[i]);
			num = num - box[i];
		}
		
}




int main(void)

{
	int box[10] = { 0 };
	int num = 0;//用来接受用户取走cpu个数
	int size = sizeof(box) / sizeof(*box);
	init(box,size);//存入箱子函数
	print(box,size);//打印箱子存储数量函数
	printf("请输入你要取走的个数:\n");
	scanf("%d",&num);//取地址

	if(num <= 1000)
		{
			take_out(box,size,num);//取cpu函数
		}
	else
		printf("没有那么多，爱要不要！\n");
	
	


	return 0;
}



