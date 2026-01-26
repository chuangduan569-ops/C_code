#include<stdio.h>
typedef struct player
{
	char name[32];
	int zhili;
	int wuli;
}JY;
void print(JY p)
{
	printf("姓名 ： %s\n",p.name);
	printf("武力 ： %d\n",p.wuli);
	printf("智力 ： %d\n",p.zhili);
}
void change(JY *p)
{
	scanf("%s",p->name);
	scanf("%d",&p->wuli);
	scanf("%d",&p->zhili);
}
int main(void)
{
	JY p = {"guojing",95,95};

	return 0;
}
