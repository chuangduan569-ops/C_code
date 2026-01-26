#include<stdio.h>
void iton(int n1, int n2)
{	int tmp = n2;
	switch(n1)
	{
	case 2 : while(tmp){printf("%d ", tmp % 2);tmp /= 2;}printf("\n");break;
	case 3 : while(tmp){printf("%d ", tmp % 3);tmp /= 3;}printf("\n");break;
	case 4 : while(tmp){printf("%d ", tmp % 4);tmp /= 4;}printf("\n");break;
	case 5 : while(tmp){printf("%d ", tmp % 5);tmp /= 5;}printf("\n");break;
	case 6 : while(tmp){printf("%d ", tmp % 6);tmp /= 6;}printf("\n");break;
	case 7 : while(tmp){printf("%d ", tmp % 7);tmp /= 7;}printf("\n");break;
	case 8 : while(tmp){printf("%d ", tmp % 8);tmp /= 8;}printf("\n");break;
	case 9 : while(tmp){printf("%d ", tmp % 9);tmp /= 9;}printf("\n");break;
	case 10 : while(tmp){printf("%X ", tmp % 10);tmp /= 10;}printf("\n");break;
	case 11 : while(tmp){printf("%X ", tmp % 11);tmp /= 11;}printf("\n");break;
	case 12 : while(tmp){printf("%X ", tmp % 12);tmp /= 12;}printf("\n");break;
	case 13 : while(tmp){printf("%X ", tmp % 13);tmp /= 13;}printf("\n");break;
	case 14 : while(tmp){printf("%X ", tmp % 14);tmp /= 14;}printf("\n");break;
	case 15 : while(tmp){printf("%X ", tmp % 15);tmp /= 15;}printf("\n");break;
	case 16 : while(tmp){printf("%X ", tmp % 16);tmp /= 16;}printf("\n");break;
	}

}

int main(void)

{
	int n1 = 0;//用来接受用户想要转的进制数
	int n2 = 0;//用来接受用户输入的十进制数
	printf("请输入你想要转换的进制(2~16)\n");//
	scanf("%d",&n1);
	
	printf("请输入你想要转换的十进制数\n");//
	scanf("%d",&n2);

	iton(n1,n2);//调用函数


	return 0;
}



