#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

STACK *stack_create(void)
{
    return llist_create();//返回开辟的栈式存储结构
}

static int is_full(STACK *s)
{
    return 0;
}

static int is_empty(STACK *s)
{
    return llist_empty(s);
}

int stack_push(STACK *s, int data)
{
    if(is_full(s))//判断栈是否满了
        return -1;//由于栈满了,结束函数,并且返回-1
    llist_insert(s,&data,HEADINSERT);//头插法入栈
    return 0;
}

int stack_pop(STACK *s, int *save)
{
    if(is_empty(s))//判断栈是否空了
        return -1;//由于栈空了,结束函数,并且返回-1
    llist_pop(s,save);
    return 0;
}

void stack_display(STACK *s)
{
//    int i = s->stack_ind;//使用i变量代替栈顶元素访问栈式存储空间

    if(is_empty(s))//判断栈是否空了
        return ;//由于栈空了,结束函数
    llist_display(s);
    printf("------------------------------\n\n");
}

void stack_destroy(STACK *s)
{
    if(s == NULL)//判断是否没有栈
        return ;
  //  free(s);//释放栈
	llist_destroy(s);
}
