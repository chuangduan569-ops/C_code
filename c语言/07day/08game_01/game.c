#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "game.h"
char name[32];
int wuli;
int zhili;

void init(void)
{
//	char name[32];
//	int wuli;
//	int zhili;

	srand(getpid());
	printf("名字\n");
	scanf("%s",name);
	wuli = rand()%100;
	zhili = rand()%100;
}
void print(void)
{
	printf("姓名：%s\n",name);
	printf("wuli：%d\n",wuli);
	printf("zhili：%d\n",zhili);
}
