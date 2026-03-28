#include <stdio.h>
#include <unistd.h>
#include "anytimer.h"    
    
    void sig_handler(void *sig)
{
    write(1, "!", 1);
}
    
    int main(void)
{
	int id1, id2, id3, id4, id5, id6;	//定义三个闹钟ID    
    id1 = anytimer_init(1, sig_handler, NULL);//初始化1秒闹钟，返回ID
	id2 = anytimer_init(5, sig_handler, NULL);
	id3 = anytimer_init(10, sig_handler, NULL);//初始化10秒闹钟，返回ID
    id4 = anytimer_init(11, sig_handler, NULL);
	id5 = anytimer_init(15, sig_handler, NULL);
	id6 = anytimer_init(20, sig_handler, NULL);
        
        while(1)
        {
            write(1, "*", 1);
            sleep(1);
        }
    
    return 0;
}
