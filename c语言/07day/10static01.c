#include<stdio.h>
void fun1(void)
{
	int i = 0;
		i++;
	printf("i = %d\n",i);
}
void fun2(void)
{
    static int i = 0;
        i++;
    printf("i = %d\n",i);
}
int main(void)
{
	fun1();
	fun1();
	fun1();
	fun1();
	fun1();
	printf("--------------------------\n");
	fun2();
	fun2();
	fun2();
	fun2();
	fun2();
	


}
