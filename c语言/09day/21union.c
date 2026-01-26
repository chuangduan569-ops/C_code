#include<stdio.h>
union U
{
	int i;
	float f;
};

int main(void)
{
	union U u;
//	u.i = 100;
	u.f = 3.14;
	printf("u.f = %f\n",u.f);
	printf("u.i = %d\n",u.i);



	return 0;
}
