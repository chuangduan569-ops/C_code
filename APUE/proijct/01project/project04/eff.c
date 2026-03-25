#include "eff.h"
#include <getopt.h>


struct termios old_t;  // 存储终端配置（登录认证函数使用）

char *g_batch_mode = NULL;  // 存储批量修改权限时的权限值，默认为NULL

char get_file_type(mode_t st_mode)//获取文件的类型
{   
    char c = 0;//用来存储解析出来的文件类型的符号
    
    switch(st_mode & S_IFMT)//根据文件类型获取字符
    {
        case S_IFREG : c = '-'; break;//普通文件
        case S_IFDIR : c = 'd'; break;//目录
        case S_IFCHR : c = 'c'; break;//字符设备
        case S_IFBLK : c = 'b'; break;//块设备
        case S_IFSOCK: c = 's'; break;//套接字
        case S_IFIFO : c = 'p'; break;//管道
        case S_IFLNK : c = 'l'; break;//符号链接
        default      : c = '?'; break;//未知类型
    }
    
    return c;
}

char *get_file_permission(mode_t st_mode, char *perm)//获取文件权限
{   
    int mask[9] = {S_IRUSR, S_IWUSR, S_IXUSR,
                            S_IRGRP, S_IWGRP, S_IXGRP, 
                            S_IROTH, S_IWOTH, S_IXOTH};//存储权限的宏值
    char permission[10] = "rwxrwxrwx";//满权限
    int i = 0;//循环变量
    
    for(i = 0; i < 9; i++)
    {
        if(!(st_mode & mask[i]))//判断当前文件是否没有该权限
            permission[i] = '-';//由于没有该权限,把该位置的符号修改为'-'
    }
    
    /* 处理特殊权限 */
    if(st_mode & S_ISUID)//检查是否设置了SUID位
        permission[2] = (st_mode & S_IXUSR) ? 's' : 'S';//如果文件是UID文件,则把第3个位置的符号修改为's'或'S'
    if(st_mode & S_ISGID)//检查是否设置了SGID位
        permission[5] = (st_mode & S_IXGRP) ? 's' : 'S';//如果文件是GID文件,则把第6个位置的符号修改为's'或'S'
    if(st_mode & S_ISVTX)//检查是否设置了粘滞位
        permission[8] = (st_mode & S_IXOTH) ? 't' : 'T';//如果文件是VTX文件,则把第9个位置的符号修改为't'或'T'
    
    strncpy(perm, permission, BUFSIZE);//把局部数组的数据转存到形参指向的存储空间
    return perm;//返回形参指向的存储空间
}

static int get_file_nlink(nlink_t st_nlink)
{   
    return st_nlink;
}

static char *get_file_uname(uid_t st_uid)
{   
    struct passwd *p = NULL;//指针p指向获取到的用户信息
    
    p = getpwuid(st_uid);//根据uid获取该用户的用户信息
    if(p == NULL)//判断获取用户信息是否失败
    {
        perror("getpwuid()");//打印错误信息
        return NULL;//由于获取用户信息失败,结束函数,并且返回NULL
    }
    return p->pw_name;//返回用户信息中的user name
}

static char *get_file_gname(gid_t st_gid)
{   
    struct group *p = NULL;//指针p指向获取到的组信息
    
    p = getgrgid(st_gid);//根据gid获取该组的组信息
    if(p == NULL)//判断获取组信息是否失败
    {
        perror("getgrgid()");//打印错误信息
        return NULL;//由于获取用户信息失败,结束函数,并且返回NULL
    }
    return p->gr_name;//返回用户信息中的group name
}

static off_t get_file_size(off_t st_size)
{
    return st_size;
}

static char *get_file_mtime(time_t tm, char *tbuf)
{
    struct tm *p = NULL;//p指针指向转换后的时间结构体

    p = localtime(&tm);//把时间戳转换成时间结构
    if(p == NULL)//判断转换是否失败
    {
        perror("localtime()");//打印错误信息
        return NULL;//由于转换失败,结束函数,并且返回NULL
    }
    strftime(tbuf, TIMELEN, "%m月 %d %H:%M", p);//把时间结构转换为格式化时间的字符串

    return tbuf;
}


   int file_stat_l(const char *filename)//获取文件的详细信息
   {
            struct stat fs;//用来存储文件的属性
            char perm[BUFSIZE]={0};//用来存储文件的权限字符串
            char tbuf[TIMELEN]={0};//用来存储文件的修改时间 
            struct passwd *up = NULL;//指针up指向获取到的用户信息
            struct group *gp = NULL;//指针gp指向获取到的组信息
            struct tm *tm = NULL;//指针tm指向获取到的本地时间信息   
     
    if(filename ==NULL)
    {
        fprintf(stderr, "Error: filename is NULL\n");
        return -1;
    }

     if(stat(filename, &fs) == -1)//判断获取文件的元信息是否失败    
    {
        perror("stat()");//打印错误信息
        return -2;//由于获取文件的元信息失败,结束程序,并且返回-2
    }

    up= getpwuid(fs.st_uid);//根据uid获取该用户的用户信息
    if(up == NULL)//判断获取用户信息是否失败
    {
        perror("getpwuid()");//打印错误信息
        return -3;//由于获取用户信息失败,结束函数,并且返回-3
    }

    gp= getgrgid(fs.st_gid);//根据gid获取该组的组信息
    if(gp == NULL)//判断获取组信息是否失败
    {
        perror("getgrgid()");//打印错误信息
        return -4;//由于获取组信息失败,结束函数,并且返回-4
    }

    tm = localtime(&fs.st_mtime);//把时间戳转换成时间结构体
    if(tm == NULL)
    {
        perror("localtime()");
        return -5;//由于转换失败，结束函数，返回-5
    }
    strftime(tbuf, TIMELEN,"%m月%d %H:%M",tm);  

    printf("%c", get_file_type(fs.st_mode));//获取文件的类型
    printf("%s", get_file_permission(fs.st_mode, perm));//获取文件的权限
    printf(" %ld ", (long)fs.st_nlink);//获取文件的硬链接数
    printf("%s", up->pw_name);//获取文件的所属者
    printf(" %s ", gp->gr_name);//获取文件的所属组
    printf("%ld ", (long)fs.st_size);//获取文件的大小
    printf("%s ", tbuf);//获取文件的修改时间
    printf("%s\n", filename); //打印文件名

    return 0;
}

int dir_scan(const char *dirpath)//扫描目录
{
    struct stat fs;//用来存储获取到的文件元信息
    DIR *dp = NULL;//dp指针指向打开的目录流
    struct dirent *entry = NULL;//entry指针指向目录项结构
    int ret = 0;//用来存储错误码
    
    if(dirpath == NULL)//判断目录路径是否为NULL
    {
        fprintf(stderr, "Error: dirpath is NULL\n");//打印错误信息
        ret = -1;//存储-1错误码
        goto ERR_1;//跳转到ERR_1
    }
    
    if(stat(dirpath, &fs) == -1)//判断获取文件元信息是否失败
    {
        perror("stat()");//打印错误信息
        ret = -2;//存储-2错误码
        goto ERR_1;//由于获取文件元信息失败,跳转到ERR_1
    }
    
    if(!S_ISDIR(fs.st_mode))//判断是否不是目录文件
    {
        printf("%s Not A Directory!\n", dirpath);//打印提示信息
        ret = -3;//存储-3错误码
        goto ERR_1;//由于不是目录文件,跳转到ERR_1
    }
    
    dp = opendir(dirpath);//打开目录文件
    if(dp == NULL)//判断打开目录文件是否失败
    {
        perror("opendir()");//打印错误信息
        ret = -4;//存储-4错误码
        goto ERR_1;//由于打开目录文件失败,跳转到ERR_1
    }
    
    while(1)//循环读取目录项结构
    {
        errno = 0;//为了防止errno存储之前的错误码,进行清空处理
        entry = readdir(dp);//读取目录流中的目录项结构
        if(entry == NULL)//判断是否读取结束或者读取失败
        {
            if(errno != 0)//判断是否读取失败
            {
                perror("readdir()");//打印错误信息
                ret = -5;//存储-5错误码
                goto ERR_2;//由于读取目录项结构失败,跳转到ERR_2
            }
            break;//跳出死循环
        }
        printf("%ld-%s\n", entry->d_ino, entry->d_name);//打印子目录的inode号以及文件名
    }
ERR_2:
    closedir(dp);//关闭目录流
ERR_1:
    return ret;
}

// 帮助指引函数：输出使用方法
void show_help(const char *prog_name)
{
    printf("====================Embedded File Factory(嵌入式文件工厂)项目 ====================\n");
    printf("使用方法：%s [选项] 目标文件/目录\n", prog_name);
    printf("  -l 文件名/目录      查看文件详细属性 (模拟ls -l)\n");
    printf("  -c 源文件 目标文件  复制普通文件 (模拟cp)\n");
    printf("  -v 文件名           查看文件内容 (模拟cat)\n");
    printf("  -d 目录名           查看目录下的所有子文件/子目录/子目录的子文件\n");
    printf("  -s 文件名/目录      统计磁盘占用 (模拟du -sh, 单位KB MB GB)\n");
    printf("  -h                  查看帮助指引\n");
    printf("  -R 源目录 目标目录  递归复制目录 (支持子目录/文件)\n");
    printf("  -r 路径             删除文件/目录 (模拟rm -rf)\n");
    printf("  -n 旧路径 新路径    重命名文件/目录 (模拟mv)\n");
    printf("  -f 搜索模式         文件搜索 (模拟find -name 如*.c, 支持通配符)\n");
    printf("  -m 权限 路径        修改文件权限 (如755, 八进制)\n");
    printf("  -b 模式 操作        批量操作 (模式: *.c; 操作: l-属性 v-查看 s-统计)\n");
    printf("  -o 日志路径         输出重定向到指定文件\n");
    printf("================================================================================\n");
}

// 文件基础操作：复制文件（支持普通文件）
int file_cp(const char *src, const char *dest)
{
    int fd1 = 0; 
    int fd2 = 0;
    char buf[4096] = {0};
    int count = 0;
    
    fd1 = open(src, O_RDONLY);
    if(fd1 < 0)
    {
        perror("open()");
        return -1;
    }

    fd2 = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd2 < 0)
    {   
        close(fd1);
        perror("open()");
        return -2;
    }

    while(1)
    {
        memset(buf, 0, 4096);
        count = read(fd1, buf, 4096);
        if(count == 0)
            break;
        else if(count < 0)
        {
            perror("read()");
            return -3;
        }
        write(fd2, buf, count);
    }
    
    close(fd1);
    close(fd2);

    return 0;   
}

// 复制目录文件
int dir_copy_recursive(const char *src_dir, const char *dest_dir)
{
    struct stat fs;
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    char src_path[4096] = {0};
    char dest_path[4096] = {0};
    int ret = 0;

    if(stat(src_dir, &fs) == -1)
    {
        perror("stat()");
        return -1;
    }

    if(mkdir(dest_dir, 0755) == -1)
    {
        if(errno != EEXIST)
        {
            perror("mkdir()");
            return -2;
        }
    }

    dp = opendir(src_dir);
    if(dp == NULL)
    {
        perror("opendir()");
        return -3;
    }

    while(1)
    {
        errno = 0;
        
        entry = readdir(dp);
        if(entry == NULL)
        {
            if(errno != 0)
            {
                perror("readdir()");
                closedir(dp);
                return -4;
            }
            break;
        }
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);
        
        struct stat st;
        if(lstat(src_path, &st) == -1)
            continue;

        if(S_ISDIR(st.st_mode))
        {
            if(dir_copy_recursive(src_path, dest_path) < 0)
                ret = -1;
        }
        else
        {
            if(file_cp(src_path, dest_path) < 0)
                ret = -1;
        }
    }
    closedir(dp);
    return ret;
}

// 文件基础操作：查看文件内容（模拟cat）
int file_view(const char *filename)
{
    int fd = 0;
    char buf[4096] = {0};
    int count = 0;

    fd = open(filename, O_RDONLY);
    if(fd < 0)
    {
        perror("open()");
        return -1;
    }

    while(1)
    {
        memset(buf, 0, 4096);
        count = read(fd, buf, 4096);
        if(count == 0)
            break;
        else if(count < 0)
        {
            perror("read()");
            return -2;
        }
        write(1, buf, count);
    }

    close(fd);

    return 0;
}


/* ==================== 磁盘统计函数 ==================== */

// 获取指定文件的所占磁盘空间的块数
long du_calc(const char *path)
{
    struct stat fs;//fs变量用来存储获取到的文件元信息
    long count = 0;//count变量用来存储目录整体的块数
    DIR *dp = NULL;//dp指针指向打开目录的目录流
    struct dirent *entry = NULL;//entry指针指向目录项结构
    char buf[1024] = {0};//buf数组中存储的是路径字符串

    if(stat(path, &fs) == -1)//判断获取文件元信息是否失败
    {
        perror("stat()");//打印错误信息
        return -1;//由于获取文件元信息失败,结束函数,并且返回-1
    }

    if(!S_ISDIR(fs.st_mode))//判断是否不是目录文件
    {
        return fs.st_blocks;//直接返回非目录文件的块数
    }

    count = fs.st_blocks;//获取目录本身的块数
    dp = opendir(path);//打开操作的目录
    if(dp == NULL)//判断打开操作的目录是否失败
    {
        perror("opendir()");//打印错误信息
        return -2;//由于打开操作的目录失败,结束函数,并且返回-2
    }

    while(1)//循环读取目录项结构
    {
        errno = 0;//把errno清空
        entry = readdir(dp);//读取目录流指针指向的目录项结构
        if(entry == NULL)//判断读取目录项结构是否失败或者读取结束
        {
            if(errno != 0)//判断读取目录项结构是否失败
            {
                closedir(dp);//关闭目录流
                perror("readdir()");//打印错误信息
                return -3;//由于读取目录项结构失败,结束函数,并且返回-3
            }
            break;//读取目录项完毕,跳出死循环
        }
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;//跳过目录中的 . 和 .. 目录
        memset(buf, 0, 1024);//清空buf空间
        strcpy(buf, path);//先把当前操作的目录拷贝到buf空间中
        strcat(buf, "/");//拼接目录下的符号
        strcat(buf, entry->d_name);//拼接子文件名
        count += du_calc(buf);//递归累加子文件的块数
    }
    closedir(dp);//关闭目录句柄避免泄漏
    return count;
}

// 调用du_calc统计块数，转换为KB/MB/GB并格式化输出
int file_du(const char *path)
{
    if (path == NULL || strlen(path) == 0)
    {
        fprintf(stderr, "错误：路径不能为空！\n");
        return -1;
    }

    // 调用核心统计函数，失败返回错误码
    long total_blocks = du_calc(path);
    if (total_blocks < 0)
    {
        fprintf(stderr, "错误：统计%s的磁盘占用失败（错误码：%ld）\n", path, total_blocks);
        return -2;
    }

    // 块数转字节（1块=512字节），再适配KB/MB/GB单位
    long long bytes = (long long)total_blocks * 512;
    double size = bytes / 1024.0;
    const char *units[] = {"KB", "MB", "GB"};
    int unit_idx = 0;

    // 自动切换更大单位（KB→MB→GB）
    while (size >= 1024 && unit_idx < 2)
    {
        size /= 1024;
        unit_idx++;
    }

    // 模拟du -sh格式输出结果
    printf("%.1f%s\t%s\n", size, units[unit_idx], path);
    return 0;
}

/* ==================== 删除和重命名函数 ==================== */

// 递归删除文件/目录：文件直接删，目录先删子项再删自身
int file_rm(const char *path)
{
    struct stat fs;
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    char buf[1024] = {0};

    // 校验路径是否存在
    if (stat(path, &fs) == -1)
    {
        fprintf(stderr, "file_rm: 路径%s不存在: %s\n", path, strerror(errno));
        return -1;
    }

    // 普通文件：直接删除
    if (!S_ISDIR(fs.st_mode))
    {
        if (remove(path) == -1)
        {
            fprintf(stderr, "file_rm: 删除文件%s失败: %s\n", path, strerror(errno));
            return -2;
        }
        printf("成功删除文件：%s\n", path);
        return 0;
    }

    // 目录：先打开目录，递归删除所有子项
    dp = opendir(path);
    if (dp == NULL)
    {
        fprintf(stderr, "file_rm: 打开目录%s失败: %s\n", path, strerror(errno));
        return -3;
    }

    // 遍历目录子项，跳过.和..避免死循环
    while (1)
    {
        errno = 0;
        entry = readdir(dp);
        if (entry == NULL)
        {
            if (errno != 0)
            {
                closedir(dp);
                fprintf(stderr, "file_rm: 读取目录%s失败: %s\n", path, strerror(errno));
                return -4;
            }
            break;
        }
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // 拼接子项完整路径，递归删除
        snprintf(buf, sizeof(buf), "%s/%s", path, entry->d_name);
        if (file_rm(buf) != 0)
            fprintf(stderr, "警告：子项%s删除失败\n", buf);
    }

    closedir(dp);

    // 删除空目录
    if (rmdir(path) == -1)
    {
        fprintf(stderr, "file_rm: 删除目录%s失败: %s\n", path, strerror(errno));
        return -5;
    }
    printf("成功删除目录：%s\n", path);
    return 0;
}

// 文件/目录重命名
int file_rename(const char *old_path, const char *new_path)
{
    // 入参校验：原/新路径不能为空
    if (old_path == NULL || strlen(old_path) == 0 || new_path == NULL || strlen(new_path) == 0)
    {
        fprintf(stderr, "file_rename: 原路径/新路径不能为空！\n");
        return -1;
    }

    // 校验原路径是否存在
    struct stat fs;
    if (stat(old_path, &fs) == -1)
    {
        fprintf(stderr, "file_rename: 原路径%s不存在: %s\n", old_path, strerror(errno));
        return -2;
    }

    // 执行重命名操作
    if (rename(old_path, new_path) == -1)
    {
        fprintf(stderr, "file_rename: 重命名%s->%s失败: %s\n", old_path, new_path, strerror(errno));
        return -3;
    }

    printf("成功重命名：%s -> %s\n", old_path, new_path);
    return 0;
}


// 修改文件权限
int file_chmod(const char *path, const char *mode_str) 
{
    char *endptr;
    long mode_long;
    mode_t mode;
    
    /* 将字符串权限转换为八进制数 */
    mode_long = strtol(mode_str, &endptr, 8);
    
    if (*endptr != '\0') {
        fprintf(stderr, "错误: 无效的权限格式 '%s'\n", mode_str);
        return -1;
    }
    
    if (mode_long < 0 || mode_long > 07777) {
        fprintf(stderr, "错误: 权限值必须在 0-7777 之间\n");
        return -1;
    }
    
    mode = (mode_t)mode_long;
    
    if (chmod(path, mode) == -1) {
        perror("修改权限失败");
        return -1;
    }
    
    printf("权限修改成功: %s -> %03o\n", path, (unsigned int)mode);
    return 0;
}

// 文件搜索
int file_search(const char *pattern) 
{
    glob_t globbuf;
    int ret;
    size_t i;
    
    ret = glob(pattern, GLOB_ERR | GLOB_NOSORT, NULL, &globbuf);
    
    switch (ret) {
        case 0:
            printf("找到 %lu 个匹配文件:\n", (unsigned long)globbuf.gl_pathc);
            for (i = 0; i < globbuf.gl_pathc; i++) {
                printf("  %s\n", globbuf.gl_pathv[i]);
            }
            break;
            
        case GLOB_NOMATCH:
            printf("没有找到匹配 '%s' 的文件\n", pattern);
            break;
            
        default:
            fprintf(stderr, "搜索出错\n");
            globfree(&globbuf);
            return -1;
    }
    
    globfree(&globbuf);
    return 0;
}

/* ==================== 批量操作函数 ==================== */

// 批量操作函数
int batch_operate(const char *pattern, char opt) 
{
    glob_t globbuf;     // 用于存储glob匹配结果
    int i;              // 循环计数器
    struct stat st;     // 文件状态结构体
    
    // 使用glob函数匹配文件模式
    if (glob(pattern, GLOB_TILDE, NULL, &globbuf) != 0) {
        perror("glob");
        return -1;
    }
    
    // 遍历匹配到的所有文件
    for (i = 0; i < globbuf.gl_pathc; i++) {
        // 获取文件状态
        if (stat(globbuf.gl_pathv[i], &st) != 0) {
            perror("stat");
            continue;
        }
        
        // 根据操作选项修改文件权限
        switch (opt) {
            case 'l':  // 显示文件属性
                file_stat_l(globbuf.gl_pathv[i]);
                break;
            case 'v':  // 查看文件内容
                file_view(globbuf.gl_pathv[i]);
                break;
            case 's':  // 统计磁盘占用
                file_du(globbuf.gl_pathv[i]);
                break;
            case 'd':  // 删除文件
                file_rm(globbuf.gl_pathv[i]);
                break;
            case 'p':  /* ========== 新增：批量修改权限 ========== */
                // 检查是否设置了权限值
                if (g_batch_mode != NULL) {
                    printf("正在修改权限: %s -> %s\n", 
                           globbuf.gl_pathv[i], g_batch_mode);
                    file_chmod(globbuf.gl_pathv[i], g_batch_mode);
                } else {
                    fprintf(stderr, "错误: 批量修改权限需要先设置权限值\n");
                }
                break;
                
            default:   // 无效选项
                fprintf(stderr, "未知操作: %c\n", opt);
                globfree(&globbuf);
                return -1;
        }
    }
    
    // 释放glob分配的内存
    globfree(&globbuf);
    // 操作完成后清空全局权限值，避免影响后续操作
    g_batch_mode = NULL;
    
    return 0;
}

/* ==================== 输出重定向函数 ==================== */

// 输出重定向函数，将标准输出和标准错误重定向到指定文件
int output_redirect(const char *log_path) 
{
    int fd;  // 文件描述符
    
    // 打开日志文件，创建不存在的文件，截断已存在的文件
    fd = open(log_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    
    // 将标准输出重定向到日志文件
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    // 将标准错误重定向到日志文件
    if (dup2(fd, STDERR_FILENO) == -1) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    // 关闭文件描述符（已通过dup2复制，原文件描述符可关闭）
    close(fd);
    return 0;
}

/* ==================== 登录认证函数 ==================== */

// 登录主函数//成功返回0，失败返回62
// 登录主函数//成功返回0，失败返回62
int login_authenticate()
{
	char user[64],*pass;//存储用户名和密码
	
	signal(SIGALRM, login_timeout);//注册SIGALRM信号的处理函数,触发时调用login_timeout函数
	alarm(TIMEOUT);//设置TIMEOUT秒后发送SIGALRM信号

	printf("---系统登陆---\n");
	printf("用户名:");
	scanf("%63s",user);//记录输入用户名
	getchar();//清除输入缓冲区

	pass = get_passwd_input("密码: ");//无回显输入密码

	alarm(0);//取消超时计时

	if(verify_user_passwd(user, pass))//校验用户名和密码
	{
		printf("登录成功！欢迎%s\n",user);
		return 0;
	}
	else
	{
		printf("认证失败：用户名或密码错误。\n");
		return -62;
	}

}

// 无回显读取密码 
char *get_passwd_input(const char *prompt)
{
	static char passwd[64];//静态存储密码
	struct termios new_t;//存储修改后的终端属性

	printf("%s", prompt);//输出"密码："
	fflush(stdout);//强制刷新输出缓冲区，让"密码："显示

	tcgetattr(STDIN_FILENO, &old_t);//获取当前终端的原始属性，保存到全局变量old_t
	new_t = old_t;//复制原属性到new_t
	new_t.c_lflag &= ~ECHO; // 关闭回显
	tcsetattr(STDIN_FILENO, TCSANOW, &new_t);//应用new_t配置

	if (fgets(passwd, sizeof(passwd), stdin) != NULL)//读取用户输入的密码
	{
		passwd[strcspn(passwd, "\n")] = 0;//替换'\n'为'\0'
	}

	reset_terminal();//恢复终端原始属性
	printf("\n");
	return passwd;
}

// 校验用户名密码//成功返回61，失败返回0
int verify_user_passwd(const char *user, const char *passwd)
{
	char line[256], u[64], h[128];
	//从文件中读取的数据(用户名+密码)
	//解析出的用户名
	//解析出的加密值
	char *new_hash = NULL;
	FILE *fp = NULL;

	fp = fopen(USER_DB, "r");//打开用户数据文件
	if (!fp) return 0;//用户数据文件打开失败

	while (fgets(line, sizeof(line), fp))//逐行读取用户数据库文件（直到文件末尾）
	{
		sscanf(line, "%[^:]:%s", u, h);//解析每行数据：按':'分割
		if (strcmp(u, user) == 0)//匹配用户名
		{
			// 使用存储的密文作为 salt 重新计算哈希
			new_hash = crypt(passwd, h);
			if (strcmp(new_hash, h) == 0)//对比哈希值
			{
				fclose(fp);
				return 61;
			}
		}
	}
	fclose(fp);
	return 0;
}

// 登录超时处理函数
void login_timeout(int sig)
{
	printf("\n[错误] 登录超时，请重新运行程序。\n");
	reset_terminal();//恢复中断设置
	exit(EXIT_FAILURE);//退出程序
}

// 恢复终端设置
void reset_terminal()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old_t);//使用old_t配置
}

// 显示登录提示
void show_login_prompt()
{
	//system("clear");//清屏

	printf("====================Embedded File Factory(嵌入式文件工厂)项目 ====================\n");
	printf("使用方法：./EFF [选项] 目标文件/目录\n");
	printf("  -l 文件名/目录      查看文件详细属性（模拟 ls -l）\n");
	printf("  -c 源文件 目标文件  复制普通文件（模拟 cp）\n");
	printf("  -v 文件名           查看文件内容（模拟 cat）\n");
	printf("  -d 目录名           查看目录下的所有子文件/子目录/子目录的子文件\n");
	printf("  -s 文件名/目录      统计磁盘占用（模拟 du -sh，单位 KB MB GB）\n");
	printf("  -h                  查看帮助指引\n");
	printf("  -R 源目录 目标目录  递归复制目录（支持子目录/文件）\n");
	printf("  -r 路径             删除文件/目录（模拟 rm -rf）\n");
	printf("  -n 旧路径 新路径    重命名文件/目录（模拟 mv）\n");
	printf("  -f 搜索模式         文件搜索（模拟 find -name 如 *.c, 支持通配符）\n");
	printf("  -m 权限 路径        修改文件权限（如 755, 八进制）\n");
	printf("  -b 模式 操作        批量操作（模式: *.c; 操作: l-属性 v-查看 s-统计）\n");
	printf("  -o 日志路径         输出重定向到指定文件\n");
	printf("=================================================================================\n");
}



void create_user()
{
	char user[64], *pass;
	char salt[12];// "$1$" + 8位随机码 + '\0'
	char *hash = NULL;
	FILE *fp;

	printf("--- 注册新用户 ---\n");
	printf("输入用户名: ");
	scanf("%63s", user);
	getchar(); //清除输入缓冲区

	pass = get_passwd_input("输入密码: ");//无回显输入密码

	generate_md5_salt(salt);//生成随机哈希值
	hash = crypt(pass, salt);//使用MD5算法加密
	if (hash == NULL) {
		perror("加密失败");
		return;
	}

	fp = fopen(USER_DB, "a");//以追加模式打开文件USER_DB(宏定义)
	if (fp == NULL) {
		perror("无法打开用户数据文件");
		return;
	}

	fprintf(fp, "%s:%s\n", user, hash);//记录用户和哈希值
	fclose(fp);
	printf("用户 %s 创建成功！\n\n", user);
}

//生成随机salt
void generate_md5_salt(char *salt_buf)
{
	const char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
	char rand_bytes[8];
	int fd, i;

	// 从系统读取真随机数
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0) {
		perror("open /dev/urandom");
		exit(1);
	}
	read(fd, rand_bytes, 8);
	close(fd);

	// 组装 Salt 字符串
	salt_buf[0] = '$';
	salt_buf[1] = '1';
	salt_buf[2] = '$';

	for (i = 0; i < 8; i++) {
		// 将随机字节映射到字符集
		salt_buf[i + 3] = charset[(unsigned char)rand_bytes[i] % 64];
	}
	salt_buf[11] = '\0'; // 结束符
}


// 检查是否已登录
int check_login_status()
{
    FILE *fp = fopen(".eff_login", "r");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}

// 保存登录状态
void save_login_status()
{
    FILE *fp = fopen(".eff_login", "w");
    if (fp) {
        fprintf(fp, "1\n");
        fclose(fp);
    }
}

// 清除登录状态
void clear_login_status()
{
    remove(".eff_login");
}






