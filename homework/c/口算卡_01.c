//#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>


int jia(int a, int b){return a + b;}//加法函数
int jian(int a, int b){return a - b;}//减法函数
int cheng(int a, int b){return a * b;}//乘法函数
int shang(int a, int b){return b!=0 ? a/b : 0; }//取商函数
int yu(int a, int b){return b!=0 ? a%b : 0; }//取余函数
//定义一个数组，存储加减乘除
char syms[5] = {'+', '-', '*', '/', '%'};



//定义一个函数指针去做运算
int (*func[5])(int,int) = {jia, jian, cheng, shang, yu};//函数指针数组，指针指向哪里，做那个运算
//函数指针数组只是声明和初始化，不是调用注意：这里只是存储函数的地址，并没有调用函数，所以不需要传参。




//用于计算的函数
int calate_randow(int a, int b, char *op)
{
	int i = 0;//用来接收随机变量
	i = rand()%5;//随机1～5数字
	*op = syms[i];//用随机值来接受符号地址
	return func[i](a,b);//返回指针数组，去执行指针数组的函数

}




int main(void)
{
	int num = 0;
	int i = 0, a = 0, b = 0;
	char op;
	int answer = 0;
	int result = 0;
	int score = 0;
	srand(getpid());//获取随机因子
	printf("请输入你想做多少题\n");
	scanf("%d",&num);
	for(i=1;i<=num;i++)
	{
		a = rand()%100;//一百以内的随机数
		b = rand()%100;//一百以内的随机数
		result = calate_randow(a,b,&op);//调用函数
		printf("第%d道题:\n%d %c %d = ",i,a,op,b);//打印题目
		
		scanf("%d",&answer);//接收用户的输入的值
		if(answer == result)
		{
			printf("Bingo  +10分\n");
			score += 10;
		}
		else
			printf("Error\n");
	
	}
		printf("总分：	%d\n",score);
	
	
	return 0;
}





