//输入任意一行字符，降序排列之
#include<stdio.h>
void print(char *arr,int size){
	int i = 0;
	for(i=0;i<size;i++){
		if(arr[i] != '\0')
			printf("arr[%d] = %-3c",i,arr[i]);

	}
	printf("\n");
}

int main(void)
{
	//	gets()版本
	//用fgets()的话，不用scanf()输入，也不用循环输出，只不过要去掉尾零
	char arr[64];
	int size = sizeof(arr) / sizeof(*arr);
	int i = 0, j = 0, tmp = 0;
	printf("请输入字符\n");
	scanf("%s",arr);
	for(i=0;i < size-1; i++){
		for(j=0;j<size-i-1;j++){
			if(arr[j]<arr[j+1]){
				tmp = arr[j];
				arr[j]= arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
	print(arr,size);
	return 0;
}

/*

#include <stdio.h>
#include <string.h>

int main(void)
{
    char str[1000];  // 假设最多输入1000个字符
    int i, j, n;
    char temp;
    
    // 输入字符串
    printf("请输入一行字符：\n");
    fgets(str, sizeof(str), stdin);  // 读取一行
    
    // 去掉末尾的换行符
    n = strlen(str);
    if(n > 0 && str[n-1] == '\n')
    {
        str[n-1] = '\0';
        n--;  // 更新长度
    }
    
    // 使用冒泡排序进行降序排列
    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - 1 - i; j++)
        {
            if(str[j] < str[j+1])  // 降序：当前字符 < 下一个字符时交换
            {
                // 交换两个字符
                temp = str[j];
                str[j] = str[j+1];
                str[j+1] = temp;
            }
        }
    }
    
    // 输出结果
    printf("降序排列后的字符：\n");
    printf("%s\n", str);
    
    return 0;
}


*/
