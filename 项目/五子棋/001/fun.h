#ifndef __FUN_H
#define __FUN_H


void dayin(int (*p)[15], int hang, int lie);
void init(int (*p)[15], int hang, int lie);
int check_win();
void check_winner();
void player_01(int (*p)[15],int hang, int lie);
void player_02(int p[15][15],int hang, int lie);
#endif
