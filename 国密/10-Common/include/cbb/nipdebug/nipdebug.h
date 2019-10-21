#ifndef __NIPDEBUG_NIPDEBUG_H__
#define __NIPDEBUG_NIPDEBUG_H__
#include <signal.h>

#define NO_SYMBOL_BT_SIZE 4	/* elf�޷��ţ���û��ָ�������ļ�����û��ָ����СʱĬ�ϴ�ӡ��ջ��С */
#ifdef __cplusplus
extern "C"{
#endif

#define NF_NOOMIT_FRAME_POINT	0x1000
//#define NF_NO_DEBUGTHREAD		0x2000
#define NF_DEBUGTHREAD		0x4000
/* 
 * ˵����btsize�����ʾ���ݸ�����Ϊ0���ҵ�main����Ϊֹ
 * 	 symbol_file�Ƿ����ļ�����ִ���ļ�����������ű�ʱ����Դ˲�����
 *	 crushdump_file�ǳ����쳣������ļ���Ϊ�ջ����������̨
 *	 signum�ź�id
 *	 NF_NOOMIT_FRAME_POINT���û���Ż���ջ֡�ı���
 *	 NF_NO_DEBUGTHREAD��ʾ�����õ����߳�
 * ʾ����init_nipdebug(0,  NULL, NULL, 0);
 *	 init_nipdebug(10, NULL, "/usr/cursh.out", 0);
 *	 init_nipdebug(0,  "/bin/ebt.sym", "/usr/cursh.out", 0);
 *	 init_nipdebug2(SIGPIPE,  "/bin/ebt.sym", "/usr/cursh.out", NF_NOOMIT_FRAME_POINT);
 */
int init_nipdebug(int btsize, const char *symbol_file, const char *crushdump_file, int flags);
int init_nipdebug2(int signum, int btsize, const char *symbol_file, const char *crushdump_file, int flags);

char *nipdebug_version();
#define NIPDEBUG__VERSION	"NIPDEBUG 1.1.4.20080401"
/***************************
ģ��汾��NIPDEBUG 1.1.2.20070726
���ӹ��ܣ���
�޸�ȱ�ݣ���ʼ��ʱɾ������һ�ε���־�ļ����쳣ʱ�����ӡ�쳣�źŵ�ֵ
�ύ�ˣ���Զ��
*****************************/

/***************************
ģ��汾��NIPDEBUG 1.1.3.20070911
���ӹ��ܣ���
�޸�ȱ�ݣ���vlynq�ж�������ͬ��ȫ�ֱ�������Ϊ��ͬ����
�ύ�ˣ���Զ��
*****************************/

/***************************
ģ��汾��NIPDEBUG 1.1.4.20080401
���ӹ��ܣ���
�޸�ȱ�ݣ����û��ʹ��omit-frame-point�Ż���ʹ�ö�ջ֡����
�ύ�ˣ���Զ��
*****************************/
#ifdef __cplusplus
}
#endif
#endif

