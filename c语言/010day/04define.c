#include<stdio.h>
#define STM32F103C8T6
int main(void)
{
	int PIN = 0;//PIN用表示芯片的管教
#if defined(STM32F103RBT6)
	PIN = 64;
#elif defined(STM32F103C8T6)
	PIN = 32;
#endif
	printf("PIN = %d\n",PIN);
	return 0;
}
