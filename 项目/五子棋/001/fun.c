#include<stdio.h>
#include<stdlib.h>
#include "fun.h"
int all_pieces[15][15] = {0};//定义全局变量，用来记录棋盘上已有的棋子
int x = 0, y = 0;//用来接收和保存玩家输入的坐标
void dayin(int (*p)[15], int hang, int lie){
	int i = 0, j = 0;
	//行，第一行要打印提示，所以坐标从i-1开始
	for(i = 0;i<=hang;i++){
		printf("  %X  ",i);//提示
		for(j=0;j<lie;j++){
			if(i==0)
				printf(" %X ",j+1);//提示
			else if(all_pieces[i-1][j] == 1)
				printf(" ● ");//plarer_01的棋子
			else if(all_pieces[i-1][j] == 2)
				printf(" ○ ");//plarer_02的棋子
			else
				printf(" + ");//棋盘
		}
		printf("\n");
	}
}
void init(int (*p)[15], int hang, int lie){
	//int x = 0, y = 0;//接收玩家的值
	//进入循环
	while(1){
		scanf("%d,%d",&x,&y);//读取用户输入的值
		//判断是否越界[1-15]
		if(x>=1&&x<=hang&&y>=1&&y<=lie){
				if(all_pieces[x-1][y-1] == 0)
					break;//检查空，输入有效，跳出。如为1和2,则提示占用
				else
					printf("位置已有棋子，清重新输入:");//位置被占用
		}
		else
			printf("坐标越界，重新输入：");
		
	}
}
//赢棋判断函数
int check_win(){
	int i = 0, j = 0;//循环变量
	for(i=0;i < 15; i++){
		for(j=0; j < 15; j++){//遍历棋盘上的每一个位置
			int now = all_pieces[i][j];//获取当前位置的坐标，以及当前位置下棋的玩家
			if(now==0) continue;//空位置跳过
			//横向检查(右) 10,11,12,13,14 j<=10
			if(j<=10){
				//判断五字连珠
			if(now == all_pieces[i][j+1]&&
						now == all_pieces[i][j+2]&&
						now == all_pieces[i][j+3]&&
						now == all_pieces[i][j+4]
			  )
				return now;
			}
			//纵向检查(下) 10,11,12,13,14 j<=10
			if(i<=10){
				//判断五字连珠
				if(now == all_pieces[i+1][j]&&
						now == all_pieces[i+2][j]&&
						now == all_pieces[i+3][j]&&
						now == all_pieces[i+4][j]
				  )
					return now;
			}
			// 右下相连   10,10  11,11  12,12  13,13  14,14
			if(j<=10 && i<=10){
				//判断五字连珠
				if(now == all_pieces[i+1][j+1]&&
						now == all_pieces[i+2][j+2]&&
						now == all_pieces[i+3][j+3]&&
						now == all_pieces[i+4][j+4]
				  )
					return now;
			}
			//左下相连    10,10  11,9  12,8  13,7  14,6  i<=10;j>=4
			if(i<=10 && j>=4){
				//判断五字连珠
				if(now == all_pieces[i+1][j-1]&&
						now == all_pieces[i+2][j-2]&&
						now == all_pieces[i+3][j-3]&&
						now == all_pieces[i+4][j-4]
				  )
					return now;
			}
		}
	}
	return 0;
}
//检查是否获胜函数
void check_winner(){
		int winner = check_win();//	调用函数，并接受返回值
		if(winner !=  0){//
			if(winner == 1)//如果1  玩家1获胜
			
				printf("\n Play_01获胜！游戏结束！\n");
			else
				printf("\n Play_02获胜！游戏结束！\n");
			
				exit(0);//否则，结束游戏		//exit() 会自动关闭所有打开的文件需要头文件#<stdlib.h>
		}
}
void player_01(int (*p)[15],int hang, int lie){
	init(p,hang,lie);
	all_pieces[x-1][y-1] = 1;//记录棋子
	system("clear");//清屏
	dayin(p,hang,lie);
	check_winner();//调用函数，判断输赢
}
void player_02(int p[15][15],int hang, int lie){
    init(p,hang,lie);
    all_pieces[x-1][y-1] = 2;//记录棋子
    system("clear");//清屏
    dayin(p,hang,lie);
    check_winner();//调用函数判断输赢
}
