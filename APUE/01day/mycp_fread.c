/*使用fgetc-fputc实现mycp的命令*/
/*使用fgets-fputs实现mycp的命令*/
/*使用fread-fwrite实现mycp的命令*/
#include <stdio.h>
    #include <string.h>
    
    #define FREAD
    #define BUFSIZE 32
   static int mycp(const char* srcfile, const char *destfile)
{
    FILE *fps = NULL;	//fps指针指向原文件
    FILE *fpd = NULL;	//fpd指针指向目标文件
    int ch = 0;	//ch变量存储从文件流中读取出的数据
    size_t count = 0;	//count存储成功读到的成员个数
    
    char buf[BUFSIZE] = {0};	//buf存储从文件流中读取出的数据
    fps = fopen(srcfile, "r");	//通过fopen(3)以r的方式打开原文件
    if(fps == NULL)	//判断打开原文件是否失败
    {
        perror("fopen()");	//打印错误信息
        return -1;	//由于打开原文件失败,结束函数,并且返回-1
    }
  	fpd = fopen(destfile, "w");	//通过fopen(3)以w的方式打开目标文件
    if(fpd == NULL)//判断打开目标文件是否失败
    {
        perror("fopen");	//打印错误信息
        fclose(fps);	//关闭原文件的文件流
        return -2;	//由于打开目标文件失败,结束函数,并且返回-2
    }
    
    while(1)	//循环读取数据
    {
#if defined(FDETC)
    	ch = fgetc(fps);	//从原文件的文件流中读取数据
        if(ch == EOF)	//判断是否读取到了文件末尾或者读取错误
        {
            if(ferror(fps))	//判断是否读取错误
            {
                fclose(fpd);	//关闭目标文件的文件流
                fclose(fps);	//关闭原文件的文件流
                return -3;	//由于读取错误,结束函数,并且返回-3
            }
            break;	//由于读取到了文件末尾,跳出死循环
            //如果返回非0说明出错了,没有到达文件结尾,  如果返回0,说明没有出错,到达文件结尾了
        }
        fputc(ch,fpd);	//把ch存储的数据写入到目标文件的文件流中
#elif defined(FGETS)
    memset(buf, 0, BUFSIZE);	//清空数据
        if(fgets(buf, BUFSIZE, fps) == NULL)
            break;
        fputs(buf,fpd);
#elif defined(FREAD)
    memset(buf, 0, BUFSIZE);
        if((count = fread(buf, 1, BUFSIZE, fps)) == 0)
        {
            if(ferror(fps))
            {
                fclose(fpd);
                fclose(fps);
                return -3;
            }
    		break;
        }
        fwrite(buf, 1, count, fpd);
#endif
    }
    
    fclose(fps);
    fclose(fpd);
    fps = NULL;
    fpd = NULL;
    return 0;
}

    int main(int argc, char *argv[])
{
    if(argc < 3)
    {
       fprintf(stderr, "Usage : %s + srcfile + destfile\n", argv[0]);//打印使用说明 
       return -1;//由于命令行参数的个数少于3个,结束程序,并且返回-1
    }
    mycp(argv[1],argv[2]);	//调用实现的内部函数,完成cp的命令功能
    
    return 0;
}
