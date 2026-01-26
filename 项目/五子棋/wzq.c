#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void inqipan(char p[16][16])
{int i,y;
	for(i=0;i<16;i++)
	{
		for(y=0;y<16;y++)
		{
			if(i==0)p[i][y]=y;
			else
				if(y==0)p[i][y]=i;
				else
					p[i][y]='.';
		}
	}
}
void outqipan(char q[16][16],int a,int b,int c)
{int i,y;
	static int xsum=0,ysum=0;
	
	xsum+=a;ysum+=b;
	for(i=0;i<16;i++)
	{
		for(y=0;y<16;y++)
		{
			if(i==0)printf("%4x",q[i][y]);
			else
				if(y==0)printf("%4x",q[i][y]);
				else
					printf("%4c",q[i][y]);
					
		}
		if(i==1)printf("        五子棋");
		if(i==3)printf("        双方交替下棋");
		if(i==4)printf("        先连城五子的");
		if(i==5)printf("        一方即为胜利");
		if(i==7)printf("        当前回合：");
		if(i==8)printf("        第1回合：");
		if(i==10)printf("        玩家一：共%d子",xsum);
		if(i==11)printf("        玩家二：共%d子",ysum);
		if(i==13&&c!=0)c==1?printf("         玩家1赢了"):printf("         玩家2赢了");
		if(c!=0){xsum=0;ysum=0;}
		printf("\n");
	}
}
int panduan(char pd[16][16])
{
	int x,y,count=1,a,xx,yy,xsum=0,ysum=0;  //xx,yy用于存储判断是否胜利的变量，a为他们的计数
	static int i=0;
	//i%2==0?printf("玩家1请输入："):printf("玩家2请输入：");
	if(i%2==0){
		xsum=xsum+1;
		printf("玩家1请输入：");
	}
	else {
		ysum=ysum+1;
		printf("玩家2请输入：");
	}
	scanf("%d-%d",&x,&y);
	if(x>0&&x<16&&y>0&&y<16)
		if(pd[x][y]=='.')
		{
			if(++i%2==0)pd[x][y]='#';
			else
				pd[x][y]='@';
			for(a=1;a<16;a++)      //左右判定
			{
				if(count==5){if(pd[x][y]=='@')return 1;
					else return 2;}
					if(pd[x][a]==pd[x][a+1]&&pd[x][a]==pd[x][y])count++;
					else count=1;
			}
			for(a=1;a<16;a++)       //上下判定
			{
				if(count==5){if(pd[x][y]=='@')return 1;
					else return 2;}
					if(pd[a][y]==pd[a+1][y]&&pd[a][y]==pd[x][y])count++;
					else count=1;
			}
			xx=x-(x<y?x:y);          //左上到右下
			yy=y-(x<y?x:y);
			for(;xx<16&&yy<16;xx++,yy++)
			{
				if(count==5){if(pd[x][y]=='@')return 1;
					else return 2;}
					if(pd[xx][yy]==pd[xx+1][yy+1]&&pd[xx][yy]==pd[x][y])count++;
					else count=1;
			}
			xx=x+((15-x-1)<y?(15-x-1):y);//左下到右上
			yy=y-((15-x-1)<y?(15-x-1):y);
			for(;xx>0&&yy<16;xx--,yy++)
			{
				if(count==5){if(pd[x][y]=='@')return 1;
					else return 2;}
					if(pd[xx][yy]==pd[xx-1][yy+1]&&pd[xx][yy]==pd[x][y])count++;
					else count=1;
			}
			outqipan(pd,xsum,ysum,0);
			return 3;
		}   
		else 
		{
			printf("此位置已有棋子！\n");
			i--;
			return 10;
		} 
}
int main(void)
{
	int i,x=1;
	char qipan[16][16];
	inqipan(qipan);
	while(x)
	{
		outqipan(qipan,0,0,0);
		x=panduan(qipan);
		if(x==1||x==2)
		{   system("clear");
			//	outqipan(qipan);
			//	pd[x][y]=='@'?printf("玩家1赢了"):printf("玩家2赢了");
			if(x==1)outqipan(qipan,0,0,1);
			else outqipan(qipan,0,0,2);

			break;
		}

		else
			if(x==10)continue;
			else
				system("clear");
	}
	return 0;
}
