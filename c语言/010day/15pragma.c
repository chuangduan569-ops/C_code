#include <stdio.h>

#pragma pack(2)
struct aaa
{
    char c;
    int i;
};
//#pragma pack(push,n)
//#pragma pack(op)也可以不成对出现

int main(void)
{   
    printf("sizeof(struct aaa) = %ld\n", sizeof(struct aaa));
    
    return 0;
}
