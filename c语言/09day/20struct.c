#include<stdio.h>
typedef struct player
{
	char name[32];
	int wuli;
	int zhili;
}JY;

void print(JY *p)
{
	int i = 0;
	for(i = 0;i<3;i++){
		printf("NAME = %s\n",p[i].name);
		printf("WULI = %d\n",p[i].wuli);
		printf("ZHILI = %d\n",p[i].zhili);
	}
}

void change(JY *p)
{
	int i = 0;
	for(i=0;i<3;i++){
		scanf("%s",(p+i)->name);
		scanf("%d",&(p+i)->wuli);
		scanf("%d",&(p+i)->zhili);
	}
}
int main(void)
{
	JY arr[3] = {{"ouyangfeng",66,66},{"duanyu",77,77},{"xuzhu",88,88}};
	print(arr);

	change(arr);

	print(arr);

	return 0;
}
