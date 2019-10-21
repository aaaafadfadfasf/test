#ifndef __NIPDEBUG_NIPDEBUG_H__
#define __NIPDEBUG_NIPDEBUG_H__
#include <signal.h>

#define NO_SYMBOL_BT_SIZE 4	/* elf无符号，且没有指定符号文件，且没有指定大小时默认打印堆栈大小 */
#ifdef __cplusplus
extern "C"{
#endif

#define NF_NOOMIT_FRAME_POINT	0x1000
//#define NF_NO_DEBUGTHREAD		0x2000
#define NF_DEBUGTHREAD		0x4000
/* 
 * 说明：btsize最多显示回溯个数，为0则找到main函数为止
 * 	 symbol_file是符号文件（可执行文件本身包含符号表时会忽略此参数）
 *	 crushdump_file是程序异常的输出文件，为空会输出到控制台
 *	 signum信号id
 *	 NF_NOOMIT_FRAME_POINT针对没有优化堆栈帧的编译
 *	 NF_NO_DEBUGTHREAD表示不启用调试线程
 * 示例：init_nipdebug(0,  NULL, NULL, 0);
 *	 init_nipdebug(10, NULL, "/usr/cursh.out", 0);
 *	 init_nipdebug(0,  "/bin/ebt.sym", "/usr/cursh.out", 0);
 *	 init_nipdebug2(SIGPIPE,  "/bin/ebt.sym", "/usr/cursh.out", NF_NOOMIT_FRAME_POINT);
 */
int init_nipdebug(int btsize, const char *symbol_file, const char *crushdump_file, int flags);
int init_nipdebug2(int signum, int btsize, const char *symbol_file, const char *crushdump_file, int flags);

char *nipdebug_version();
#define NIPDEBUG__VERSION	"NIPDEBUG 1.1.4.20080401"
/***************************
模块版本：NIPDEBUG 1.1.2.20070726
增加功能：无
修改缺陷：初始化时删除了上一次的日志文件，异常时加入打印异常信号的值
提交人：王远涛
*****************************/

/***************************
模块版本：NIPDEBUG 1.1.3.20070911
增加功能：无
修改缺陷：和vlynq中定义了相同的全局变量，改为不同名称
提交人：王远涛
*****************************/

/***************************
模块版本：NIPDEBUG 1.1.4.20080401
增加功能：无
修改缺陷：如果没有使用omit-frame-point优化就使用堆栈帧返回
提交人：王远涛
*****************************/
#ifdef __cplusplus
}
#endif
#endif

