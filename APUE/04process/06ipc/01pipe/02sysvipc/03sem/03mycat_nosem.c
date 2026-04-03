/*使用系统调用IO函数实现mycat*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUFSIZE 8

int main(int argc, char *argv[])
{
    int fd = 0;//fd变量用来保存打开文件的文件描述符
    int i = 0;//循环变量
    pid_t pid;//存储子进程的标识
    char buf[BUFSIZE] = {0};//存储从文件中读取出的数据
    ssize_t count = 0;//存储成功读取到的字节数

    if(argc < 2)//判断命令行参数个数是否少于2个
    {
        fprintf(stderr, "Usage : %s + filename\n", argv[0]);//打印使用说明
        return -1;//由于命令行参数个数少于2个,结束程序,并且返回-1
    }

    fd = open(argv[1], O_RDONLY);//通过open(2)以只读的形式打开文件
    if(fd < 0)//判断打开文件是否失败
    {
        perror("open()");//打印错误信息
        return -2;//由于打开文件失败,结束程序,并且返回-2
    }

    for(i = 0; i < 4; i++)
    {
        pid = fork();//创建子进程
        if(pid == -1)//判断创建子进程是否失败
        {
            perror("fork()");//打印错误信息
            close(fd);//通过close(2)关闭文件
            exit(1);//由于创建子进程失败,终止进程
        }
        if(pid == 0)//子进程的操作
            break;
    }

    while(1)
    {
        memset(buf, 0, BUFSIZE);//清空脏数据
        count = read(fd, buf, BUFSIZE);//从文件中读取数据
        if(count == -1)//判断读取文件数据是否失败
        {
            perror("read()");//打印错误信息
            exit(1);//终止进程
        }
        if(count == 0)//判断是否读取完毕
            break;//跳出死循环
        sleep(1);//睡1s
        write(1, buf, count);//把buf中的数据写到标准输出文件中
    }

    switch(i)//终止子进程
    {
        case 0 :
        case 1 :
        case 2 :
        case 3 : exit(0);
    }

    for(i = 0; i < 4; i++)
        wait(NULL);//等待子进程结束

    close(fd);//通过close(2)关闭文件

    return 0;
}

