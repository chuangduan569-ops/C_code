#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//五个函数的运算
int add(int a,int b){return a + b;}
int jian(int a,int b){return a - b;}
int cheng(int a,int b){return a * b;}
int chu(int a,int b){return b !=0 ? a / b : 0;}
int mo(int a,int b){return b!= 0 ? a % b : 0;}

//随机运算符  全局函数指针数组
//int op(){return rand()%5}
int (*func[5])(int,int)={add,jian,cheng,chu,mo};//指针函数// 指向add函数（加法）指向jian函数（减法）指向cheng函数（乘法） 指向chu函数（除法）指向mo函数（取模）
char syms[5]={'+','-','*','/','%',};

//函数,运算
	int calculate_randow(int a,int b,char *op){
	int i = rand()%5;//每次调用函数
	*op=syms[i];//取数组地址符号
	return func[i](a,b);//返回随机运算符的随机取得的运算符,并把ab值赋值
}


int main()
{
	srand(getpid());
	int score = 0;
	for(int i = 1;i<=10;i++)
	{
		int a = rand()%100+1;
		int b = rand()%100+1;
		char op;
		int correct = calculate_randow(a,b,&op);
		printf("第%d题：\n%d %c %d = ",i,a,op,b);
		int answer;
		scanf("%d",&answer);
		if(answer == correct){
			printf("Bingo +10分\n");
			score+=10;
		}
		else
			printf("Error\n");
	}
		printf("总分:%d\n",score);
		return 0;


}






