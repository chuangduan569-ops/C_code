#include<stdio.h>
int main(void)
{		//输入十个数，判断正负和零
		int i,num;
		int zheng=0,fu=0,zero=0;
		printf("请输入十个数\n");
		for(i=0;i<10;i++){//用循环输入十个数
			scanf("%d",&num);
		if(num>0)
		{
			zheng++;
		}
		else if(num==0){
			zero++;
		}
		else
			fu++;
		}
		printf("正数个数为:%d\n",zheng);

		printf("负数个数为:%d\n",fu);

		printf("零的个数为:%d\n",zero);


	return 0;
}
