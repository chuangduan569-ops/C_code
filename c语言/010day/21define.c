#include<stdio.h>
//#define SWAP(A,B) {(A)^=(B);(B)^=(A);(A)^=(B);}
//#define SWAP(A,B) {(A)=(A)+(B);(B)=(A)-(B);(A)=(A)-(B);}
#define SWAP(A,B) {typeof(A)(TMP);(TMP)=(A);(A)=(B);(B)=(TMP);}
int main(void)
{
	int a = 10;
	int b = 5;
	printf("a = %d b = %d\n",a,b);
	SWAP(a,b);
	printf("a = %d b = %d\n",a,b);

	return 0;
}
