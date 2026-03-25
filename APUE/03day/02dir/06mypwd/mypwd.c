#include <stdio.h>
#include <unistd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>

#define NAMESIZE 32

int main(int argc, char *argv[])
{

	char name[NAMESIZE] = {0};	//用来存储录入的用户名
	char *pwd = NULL;	//pwd指针指向输入密码的首地址
	struct spwd *tp = NULL;	//tp指针指向/etc/shadow文件中读取的真正的密码
	char *cp = NULL;	//cp指针指向输入的密码加密以后的密码字符串

	//[1]输入用户登陆的用户名->fget(3)
	printf("请输入用户名 : ");	//打印提示信息
	fgets(name, NAMESIZE, stdin);	//从标准输入流中读取用户名
	//标准输入使用的是行缓冲,所以name会存'\n'
	*strchr(name, '\n') = '\0';	//strchr返回的是地址,对地址取 * 就算取\n 然后给它赋值替换
	
	//[2]输入密码->getpass(3)
	pwd = getpass("请输入密码 : ");	//输入密码
	if(pwd == NULL)	//判断获取密码是否失败
	{
		perror("getpass()");	//会设置errno
		return -1;
	}

	//[3]读真正的密码->getspnam(3)
	tp = getspnam(name);	//根据录入的用户名从/rtc/shadow文件获取用户的信息
	if(tp == NULL)	//判断获取用户信息是否失败
	{
		fprintf(stderr,"获取shadow文件中的用户信息失败\n");
		return -2;
	}

	//[4]将输入的密码进行加密->crypt
	cp = crypt(pwd, tp->sp_pwdp);	//把输入的密码根据加密算法和盐值进行加密
	if(cp == NULL)	//判断加密是否失败
	{
        perror("crypt()");//打印错误信息
        return -3;//由于加密失败,结束程序,并且返回-3
    }

	//[5]对比密码->strcmp
	if(!strcmp(tp->sp_pwdp, cp))	//判断密码是否相同
		printf("恭喜！登陆成功\n");
	else
		printf("sorry!密码错误！\n");


	return 0;
}



