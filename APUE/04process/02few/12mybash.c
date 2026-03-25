#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define LNAMESIZE 128
#define HNAMESIZE 128
#define PWDSIZE 256
#define BUFSIZE 16

static int get_env_pwd(char *lname, char *hname, char *pwd)
{
	char *tmpname = NULL;//临时存储当前登陆用户的用户名
	//获取当前登陆用户的用户名
	if((tmpname = getenv("LOGNAME")) == NULL)//判断获取当前登陆用户的用户名是否成功
		return -1;
	else
		strcpy(lname, tmpname);//把临时存储的用户名拷贝到lname指向的存储空间
	
		//获取当前登陆终端的主机名
	if(gethostname(hname, HNAMESIZE) < 0)//判断获取当前登陆终端的主机名是否失败
	{
		perror("gethostname()");
		return -2;
	}
	
	//获取当前工作目录的路径
	if(getcwd(pwd, PWDSIZE) == NULL) //判断获取当前工作目录的路径是否失败
	{
		perror("getcwd()");
		return -3;
	}
	return 0;//3个信息都获取成功，返回0
}

static int get_cmd_line(char **line, size_t *length)
{
	if(getline(line, length, stdin) == -1)//判断读取一行内容是否失败
	{
		perror("getline()");
		return -1;
	}
	return 0;//成功读取一行内容返回0
}

static int parse_string(char *str, char *delim, char **buf, int size)
{
	int i = 0;

	while(i < size)//防止越界
	{
		buf[i] = strtok(str, delim);//分割字符串
		if(buf[i] == NULL)	//判断分割字符串是否完毕
			break;
		i++;//循环变量自增
		str = NULL;//第一次指向原串，之后再调用要指向NULL
	}
	return 0;
}

// 执行 cd 命令
static int change_directory(char **args)
{
	char *path = NULL;
	//[1] 只有 cd 没有参数或者 "cd ~" -> 切换到家目录
	if(args[1] == NULL || (strcmp(args[1], "~") == 0))
	{
		char *home = getenv("HOME");
		if(home == NULL)
		{
			fprintf(stderr,"cd :家被偷了\n");
			return -1;
		}
	path = home;
	}
	//[2] "cd -" -> 切换到上一个工作目录
	else if(strcmp(args[1], "-") == 0)
	{
		char *oldpwd = getenv("OLDPWD");
		if(oldpwd == NULL)
		{
			fprintf(stderr,"cd : no oldpwd\n");
			return -2;
		}
	path = oldpwd;
	printf("%s\n", oldpwd);//打印显示 oldpwd 目录
	}
	// [3] 其他情况 (包括 "cd .", "cd ..", "cd /", "cd 目录名" 等)	
	else
		path = args[1];
	//获取当前目录
	char old_cwd[PWDSIZE];
	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
	{
		perror("getcwd()");
		return -3;
	}

	// 切换目录
	if (chdir(path) != 0)
	{
		perror("cd");
		return -4;
	}

	//设置 OLDPWD 环境变量 (用于 cd -)
	if(setenv("OLDPWD", old_cwd, 1) != 0)
	{
		perror("setenv()");
		return -5;
	}

	// 更新 pwd 环境变量
	char new_cwd[PWDSIZE];
	if(getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		setenv("PWD", new_cwd, 1);
	
	return 0;

}

int main(void)
{
	char lname[LNAMESIZE] = {0};//用来存储当前登陆用户名
	char hname[HNAMESIZE] = {0};//当前登终端的主机名
	char pwd[PWDSIZE] = {0};//工作目录的路径
	char *line = NULL;//存储读取一行指令的首地址
	size_t length = 0;//读取到的字节数
	char *buf[BUFSIZE] = {0};//存储分割出的子串的首地址	
	pid_t pid;//存储子进程的标识

	while(1)//死循环
	{
		//打印终端提示符
		if(get_env_pwd(lname, hname, pwd) < 0)//判断获取终端信息是否失败
			exit(1);
		else
			printf("%s@%s:%s$ ", lname, hname, pwd);//打印终端提示符(可扩展)

	//等待读一行输入
	if(get_cmd_line(&line, &length) < 0)//判断读取一行指令是否失败
		exit(2);
	
	line[strcspn(line, "\n")] = '\0';//去除末尾换行符

	//如果是空命令(只按回车)，继续循环
	if(strlen(line) == 0)
	{
		free(line);
		line = NULL;
		length = 0;
		continue;
	}

	//如果是exit 就直接退出
	if(strcmp(line, "exit") == 0)
		break;

	//分割字符串
	//把 "ls -l", 分割出 "ls" "-l"
	parse_string(line, " \n", buf, BUFSIZE);//分割命令 选项 参数
	
	//检查是否成功分割出命令
	if(buf[0] == NULL)
	{
		free(line);
		line = NULL;
		length = 0;
		continue;
	}

	//处理内部cd 命令
	if(strcmp(buf[0], "cd") == 0)
	{
		change_directory(buf);
		// cd 命令执行后继续循环, 不需要创建子进程
		free(line);
		line = NULL;
		length = 0;
		continue;

	}


	//创建子进程
	pid = fork();//创建子进程
	if(pid < 0)//判断创建子进程是否失败
	{
		perror("fork()");
		exit(3);
	}
	
	//替换子进程
	if(pid == 0) //子进程的操作
	{
		execvp(buf[0],buf);//替换子进程
		perror("execvp()");
		exit(4);//由于替换子进程失败，终止进程，并且返回状态4
	}
	else//父进程的操作(需要收尸)
		wait(NULL);
	}

	free(line);//释放空间  由于getline(3)会动态开辟空间,所以释放空间
	
	return 0;
}




