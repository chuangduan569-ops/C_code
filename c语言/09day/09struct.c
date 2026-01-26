#include<stdio.h>
#include<string.h>

struct aaa
{
	int id;
	char name[20];
	int math;
};
struct bbb
{
    int id;
    char name[20];
    int math;
};

int main(void)
{
	struct aaa a;
	struct bbb b;

	a.id = 10010;
	strcpy(a.name,"liantong");
	a.math = 19;

	b.id = 10010;
 	strcpy(b.name,"yidong");
	b.math = 59;
	
	printf("a => %d %s %d\n",a.id,a.name,a.math);
	printf("b => %d %s %d\n",b.id,b.name,b.math);

	memcpy(&a,&b, sizeof(a));
	
	printf("a => %d %s %d\n",a.id,a.name,a.math);
	printf("b => %d %s %d\n",b.id,b.name,b.math);

	return 0;

}
