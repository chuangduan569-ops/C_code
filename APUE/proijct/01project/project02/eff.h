#ifndef __EFF_H
#define __EFF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <glob.h>
#include <termios.h>
#include <signal.h>
#include <crypt.h>


#define BUFSIZE     12      // 权限字符串缓冲区大小
#define TIMELEN     32      // 时间字符串长度 
#define TIMEOUT     30      // 登录超时时间(秒)
#define USER_DB     "users.dat"  // 用户数据文件

extern char *g_batch_mode;	//批量修改权限时存储权限值

extern int g_buffer_size;       // 缓冲区大小（字节），默认4096，范围1024-65536

extern struct termios old_t;  // 存储终端配置

char get_file_type(mode_t st_mode);

char *get_file_permission(mode_t st_mode, char *perm);

int file_stat_l(const char *filename);

int dir_scan(const char *dirpath);

//int get_file_nlink(nlink_t st_nlink);

//char *get_file_uname(uid_t st_uid);

//char *get_file_gname(gid_t st_gid);

//off_t get_file_size(off_t st_size);

//char *get_file_mtime(time_t tm, char *tbuf);
////////////////////////////////////////////////
void show_help(const char *prog_name);

int file_cp(const char *src, const char *dest);

int dir_copy_recursive(const char *src_dir, const char *dest_dir);

int file_view(const char *filename);
////////////////////////////////////////////////
int file_chmod(const char *path, const char *mode_str);
int file_search(const char *pattern);
////////////////////////////////////// ////////

int file_rm(const char *path);

int file_rename(const char *old_path, const char *new_path);

//==================== 磁盘统计函数 ==================== 
long du_calc(const char *path);
int file_du(const char *path);

//////////////////////////////////////////////////

// ==================== 批量操作函数 ==================== 
int batch_operate(const char *pattern, char opt);

// ==================== 输出重定向函数 ==================== 
int output_redirect(const char *log_path);

///////////////////////////////////////////////////
//==================== 登录认证函数 ==================== 
int login_authenticate(void);
char *get_passwd_input(const char *prompt);
int verify_user_passwd(const char *user, const char *passwd);
void login_timeout(int sig);
void reset_terminal(void);
void show_login_prompt(void);
void create_user(void);
void generate_md5_salt(char *salt_buf);





// myls 选项结构体
typedef struct {
    int show_all;      // -a选项：显示所有文件
    int long_format;   // -l选项：长格式显示
    int show_inode;    // -i选项：显示inode号
    int show_directory;// -d选项：显示目录本身
    int human_readable;// -h选项：人性化显示大小
} LsOptions;           // 选项结构体类型

// myls 文件信息结构体（用于排序）
typedef struct {
    char name[256];    // 文件名
    ino_t inode;       // inode号
    struct stat st;    // 文件属性
} LsFileInfo;          // 文件信息结构体类型

// myls 函数声明
int myls(int argc, char *argv[]);  // myls主函数




// 登录状态管理
int check_login_status(void);
void save_login_status(void);
void clear_login_status(void);



#endif




