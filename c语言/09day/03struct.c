#include<stdio.h>
struct stu
{
	int id;
	char name[20];
	int math;
};


int main(void)

{

	struct stu s;
	
	s.id = 10086;
	//s.name = "10086";//不可以，不能给数组名做i赋值运算
	//strcpy(s.name,"yidong")
	sprintf(s.name, "yidong");
	s.math = 60;

	printf("ID = %d\n",s.id);
	printf("NAME = %s\n",s.name);
	printf("MATH = %d\n",s.math);
	
	scanf("%d",&s.id);//&是作用在成员 id 上的
	scanf("%s",s.name);
	scanf("%d",&s.math);//&是作用在成员 math 上的
	
	printf("ID = %d\n",s.id);
	printf("NAME = %s\n",s.name);
	printf("MATH = %d\n",s.math);


	return 0;
}
