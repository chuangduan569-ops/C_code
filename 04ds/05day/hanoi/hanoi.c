#include <stdio.h>
#include <stdlib.h>

struct hanoi
{
	int hanoi_arr[9];//栈式存储空间
	int hanoi_ind;//栈顶元素
}hanoi[3] = {{{9,8,7,6,5,4,3,2,1}, 9}, {}, {}};//分别表示三个柱子

void draw(void)
{
	int i = 0, j = 0;	

	system("clear");
	for(i = 0;i<3;i++)//i循环，表示3个柱子
	{
		for(j = 0; j < hanoi[i].hanoi_ind; j++)
			printf("%d",hanoi[i].hanoi_arr[j]);
		for(; j < 13; j++)
			printf("-");
		printf("\n");
	}
//	getchar();
}

void swap(int src, int dest)
{
	hanoi[src].hanoi_ind--;//源柱子的栈顶标记下移
	hanoi[dest].hanoi_arr[hanoi[dest].hanoi_ind] = hanoi[src].hanoi_arr[hanoi[src].hanoi_ind];
	hanoi[dest].hanoi_ind++;
	draw();

}



void play(int src, int dest, int tmp, int num)
{
	if(num == 1)//如果只有一个铁饼
	{
		swap(src,dest);//把src的铁饼放在dest上
		return ;
	}
	play(src, tmp, dest, num - 1);
	swap(src, dest);
	play(tmp, dest, src, num - 1);
}


int main(void)

{
	draw();
	
	play(0, 1, 2, 9);//0 1 2 分别表示三个柱子的下标，3表示最多三个铁饼

	return 0;
}


