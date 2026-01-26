#include <stdio.h>
#include "stack.h"

int main(void)
{
    STACK *s = NULL;//s指针指向栈式的存储结构
    int arr[] = {11,22,33,44,55,66,77,88,99};//数据源
    int i = 0;//循环变量
    int ret = 0;//ret变量用来接收返回值
    int save = 0;//save变量存储出栈的数据

    s = stack_create();//创建栈式的存储结构
    if(s == NULL)//判断是否创建失败
    {
        fprintf(stderr, "stack_create() Is Failed!\n");//打印错误信息
        return -1;//由于创建栈式的存储结构失败,结束程序,并且返回-1
    }

    for(i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        ret = stack_push(s, arr[i]);//入栈
        if(ret != 0)//判断入栈是否失败
        {
            printf("Stack Is Full!\n");//打印栈满的信息
            break;//由于栈满了,跳出循环
        }
    }

    stack_display(s);//遍历栈

    ret = stack_pop(s, &save);//出栈
    if(ret != 0)//判断出栈是否失败
        printf("Stack Is Empty!\n");//打印栈空的信息
    else
        printf("save = %d\n\n", save);

    stack_display(s);//遍历栈

    stack_push(s, 9527);//入栈

    stack_display(s);//遍历栈

    stack_destroy(s);//销毁栈

    return 0;
}
