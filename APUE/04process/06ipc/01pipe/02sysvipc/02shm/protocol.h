#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define PATH "/etc/passwd"//规定好使用同一个文件路径
#define PROJ_ID 'a'//规定好使用同一个proj_id

#define STRSIZE 1024//规定好不能超过1024byte

 //还需要规定消息队列的对话结构
struct shm_st
{
	long mtype;//官方规定需要有一个msgtyp
	char str[STRSIZE];//buf空间
};


#endif




