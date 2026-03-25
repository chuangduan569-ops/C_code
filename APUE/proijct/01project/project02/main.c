
#include "eff.h"
#include <getopt.h>
#include <sys/stat.h>

extern int g_buffer_size;   // 缓冲区(4096)

int main(int argc, char *argv[]) {
	int opt;                    // getopt返回值，存储当前解析的选项
	char *src = NULL;           // 源文件/源目录路径
	char *dest = NULL;          // 目标文件/目标目录路径
	char *pattern = NULL;       // 搜索模式(支持通配符)
	char batch_opt = 0;         // 批量操作类型: l-属性 v-查看 s-统计 d-删除
	struct stat st;             // 文件状态结构体，用于判断文件类型
	int choice;                 // 用户菜单选择
	int login_success = 0;      // 登录成功标志

	/* ========== 1. 没有参数时，显示登录/注册菜单 ========== */
	if (argc < 2) {
		while (1) {
			/* 显示主菜单 */
			printf("\n========================================\n");
			printf("    嵌入式文件工厂 (Embedded File Factory)\n");
			printf("========================================\n");
			printf("1. 注册新用户\n");
			printf("2. 登录\n");
			printf("3. 退出\n");
			printf("========================================\n");
			printf("请选择: ");

			/* 输入验证：只接受数字 */
			while (scanf("%d", &choice) != 1) {
				getchar();
				printf("请输入数字 (1-3): ");
			}
			getchar();  // 清除换行符

			switch (choice) {
				case 1:  // 注册新用户
					create_user();          // 调用 eff.c 中的注册函数
					break;

				case 2:  // 登录
					if (login_authenticate() == 0) {  // 登录成功返回0
						save_login_status();
						login_success = 1;
						printf("\n登录成功！欢迎使用嵌入式文件工厂\n");
						goto start_eff;      // 跳转到主程序
					} else {
						printf("登录失败，请重试\n");
					}
					break;

				case 3:  // 退出
					printf("再见！\n");
					return 0;

				default:
					printf("无效选择，请重新输入\n");
					break;
			}
		}
	}

start_eff:
	/* ========== 2. 有参数时，检查登录状态 ========== */
	/* 检查是否有命令行参数 */
	if (argc < 2) {
		show_help(argv[0]);
		return 0;
	}

	/* 处理退出登录命令 */
	if (strcmp(argv[1], "-logout") == 0) {
		clear_login_status();
		printf("已退出登录\n");
		return 0;
	}

	/* 除了 -h 选项外，其他所有操作都需要检查登录状态 */
	if (strcmp(argv[1], "-h") != 0 && !login_success) {
		/* 先检查是否已登录（有保存的登录状态文件） */
		if (!check_login_status()) {
			printf("\n========== 需要登录验证 ==========\n");

			/* 显示登录菜单 */
			while (1) {
				printf("1. 登录\n");
				printf("2. 注册新用户\n");
				printf("3. 退出\n");
				printf("请选择: ");

				scanf("%d", &choice);
				getchar();

				switch (choice) {
					case 1:
						if (login_authenticate() == 0) {
							save_login_status();     // 保存登录状态
							login_success = 1;
							goto login_success;
						} else {
							printf("登录失败，请重试\n");
						}
						break;

					case 2:
						create_user();
						printf("注册成功！请重新登录\n");
						break;

					case 3:
						printf("再见！\n");
						return 0;

					default:
						printf("无效选择\n");
						break;
				}
			}
		} else {
			login_success = 1;  // 已有登录状态文件，直接登录成功
		}
	}

login_success:
	if (login_success) {
		printf("=================================\n\n");
	}

	/* ========== 3. 显示登录提示 ========== */
	// show_login_prompt();     // 显示项目Logo和使用方法
	
	
	// 处理 -ls 命令
if (argc >= 2 && strcmp(argv[1], "-ls") == 0) {
    myls(argc - 1, argv + 1);
    return 0;
}

	/* ========== 4. 命令行参数解析 ========== */
	/* 
	 * getopt函数说明:
	 * - 选项字符串 "hc:v:r:m:l:s:d:f:b:p:o:n:R:"
	 * - 冒号(:)表示该选项需要一个参数
	 * - 无冒号表示该选项不带参数
	 * 
	 * 各选项含义:
	 * h: 帮助(无参数)
	 * c: 复制(需要源文件和目标文件两个参数)
	 * v: 查看文件(需要一个文件名参数)
	 * r: 删除(需要一个路径参数)
	 * m: 重命名(需要旧名和新名两个参数)
	 * l: 文件详细信息(需要一个路径参数)
	 * s: 磁盘统计(需要一个路径参数)
	 * d: 扫描目录(需要一个目录路径参数)
	 * f: 搜索文件(需要一个模式参数)
	 * b: 批量操作(需要模式和操作符两个参数)
	 * p: 修改权限(需要路径和权限两个参数)
	 * o: 输出重定向(需要一个日志文件路径参数)
	 * n: 重命名(需要旧名和新名两个参数)
	 * R: 递归复制目录(需要源目录和目标目录两个参数)
	 */
	while ((opt = getopt(argc, argv, "hc:v:r:m:l:s:d:f:b:p:o:n:R:")) != -1) {
		switch (opt) {
			/* ---------- 帮助选项 ---------- */
			case 'h':
				show_help(argv[0]);
				return 0;

				/* ---------- 复制选项：-c ---------- */
			case 'c': /* 复制文件或目录 */
				if (optind < argc) {
					src = optarg;               // 源文件/目录
					dest = argv[optind];        // 目标文件/目录
					optind++;

					/* 判断源路径类型：目录还是普通文件 */
					if (stat(src, &st) == 0 && S_ISDIR(st.st_mode)) {
						dir_copy_recursive(src, dest);  // 目录递归复制
					} else {
						file_cp(src, dest);             // 普通文件复制
					}
				} else {
					fprintf(stderr, "错误: -c 需要源文件和目标文件\n");
				}
				break;

				/* ---------- 查看文件选项：-v ---------- */
			case 'v': /* 查看文件内容 */
				file_view(optarg);
				break;

				/* ---------- 删除选项：-r ---------- */
			case 'r': /* 删除文件或目录 */
				file_rm(optarg);
				break;

				/* ---------- 重命名选项：-m ---------- */
			case 'm': /* 重命名文件或目录 */
				if (optind < argc) {
					src = optarg;               // 旧文件名
					dest = argv[optind];        // 新文件名
					optind++;
					file_rename(src, dest);
				} else {
					fprintf(stderr, "错误: -m 需要旧文件名和新文件名\n");
				}
				break;

				/* ---------- 文件详细信息选项：-l ---------- */
			case 'l': /* 查看文件详细信息（模拟 ls -l） */
				file_stat_l(optarg);
				break;

				/* ---------- 磁盘统计选项：-s ---------- */
			case 's': /* 统计磁盘占用（模拟 du -sh） */
				file_du(optarg);
				break;

				/* ---------- 扫描目录选项：-d ---------- */
			case 'd': /* 扫描目录内容 */
				dir_scan(optarg);
				break;

				/* ---------- 文件搜索选项：-f ---------- */
			case 'f': /* 搜索文件（支持通配符） */
				file_search(optarg);
				break;

				/* ---------- 批量操作选项：-b ---------- */
			case 'b':   // 批量操作选项
				if (optind < argc) {   // 检查是否有足够的命令行参数
					pattern = optarg;   // 获取文件匹配模式（如 "*.txt"）
					batch_opt = argv[optind][0];   // 获取操作符（l/v/s/d/p）
					if (batch_opt == 'p') {   // 判断是否为权限修改操作
						if (optind + 1 < argc) {   // 检查权限值参数是否存在
							g_batch_mode = argv[optind + 1];   // 存储权限值到全局变量
							optind += 2;   // 跳过操作符和权限值两个参数
							batch_operate(pattern, batch_opt);   // 调用批量操作函数
						} else {   // 缺少权限值参数
							fprintf(stderr, "错误: -b p 需要指定权限值\n");   // 输出错误提示
						}
					} else {   // 处理其他操作（l/v/s/d）
						optind++;   // 跳过操作符参数
						batch_operate(pattern, batch_opt);   // 调用批量操作函数
					}
				}
				break;

				/* ---------- 修改权限选项：-p ---------- */
			case 'p': /* 修改文件权限 */
				if (optind < argc) {
					char *path = optarg;            // 文件路径
					char *mode_str = argv[optind];  // 权限字符串（如"755"）
					optind++;
					file_chmod(path, mode_str);     // 调用权限修改函数
				} else {
					fprintf(stderr, "错误: -p 需要路径和权限\n");
				}
				break;

				/* ---------- 输出重定向选项：-o ---------- */
			case 'o': /* 输出重定向到指定文件 */
				output_redirect(optarg);
				break;

				/* ---------- 重命名选项：-n（与-m功能相同） ---------- */
			case 'n': /* 重命名文件或目录 */
				if (optind < argc) {
					src = optarg;
					dest = argv[optind];
					optind++;
					file_rename(src, dest);
				} else {
					fprintf(stderr, "错误: -n 需要旧文件名和新文件名\n");
				}
				break;

				/* ---------- 递归复制目录选项：-R ---------- */
			case 'R': /* 递归复制目录 */
				if (optind < argc) {
					src = optarg;
					dest = argv[optind];
					optind++;
					dir_copy_recursive(src, dest);
				} else {
					fprintf(stderr, "错误: -R 需要源目录和目标目录\n");
				}
				break;
					
			
	
				/* ---------- 未知选项 ---------- */
			default:
				fprintf(stderr, "未知选项，使用 -h 查看帮助\n");
				return 1;
		}
	}

	return 0;
}
