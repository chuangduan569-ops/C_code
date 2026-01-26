#include<stdio.h>
#include<stdlib.h>

void check_winner(int player_num);//声明函数
int all_pieces[15][15] = {0};// 全局数组：记录棋盘上所有棋子的位置

int dayin(int p[15][15], int hang, int lie)
{
	//system("clear");
	int i = 0, j = 0;
	for(i = 0; i < hang; i++)
	{   
		printf("%x ",i);
		for(j = 0; j < lie; j++)
		{
			if(i==0)
				printf("%X ",j+1);
			else
				printf("+ ");
		}
		printf("\n");
	}
}

void player_01(int p[15][15],int hang,int lie)
{   
	//system("clear");
	int i = 0, j = 0;
	int x = 0, y = 0;//接收玩家的值
	//scanf("%d,%d",&x,&y);

	while(1) {                          // 开始无限循环
		scanf("%d,%d", &x, &y);         // 读取用户输入的坐标

		if(x >= 1 && x <= hang && y >= 1 && y <= lie) {  // 检查坐标是否在1-15范围内
			if(all_pieces[x-1][y-1] == 0) {              // 检查该位置是否为空,1玩家1已有，2玩家2已有
				break;                                   // 输入有效，退出循环
			} else {
				printf("位置已有棋子，请重新输入: ");     // 提示位置已被占用
			}
		} else {
			printf("坐标越界(1-15)，请重新输入: ");      // 提示坐标超出范围
		}
	}                                    // 循环结束

	// 记录棋子
	all_pieces[x-1][y-1] = 1;

	// 直接在这里打印所有棋子
	for(i = 0; i <= hang; i++)
	{
		printf("%x ",i);
		for(j = 0; j < lie; j++)
		{
			if(i==0)
				printf("%X ",j+1);
			else if(all_pieces[i-1][j] == 1)
				printf("* ");
			else if(all_pieces[i-1][j] == 2)
				printf("# ");
			else
				printf("+ ");
		}
		printf("\n");
	}
	//调用函数判断是否赢得胜利
	check_winner(1);
}

void player_02(int p[15][15],int hang,int lie)
{   
	//system("clear");
	int i = 0, j = 0;
	int x = 0, y = 0;//接收玩家的值
	// scanf("%d,%d",&x,&y);
	while(1) {                          // 开始无限循环
		scanf("%d,%d", &x, &y);         // 读取用户输入的坐标

		if(x >= 1 && x <= hang && y >= 1 && y <= lie) {  // 检查坐标是否在1-15范围内
			if(all_pieces[x-1][y-1] == 0) {              // 检查该位置是否为空
				break;                                   // 输入有效，退出循环
			} else {
				printf("位置已有棋子，请重新输入: ");     // 提示位置已被占用
			}
		} else {
			printf("坐标越界(1-15)，请重新输入: ");      // 提示坐标超出范围
		}
	}                                    // 循环结束

	// 记录棋子
	all_pieces[x-1][y-1] = 2;

	// 直接在这里打印所有棋子（和player_01一样的逻辑）
	for(i = 0; i <= hang; i++)
	{
		printf("%x ",i);// 打印行号
		for(j = 0; j < lie; j++) // 内层循环：遍历每一列
		{
			if(i==0)			//情况1：第一行（显示列号）
				printf("%X ",j+1);
			else if(all_pieces[i-1][j] == 1)	//第一行（i=0）;第一行（i=0）;0 = 空位置;1 = 玩家1的棋子;2 = 玩家2的棋子
				printf("* "); // 情况2：玩家1的棋子
			else if(all_pieces[i-1][j] == 2)
				printf("# ");// 情况3：玩家2的棋子
			else
				printf("+ ");// 情况4：空位置
		}
		printf("\n");
	}
	//调用函数判断是否赢得胜利
	check_winner(2);
}
// 赢棋判断函数
int check_win()
{
	int i, j;

	// 检查所有位置
	for(i = 0; i < 15; i++) {// 外层循环：遍历棋盘的每一行（0-14行）
		for(j = 0; j < 15; j++) {// 内层循环：遍历棋盘的每一列（0-14列）
			int current = all_pieces[i][j];// 获取当前位置的棋子值		// current = 0(空), 1(玩家1), 2(玩家2)
			if(current == 0) continue;  // 空位置跳过

			// 横向检查（向右）
			//  横向检查（向右检查5个连续棋子）
			// j <= 10（因为要向右检查4个位置，所以j最大为10）
			// 如果j=10	检查10,11,12,13,14
			if(j <= 10) {
			// 检查当前位置和右边4个位置是否都是相同的棋子
				if(current == all_pieces[i][j+1] &&//右1
						current == all_pieces[i][j+2] &&//右2
						current == all_pieces[i][j+3] &&//右3
						current == all_pieces[i][j+4]) {//右4
					// 发现连续5个相同棋子，返回当前棋子的玩家编号
					return current;
				}
			}

			// 纵向检查（向下）
			//i <= 10（因为要向下检查4个位置）
			//i=10，	10,11,12,13,14
			if(i <= 10) {
			//检查当前位置和下面4个位置是否都是相同的棋子
				if(current == all_pieces[i+1][j] &&
						current == all_pieces[i+2][j] &&
						current == all_pieces[i+3][j] &&
						current == all_pieces[i+4][j]) {
						// 发现连续5个相同棋子，返回当前棋子的玩家编号
					return current;
				}
			}

			// 右下斜向检查
			//i <= 10 且 j <= 10	行和列都要满足条件以及边界条件
			if(i <= 10 && j <= 10) {
				if(current == all_pieces[i+1][j+1] &&
						current == all_pieces[i+2][j+2] &&
						current == all_pieces[i+3][j+3] &&
						current == all_pieces[i+4][j+4]) {
						// 发现连续5个相同棋子，返回当前棋子的玩家编号
					return current;
				}
			}

			// 左下斜向检查
			//i <= 10 且 j >= 4；j >= 4，因为要向左检查，所以最小为4；从当前位置向左下方向检查
			if(i <= 10 && j >= 4) {
				// 检查当前位置和左下对角线4个位置
				if(current == all_pieces[i+1][j-1] &&
						current == all_pieces[i+2][j-2] &&
						current == all_pieces[i+3][j-3] &&
						current == all_pieces[i+4][j-4]) {
						// 发现连续5个相同棋子，返回当前棋子的玩家编号
					return current;
				}
			}
		}
	}

	return 0;  // 没有人赢
}
// 检查是否获胜的函数（供玩家1和玩家2调用）
void check_winner(int player_num)//接受玩家的实参  1玩家1,2玩家2
{
	int winner = check_win();//调用赢得条件的函数，判断谁赢
	if(winner == player_num) {
		if(player_num == 1) {//如果是1玩家1赢得胜利，返回值winner（1）
			printf("\nPlay_01获胜！游戏结束！\n");
		} else {
			printf("\nplay_02获胜！游戏结束！\n");//玩家2赢得胜利，返回值winner（2）
		}
		exit(0);  // 否则；结束游戏
	}
}


int main(void)
{
	int qipan[15][15] = {0};
	int hang = sizeof(qipan) / sizeof(qipan[0]);
	int lie = sizeof(qipan[0]) / sizeof(qipan[0][0]);

	dayin(qipan,hang,lie);

	// 游戏主循环
	while(1) {
		// 玩家1下棋
		printf("Play_01(x,y): ");
		player_01(qipan, hang, lie);

		// 玩家2下棋
		printf("Play_02(x,y): ");
		player_02(qipan, hang, lie);
	}
	return 0;
}
// printf("Play_01(x,y): ");
// player_01(qipan,hang,lie);
//printf("Play_02(x,y): ");
//  player_02(qipan,hang,lie);
