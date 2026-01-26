#include<stdio.h>
#include "fun.h"

int main(void)
{
	int qipan[15][15] = {0};
	int hang = sizeof(qipan) / sizeof(qipan[0]);
	int lie = sizeof (qipan[0]) / sizeof(qipan[0][0]);
	dayin(qipan,hang,lie);//调用打印函数
	//游戏开始
	while(1){
		//play_01开始
		printf("\33[5m\33[35mPlayer_01(x,y):\33[0m");
		player_01(qipan,hang,lie);//调用play_01函数
		//play_02开始
		printf("\33[5m\33[33mPlayer_02(x,y):\33[0m");
		player_02(qipan,hang,lie);//调用player_02函数
	}

	return 0;
}
