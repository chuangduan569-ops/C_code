#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBER 5

struct stu
{
    int id;
    char name[20];
    int math;
};

int int_cmp(const void *key, const void *k)
{
    return *(int *)key - *(int *)k;
}

int double_cmp(const void *key, const void *k)
{
    if(*(double *)key < *(double *)k)
        return -1;
    else if(*(double *)key > *(double *)k)
        return 1;
    else
        return 0;
}

int char_cmp(const void *key, const void *k)
{
    return *(char *)key - *(char *)k;
}

void swap(void *a, void *b, size_t size)
{
    char *tmp = NULL;

    tmp = malloc(size);

    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);

    free(tmp);
}

void sort(void *arr, int nmemb, size_t size, int (*cmp)(const void *, const void *))
{
    int i = 0, j = 0;

    for(i = 0; i < nmemb - 1; i++)
    {
        for(j = 0; j < nmemb - 1 - i; j++)
        {
            if(cmp((char *)(arr)+j*size, (char *)(arr)+(j+1)*size) > 0)
                swap((char *)(arr)+j*size, (char *)(arr)+(j+1)*size, size);
        }
    }
}

int main(void)
{
    int irr[MEMBER] = {100, 201, 50, 70, 30};
    double drr[MEMBER] = {3.14, 9.87, 1.23, 2.34, 4.56};
    char crr[MEMBER] = {'y', 'z', 'g', 'k', 'a'};
    struct stu srr[MEMBER] = {
                                {10010, "zack", 98},
                                {10086, "tom", 97},
                                {10011, "jack", 85},
                                {12306, "jerry", 99},
                                {10007, "dennie", 80}
                            };
    int i = 0;//循环变量

    sort(irr, MEMBER, sizeof(*irr), int_cmp);//对于整型成员的数组进行排序
    for(i = 0; i < MEMBER; i++)
        printf("%d\t", irr[i]);
    printf("\n");

    sort(drr, MEMBER, sizeof(*drr), double_cmp);//对于双精度浮点类型成员的数组进行排序
    for(i = 0; i < MEMBER; i++)
        printf("%.2f\t", drr[i]);
    printf("\n");

    sort(crr, MEMBER, sizeof(*crr), char_cmp);//对于字符型成员的数组进行排序
    for(i = 0; i < MEMBER; i++)
        printf("%c\t", crr[i]);
    printf("\n");

    return 0;
}