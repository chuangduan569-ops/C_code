#include<stdio.h>
#include<stdlib.h>
int dayin(int p[15][15], int hang, int lie)
{
	//system("clear");
//	printf("  1 2 3 4 5 6 7 8 9 A B C D E F\n");
	int i = 0, j = 0;
	for(i = 0; i < hang; i++)
	{	
		printf("%x ",i);//每一行开头展示是哪一行
		for(j = 0; j < lie; j++)
		{
			if(i==0)
				printf("%X ",j+1);//第一列打印提示行
			else
				printf("+ ");
		}

		printf("\n");
	}
}
void player_01(int p[15][15],int hang,int lie)
{	
//	system("clear");
    	int i = 0, j = 0;
	int x = 0, y = 0;
	scanf("%d,%d",&x,&y);
	y = x-1;
//	printf("  1 2 3 4 5 6 7 8 9 A B C D E F\n");
    for(i = 0; i <= hang; i++)
    {
        printf("%x ",i);//每一行开头展示是哪一行
        for(j = 0; j < lie; j++)
        {
            if(i==0)
                printf("%X ",j+1);//第一列打印提示行
			else if(i==x&&j==y)
				{
//					i=x-1;
//					y=j-2;
					printf("* ");
				}
            else
                printf("+ ");
        }

        printf("\n");
    }
}
void player_02(int p[15][15],int hang,int lie)
{	
//	system("clear");

 	int i = 0, j = 0;
	int x = 0, y = 0;
    	scanf("%d,%d",&x,&y);
    	y = x-1;
//  printf("  1 2 3 4 5 6 7 8 9 A B C D E F\n");
    for(i = 0; i <= hang; i++)
    {
        printf("%x ",i);//每一行开头展示是哪一行
        for(j = 0; j < lie; j++)
        {
            if(i==0)
                printf("%X ",j+1);//第一列打印提示行
            else if(i==x&&j==y)
                {
//                  i=x-1;
//                  y=j-2;
                    printf("# ");
                }
            else
                printf("+ ");
        }

        printf("\n");
    }
}

int main(void)

{
	int qipan[15][15] = {0};
	int hang = sizeof(qipan) / sizeof(qipan[0]);//行
	int lie = sizeof(qipan[0]) / sizeof(qipan[0][0]);//列
	dayin(qipan,hang,lie);//调用函数，打印棋盘
//	system("clear");
	player_01(qipan,hang,lie);//调用玩家01的函数
//	system("clear");
	player_02(qipan,hang,lie);//调用玩家01的函数


	


}
