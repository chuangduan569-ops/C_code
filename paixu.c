for(i=0;i<size-1;i++)
{for(j=0;j<size-i--;j++)
{if(arr[j]>arr[j+1])
{	t =  arr[j];
	arr[j] = arr[j+1];
	arr[j+1] = t;


}
}
}

for(i=0;i<size-1;i++){
	k = i;
	for(j=i+1;j<size;j++){
	if(p[k] > p[j])
	k = j;	
	}
	if(i!=k){
	t = p[i];
	p[i] = p[k];
	p[k] = t;
	}

}
插入排序，分为已排序和未排序两个部分，每一次从未排序的
void sort(int *arr,int size){
int i = 0, j =0;
int flag = 0;//用于存储临时等待插入的顺序
if(size<=1)//判断数列中的数值是否少于等于1
	return;//不需要排序直接结束
for(i=1;i<size;i++)//外层循环；从第二个元素开始；（下标为1） 循环到 size - 1 为止
{
	flag = arr[i];//来保存当前等待插入元素的数据
	j = i - 1;//j标记当前元素的前一个位置,其实也就是已排序的最后一个位置
	while(j>=0&&arr[j]>flag)//内层循环：将已排序的部分大于flag的元素向后移动
	{//注意： j>=0 j是不能越界的
		arr[j+1] = arr[j];
		j--;
	}
	//循环结束后 j+1就是flag应该插入的正确位置
	arr[j+1] = flag;
}
}
