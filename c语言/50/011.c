#include<stdio.h>
int main(void)
{
	char a,b,c;
	for(a='x';a<='z';a++){
		for(b='x';b<='z';b++){
			for(c='x';c<='z';c++){
					if(a!=b&&a!=c&&b!=c){
									if(a!='x'&&c!='x'&&c!='z')
									{

									printf("a的对手是:%c\n",a);
									printf("b的对手是:%c\n",b);
									printf("c的对手是:%c\n",c);
									}
					}
				}
		}

	}
	//printf("\n");
	return 0;
}
