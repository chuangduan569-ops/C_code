#include <stdio.h>

int main()
{
    unsigned int a = 0, c = 0;
    do {
        --c;
        a = a - 1;
    } while(a > 0);
    
    printf("%d\n", c);  
    

    
    return 0;
}


