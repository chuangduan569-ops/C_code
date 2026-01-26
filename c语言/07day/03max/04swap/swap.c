#include "swap.h"

int swap(int *a,int *b)
{
	int t = 0;
	t = *a;
	*a = *b;
	*b = t;
}
