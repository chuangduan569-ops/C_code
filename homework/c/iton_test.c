#include<stdio.h>
void iton(int n1, int n2)
{    
    int tmp = n2;//定义一个中间变量接收用户输入的值
    int digits[32];//定义一个数组用于接收余数，只不过是倒叙的
    int length = 0;// 记录已经存储了多少个余数
    /*
digits[0] = 0  // 第一次计算的余数（最低位）
digits[1] = 1  // 第二次计算的余数
digits[2] = 0  // 第三次计算的余数
digits[3] = 1  // 第四次计算的余数（最高位）
    */
    
    // 先计算并存储所有数字
    switch(n1)
    {
    case 2 : 
        while(tmp){ digits[length++] = tmp % 2; tmp /= 2; } break;//把结果存入digits[length]，length的值加1
    case 3 : 
        while(tmp){ digits[length++] = tmp % 3; tmp /= 3; } break;
    case 4 : 
        while(tmp){ digits[length++] = tmp % 4; tmp /= 4; } break;
    case 5 : 
        while(tmp){ digits[length++] = tmp % 5; tmp /= 5; } break;
    case 6 : 
        while(tmp){ digits[length++] = tmp % 6; tmp /= 6; } break;
    case 7 : 
        while(tmp){ digits[length++] = tmp % 7; tmp /= 7; } break;
    case 8 : 
        while(tmp){ digits[length++] = tmp % 8; tmp /= 8; } break;
    case 9 : 
        while(tmp){ digits[length++] = tmp % 9; tmp /= 9; } break;
    case 10 : 
        while(tmp){ digits[length++] = tmp % 10; tmp /= 10; } break;
    case 11 : 
        while(tmp){ digits[length++] = tmp % 11; tmp /= 11; } break;
    case 12 : 
        while(tmp){ digits[length++] = tmp % 12; tmp /= 12; } break;
    case 13 : 
        while(tmp){ digits[length++] = tmp % 13; tmp /= 13; } break;
    case 14 : 
        while(tmp){ digits[length++] = tmp % 14; tmp /= 14; } break;
    case 15 : 
        while(tmp){ digits[length++] = tmp % 15; tmp /= 15; } break;
    case 16 : 
        while(tmp){ digits[length++] = tmp % 16; tmp /= 16; } break;
    }
    
    // 统一正序输出
    if(length == 0){  // 处理0的情况
        printf("0");
    } else {
        for(int i = length-1; i >= 0; i--){//遍历所有的数字
            if(n1 <= 10){//判断小于10可以%d正常输出，2,3,4,5,6,7,8,9,每次的余数都比它本身小，所以直接打印就行
                printf("%d", digits[i]);
            } else {
                if(digits[i] < 10){//因为10以下的余数都走的n1<=10，这些都是十进制以上的余数。判断小于十直接输出
                    printf("%d", digits[i]);
                } else {
                    printf("%c", 'A' + (digits[i] - 10));//将十转到十五的关键。ascll码值
                }
            }
        }
    }
 //   printf("\n");
}.

int main(void)
{
    int n1 = 0;
    int n2 = 0;
    printf("请输入你想要转换的进制(2~16)\n");
    scanf("%d",&n1);
    
    printf("请输入你想要转换的十进制数\n");
    scanf("%d",&n2);

    printf("%d(10) = ", n2);//打印十进制数
    iton(n1,n2);//调用函数并打印出结果
    printf("(%d进制)\n", n1);//打印你要转换的进制数

    return 0;
}
