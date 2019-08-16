/**
* @file		rpctrl.h
* @brief		¼������ṩ���ϲ�Ľӿ��ļ�
* @author	��С�¡�����
* @date		2007-04-11
* @version	4.0
* @copyright V4.0  Copyright(C) 2003-2005 KDC, All rights reserved.
*/

/**
 *@note								һЩ����

 * һ	·������ "/"��β���� "d:/asf/", ��Ҫ��"d:/asf"
 *	ֻ���ڿ�ʼ�ļ����ؽ��ղ����У��ļ���Ϊʵ�ʵ��ļ���(��a.asf)�����������ӦΪ�ļ�ǰ׺��(��a)
 *
 * ��	��ʼ¼��,��ʼʱ�����Ϊ����ʱ��(��Ϊ��λ),¼С�ļ�ʱ����Ϊ0��¼���ļ�����Ϊ0;
 *			 �����С�ļ�¼��ʱ����Ϊ[MIN_TIME_ITVL,MAX_TIME_ITVL];
 *			 ·�����ļ�������Ϊ��;
 *			 ¼С�ļ�ʱָ����Ŀ¼��������ڣ�¼���ļ�ʱָ����Ŀ¼���Դ���;
 *
 * ��	��ʼ����,��ʼʱ�����Ϊ����ʱ��(��Ϊ��λ),��С�ļ�ʱ����Ϊ0���Ŵ��ļ�ʱ����Ϊ0;
 *			 ����ʱ�����Ϊ����ʱ��(��Ϊ��λ),������ڿ�ʼʱ��,����Ϊ0����ʾ����������¼��;
 *			 ·�����ļ�������Ϊ��;
 *
 * ��  ��ʼ�ļ����ط���,��ʼʱ�����Ϊ����ʱ��(��Ϊ��λ),����Ϊ0;
 *			 ����ʱ�����Ϊ����ʱ��(��Ϊ��λ),������ڿ�ʼʱ��,����Ϊ0����ʾ����������¼��;
 *			 ·�����ļ�������Ϊ��;
 *
 * ��	ɾ���ļ���Ŀ¼Ϊ��Ԫ,Ŀ¼�������ļ���ɾ��,���ɾ��Ŀ¼;
 * 
 * ��  Ϊ�˼�㣬TNetAddr�е�m_wPort��ָrtp�˿ڣ������ڲ���rtcp�˿�����Ϊm_wPort + 1, ��������m_wPort���������2
 *
 * ��  �������ԭΪ�ٷֱȣ�����֮���Ϊ����ʱ�䣨��Ϊ���ļ�����Ϊ�ļ������ʱ�䣩�����Ե���RPGetPlayerStatis
 *			�õ����ŵļ�ʱʱ�����ʱ�䣬�ϲ����ٷֱ�;
 *
 */

#ifndef _RPCTRL_H_
#define _RPCTRL_H_

#include "osp.h"
#include "kdvdef.h"
#include "kdvmedianet.h"
#include "vbfslib.h"
#include "kdslib.h"


/** RPCTRL��İ汾��*/
#define  VER_RPCTRL								( const char * )"rpctrl_nru20 git 50.10.00.30.140506"

/** �����붨��*/
#define RP_OK                 					(u16)0					///<�����ɹ�
#define RPERR_BASE								(u16)2000				///<���������ʼֵ
#define RPERR_RP_ALREADY_INITIAL					(u16)(RPERR_BASE + 1)		///<���Ѿ���ʼ��
#define RPERR_RP_NOT_INITIAL						(u16)(RPERR_BASE + 2)		///<��δ��ʼ��
#define RPERR_NO_MEMORY							(u16)(RPERR_BASE + 3)		///<�ڴ治��
#define RPERR_NULL_POINT							(u16)(RPERR_BASE + 4)		///<ָ��Ϊ��
#define RPERR_PARTION_NUM							(u16)(RPERR_BASE + 5)		///<�ﵽ�������ķ���������Ŀǰδʹ��
#define RPERR_PARAM								(u16)(RPERR_BASE + 6)		///<��������
#define RPERR_NOT_CREATE							(u16)(RPERR_BASE + 7)		///<��Դδ����
#define RPERR_NOT_RELEASE							(u16)(RPERR_BASE + 8)		///<��Դδ�ͷ�
#define RPERR_UNKNOWN								(u16)(RPERR_BASE + 9)		///<δ֪����
#define RPERR_REC_NOT_EXIST						(u16)(RPERR_BASE + 10)		///<ָ��¼���������
#define RPERR_REC_STATUS_ERROR						(u16)(RPERR_BASE + 11)		///<¼���״̬���󣬼�ĳ�����������ڵ�ǰ��¼���״̬����
#define RPERR_NO_FREE_REC  						(u16)(RPERR_BASE + 12)		///<�Ҳ������е�¼���
#define RPERR_REC_OVER_CAPACITY						(u16)(RPERR_BASE + 13)		///<��������������¼�����������
#define RPERR_MEDIATYPE_CHANGE						(u16)(RPERR_BASE + 14)		///<ý�����͸ı�,��ǰδʹ��
#define RPERR_WAIT_KEYFRAME						(u16)(RPERR_BASE + 15)		///<�ȴ��ؼ�֡����ǰδʹ��
#define RPERR_TIMESTAMP_ERR						(u16)(RPERR_BASE + 16)		///<ʱ������󣬵�ǰδʹ��
#define RPERR_PREREC_TIME_INVALID					(u16)(RPERR_BASE + 17)		///<��Ч��Ԥ¼ʱ�䣬Ԥ¼ʱ�����Ч��Χ(0, MAX_PREREC_TIME]֮��
#define RPERR_SMALL_FILE_INTERVAL_INVALID			(u16)(RPERR_BASE + 18)		///<��Ч��С�ļ�ʱ��������Ч��Χ[MIN_TIME_ITVL, MAX_TIME_ITVL]֮��
#define RPERR_ALREADY_ALARM_TWO						(u16)(RPERR_BASE + 19)		///<���̷����Ѿ����ڶ����澯����ǰδʹ��
#define RPERR_PLAY_NOT_EXIST						(u16)(RPERR_BASE + 21)		///<ָ�������������
#define RPERR_PLAY_STATUS_ERROR						(u16)(RPERR_BASE + 22)		///<�����״̬���󣬼�ĳ�����������ڵ�ǰ�ķ����״̬����
#define RPERR_NO_FREE_PLY  						(u16)(RPERR_BASE + 23)		///<�Ҳ������еķ����
#define RPERR_PLAY_OVER_CAPACITY					(u16)(RPERR_BASE + 24)		///<��������������������������
#define RPERR_PLAY_DROG_TIME_INVALID				(u16)(RPERR_BASE + 25)		///<����ʱ�Ķ�λʱ����Ч���������ļ��Ŀ�ʼʱ�������ʱ��֮��
#define RPERR_PLAY_GRAN_INVALID						(u16)(RPERR_BASE + 26)		///<������ȵĻص�������Ч����Ч��Χ[MIN_READ_CALLBACK_GRAN, MAX_READ_CALLBACK_GRAN]
#define RPERR_PLAY_RECORD_LENGTH_ZERO				(u16)(RPERR_BASE + 27)		///<¼���ļ�ʱ��Ϊ0
#define RPERR_PLAY_REACH_STOP_TIME					(u16)(RPERR_BASE + 28)		///<�Ѿ����ŵ�����ʱ��
#define RPERR_PLAY_MODE_LOCKED                      (u16)(RPERR_BASE + 29)      ///<��֡ģʽ����>
#define RPERR_MEM_PARAM							(u16)(RPERR_BASE + 31)		///<����ѭ������ʱ�Ĳ�������
#define RPERR_MEM_NULL								(u16)(RPERR_BASE + 32)		///<ѭ�������
#define RPERR_MEM_INVALID							(u16)(RPERR_BASE + 33)		///<ѭ��������Ч
#define RPERR_MEM_FULL								(u16)(RPERR_BASE + 34)		///<ѭ��������
#define RPERR_MEM_DATA_ERR							(u16)(RPERR_BASE + 35)		///<ѭ���������ݴ���Ŀǰδʹ��
#define RPERR_MEDIATYPE_UNSUPPORT					(u16)(RPERR_BASE + 41)		///<ý�����Ͳ�֧��
#define RPERR_STREAM_IDX_INVALID					(u16)(RPERR_BASE + 42)		///<ý���������Ч����Ч��Χ[0, MAX_STREAM_NUM)
#define RPERR_PARTION_NAME_INVALID					(u16)(RPERR_BASE + 43)		///<������̫������Ч��Χ(0, MAX_PARTION_NAME_LEN]
#define RPERR_ALARM_VALUE_INVALID					(u16)(RPERR_BASE + 44)		///<���õĸ澯��ֵ��Ч����Чֵ�������0����һ���澯��ֵ������ڶ����澯��ֵ
#define RPERR_START_TIME_INVALID					(u16)(RPERR_BASE + 45)		///<��ʼʱ����Ч����ʼʱ��������0�����������ʱ�䲻Ϊ0����ʼʱ�䰴����С�ڽ���ʱ��
#define RPERR_PATH_ALREADY_EXIST					(u16)(RPERR_BASE + 51)		///<Ŀ¼�Ѿ�����
#define RPERR_PATH_NOT_EXIST						(u16)(RPERR_BASE + 52)		///<Ŀ¼������
#define RPERR_PATH_CREATE_FAIL						(u16)(RPERR_BASE + 53)		///<Ŀ¼����ʧ��
#define RPERR_PATH_DELETE_FAIL						(u16)(RPERR_BASE + 54)		///<Ŀ¼ɾ��ʧ��
#define RPERR_FILE_PATH_INVALID						(u16)(RPERR_BASE + 55)		///<�ļ�Ŀ¼����Ч����ЧĿ¼���뱣֤��'/'��β�����ҳ�����(0, MAX_FILE_PATH_LEN]֮��
#define RPERR_TOO_MANY_LINKS						(u16)(RPERR_BASE + 56)		///<linuxϵͳ�£�Ŀ¼����������ϵͳ������������
#define RPERR_NOT_ADD_DISK							(u16)(RPERR_BASE + 57)		///<����û�м�������б�
#define	RPERR_DISK_DIED							(u16)(RPERR_BASE + 58)		///<���̷�������
#define RPERR_DISK_MGR_FULL						(u16)(RPERR_BASE + 59)		///<�����ɹ��������������
#define RPERR_FILE_ALREADY_EXIST					(u16)(RPERR_BASE + 61)		///<�ļ��Ѿ�����
#define RPERR_FILE_NOT_EXIST						(u16)(RPERR_BASE + 62)		///<	�ļ�������
#define RPERR_FILE_DELETE_FAIL						(u16)(RPERR_BASE + 63)		///<�ļ�ɾ��ʧ��
#define RPERR_FILE_RENAME_FAIL						(u16)(RPERR_BASE + 64)		///<�ļ�������ʧ��
#define RPERR_FILE_NAME_INVALID						(u16)(RPERR_BASE + 65)		///<�ļ���������Ч����Ч��Χ(0, MAX_FILE_NAME_LEN]
#define RPERR_RECLOG_FILE_CREATE_FAIL				(u16)(RPERR_BASE + 71)		///<reclog.txt�ļ�����ʧ��
#define RPERR_RECLOG_FILE_NOT_EXIST					(u16)(RPERR_BASE + 72)		///<reclog.txt�ļ�������
#define RPERR_RECLOG_FILE_OP_FAIL					(u16)(RPERR_BASE + 73)		///<reclog.txt�ļ�����ʧ��
#define RPERR_PREREC_FILE_CREATE_FAIL				(u16)(RPERR_BASE + 74)		///<Ԥ¼�ļ�����ʧ��
#define RPERR_NO_FREE_FTPRCV  						(u16)(RPERR_BASE + 81)		///<�Ҳ������е����ؽ���ͨ��
#define RPERR_FTPRCV_NOT_EXIST  					(u16)(RPERR_BASE + 82)		///<ָ�����ؽ���ͨ��������
#define RPERR_FTPRCV_STATUS_ERROR  					(u16)(RPERR_BASE + 83)		///<�����ڸ����ؽ���״̬���ò���
#define RPERR_FTPRCV_LISTEN_SOCKET_ERROR			(u16)(RPERR_BASE + 84)		///<���ؽ�����Socket�ڼ�����Ŀǰδʹ��
#define RPERR_FTPRCV_LISTEN_TIMEOUT					(u16)(RPERR_BASE + 85)		///<������ʱ
#define RPERR_IP_OR_PORT_MUST_NO_ZERO				(u16)(RPERR_BASE + 86)		///<������ַ�Ͷ˿ڲ���Ϊ0
#define RPERR_FTPRCV_LISTEN_ERROR  					(u16)(RPERR_BASE + 87)		///<��������
#define RPERR_FTPRCV_ACCEPT_ERROR  					(u16)(RPERR_BASE + 88)		///<���տͻ������Ӵ���
#define RPERR_FTPRCV_CLIENT_ERROR  					(u16)(RPERR_BASE + 89)		///<���տͻ��˴���
#define RPERR_NO_FREE_FTPSND  						(u16)(RPERR_BASE + 91)		///<�Ҳ������е����ط���ͨ��
#define RPERR_FTPSND_NOT_EXIST  					(u16)(RPERR_BASE + 92)		///<ָ�����ط���ͨ��������
#define RPERR_FTPSND_STATUS_ERROR  					(u16)(RPERR_BASE + 93)		///<�����ڸ����ط���״̬���ò���
#define RPERR_FTPSND_SOCKET_ERROR  					(u16)(RPERR_BASE + 94)		///<�������ط����׽���ʧ��
#define RPERR_FTPSND_CONNECT_ERROR  				(u16)(RPERR_BASE + 95)		///<�������ؽ��շ�ʧ��
#define RPERR_RPCTL_BUG								(u16)(RPERR_BASE + 96)		///<����ʱrpctrl�ڲ����쳣
#define RPERR_INODE_DESTROYED_ERROR					(u16)(RPERR_BASE + 97)		///<Inode����
#define RPERR_UNLOAD_DISK_ABNORMAL					(u16)(RPERR_BASE + 98)		///<ж�ش��̷����쳣
#define RPERR_METHOD_NOTSUPPORTED					(u16)(RPERR_BASE + 99)		///<������֧��
#define RPERR_FILE_LOCKED							(u16)(RPERR_BASE + 100)		///<�ļ��Ѿ��ӹ���

/** add zhx ftp udp�����й�*/
#define RP_ERR_FTP_PARAM							(u16)(RPERR_BASE+101)		///<���ز�������
#define RP_ERR_FTP_SOCKET							(u16)(RPERR_BASE+102)		///<�׽�����ز�������
#define RP_ERR_FTP_TASK_CREATE						(u16)(RPERR_BASE+103)		///<���������߳�ʧ��
#define RP_ERR_SND_SERVICE_NOT_START				(u16)(RPERR_BASE+104)		///<���صķ����߳��޴�������δ����
#define RP_ERR_FTP_NEW_OBJECT						(u16)(RPERR_BASE+105)		///<�������ض���ʧ��
#define RP_ERR_FTP_MAX_CAPACITY						(u16)(RPERR_BASE+106)		///<���������������ز�����MAX_RP_FTP_SND_NUM
#define RP_ERR_FTP_DATA_SCARE						(u16)(RPERR_BASE+107)		///<�������ݴ���
#define RP_ERR_FTP_PACK_NO_INDEX					(u16)(RPERR_BASE+108)		///<������δʹ��
#define RP_ERR_OBJECT_NOT_EXIST						(u16)(RPERR_BASE+109)		///<���ض��󲻴���
#define RP_ERR_FTP_STATUS_ERR						(u16)(RPERR_BASE+110)		///<����״̬����
#define RP_ERR_FTP_TOOMUCH_PROXY_DATA				(u16)(RPERR_BASE+111)		///<Proxy���ݳ����������󳤶�MAX_PROXY_USER_DATA

#define RPERR_FILE_UNLOCKED							(u16)(RPERR_BASE + 120)		///<�ļ�������δ����

/** add zhx 06-04-18 �ļ�Ԥ¼�й�*/
//#define RP_ERR_FILE_BUF_FILENULL					(u16)(RPERR_BASE + 120)	///<δʹ��
//#define RP_ERR_FILE_BUF_PARAM						(u16)(RPERR_BASE + 121)	///<��������δʹ��
//#define RP_ERR_FILE_BUF_FOPEN						(u16)(RPERR_BASE + 122)	///<fopen����
//#define RP_ERR_FILE_BUF_NODATA					(u16)(RPERR_BASE + 123)	///<û�����ݴ���
//#define RP_ERR_FILE_BUF_FREAD						(u16)(RPERR_BASE + 124)	///<fread����
//#define RP_ERR_FILE_BUF_FWRITE					(u16)(RPERR_BASE + 125)	///<fwrite����
//#define RP_ERR_FILE_BUF_FSEEK						(u16)(RPERR_BASE + 126)	///<fseek����
//#define RP_ERR_FILE_BUF_DATAERR					(u16)(RPERR_BASE + 127)	///<���ݻ��Ҵ���
//#define RP_ERR_FILE_BUF_NEW						(u16)(RPERR_BASE + 131)	///<���仺�����

#define RP_ERR_OP_NOT_PERMISSION					(u16)(RPERR_BASE + 140)	///<������Ĳ���
#define RP_ERR_NO_SUCH_RP_TYPE						(u16)(RPERR_BASE + 141)	///<��Ч����������
#define RP_ERR_REACH_MAX_REG						(u16)(RPERR_BASE + 142)	///<�ﵽ�����������ͻص�ע����MAX_SND_CB_REG_NUM
#define RP_ERR_NOT_FIND_REG						(u16)(RPERR_BASE + 143)	///<û���ҵ����ͻص���Ϣ
#define RP_ERR_CB_NOT_REG							(u16)(RPERR_BASE + 144)	///<���ͻص�û��ע��
#define RP_ERR_DS_REG_FAIL							(u16)(RPERR_BASE + 145)	///<ע�ᷢ�ͻص�ʧ��
#define RP_ERR_CMD_TIME_OUT						(u16)(RPERR_BASE + 146)	///<�����ʱ
#define RP_ERR_FILEVER_TOOLOW						(u16)(RPERR_BASE + 147)	///<�ļ��汾̫��
#define RPERR_IO_BUSY								(u16)(RPERR_BASE + 150)	///<IO����
#define RPERR_FILE_IS_LOCKED						(u16)(RPERR_BASE + 151)	///<�ļ�������
#define RPERR_ADDSTREAM							(u16)(RPERR_BASE + 152)	///<���������ʧ��
#define RPERR_GET_FILEINFO							(u16)(RPERR_BASE + 153)	///<��ȡ�ļ���Ϣʧ��
#define RPERR_GET_STREAM_PROPERTY					(u16)(RPERR_BASE + 154)	///<��ȡ������ʧ��
#define RPERR_FILE_OPEN							(u16)(RPERR_BASE + 155)	///<�ļ���ʧ��

#define     RPCTRL_CREATE_TASK_ERR					(15800+1)				///<�����߳�ʧ��
#define     RPCTRL_CREATE_TASK_TIMEOUT				(15000+2)				///<�����̳߳�ʱ
#define     RPCTRL_CREATE_SEM_ERR					(15800+3)				///<�����ź�����ʱ
//#define     RPCTRL_NEW_ASF_ERR						(15800+4)			
#define     RPCTRL_ASF_NOT_FOUND					(15800+5)				///<¼��������δ�ҵ�

//#define     RPCTRL_ASF_MGR_EXIST					(15800+6)				
#define     RPCTRL_ASF_MGR_FULL					(15800+7)				///<������������������
#define     RPCTRL_ASF_MGR_NOT_FOUND				(15800+8)				///<δ�ҵ�����������
#define     RPCTRL_ASF_MGR_NEW_FAIL					(15800+9)				///<���������󴴽�ʧ��
#define     RPCTRL_STREAM_BUFF_NEW_FAIL				(15800+10)				///<֡���崴��ʧ��
#define     RPCTRL_STREAM_BUFF_EMPTY				(15800+11)				///<֡�����
#define     RPCTRL_STREAM_BUFF_FULL					(15800+12)				///<֡������
#define     RPCTRL_STREAM_BUFF_MALLOC_FAIL			(15800+13)				///<��֡�����з����ڴ�ʧ��
#define     RPCTRL_RECASF_BIND_MGR_FAIL				(15800+14)				///<������󶨵��������ʧ��

//#define     RPCTRL_RECASF_CMD_TIMEOUT				(15800+15)				
#define     RPCTRL_RECASF_CMD_BUSY					(15800+16)				///<�в������ڴ���
#define     RPCTRL_RECASF_NEW_ASFWRITE_FAIL			(15800+17)				///<�����ļ������ʧ��
#define     RPCTRL_RECASF_NEW_MEMMGR_FAIL			(15800+18)				///<¼���ڴ������󴴽�ʧ��
#define     RPCTRL_RECASF_NEW_RECASFMGR_FAIL			(15800+19)				///<����¼��������Ĺ������ʧ��
#define     RPCTRL_RECASF_WAIT_FRAME				(15800+20)				///<�ȴ�����֡
#define     RPCTRL_RECASF_WAIT_TIME					(15800+21)				///<�ȴ���ȡ�������������ݣ�����ǰ���������ݻ�û�е���ȡ��ʱ��
#define     RPCTRL_RECASF_STATE_ERROR				(15800+22)				///<¼������״̬����
#define     RPCTRL_RECASF_STREAMTYPE_ERROR			(15800+23)				///<ý�������ʹ�����Чֵ[0, MAX_STREAM_NUM)
#define     RPCTRL_RECASF_WAIT_KEYFRAME				(15800+24)				///<��Ҫ�ȴ��ؼ�֡
//#define     RPCTRL_RECASF_WAIT_VIDEOFRAME			(15800+25)				
//#define     RPCTRL_RECASF_BUFF_NOT_CREATE			(15800+26)				
#define     RPCTRL_RECASF_MGR_NOT_SAME				(15800+27)				///<¼��������仯
#define     RPCTRL_RECASF_MGR_NOT_REG				(15800+28)				///<¼������û�й�����
#define     RPCTRL_RECASF_FORMAT_CHANGE				(15800+29)				///<¼��������ý�����Ա仯����Ҫָ�غ����͡�����
//#define     RPCTRL_RECASF_CMD_ACK_NOTMATCH			(15000+30)				///<
//#define     RPCTRL_PLAYASF_NEW_ASFREAD_FAIL			(15800+40)			
#define     RPCTRL_PLAYASF_NEW_MEMMGR_FAIL			(15800+41)				///<�����ڴ������󴴽�ʧ��
//#define     RPCTRL_PLAYASF_BIND_MGR_FAIL			(15800+42)				
#define     RPCTRL_PLAYASF_MGR_IS_NOT_SAME			(15800+43)				///<����������仯
//#define     RPCTRL_PLAYASF_CMD_BUSY					(15800+44)			
//#define     RPCTRL_PLAYASF_CMD_TIMEOUT				(15800+45)				
#define     RPCTRL_PLAYASF_STATE_ERROR				(15800+46)				///<��������״̬����
#define     RPCTRL_PLAYASF_WAIT_TIME				(15800+47)				///<�ȴ���ȡ�������������з���
#define     RPCTRL_PLAYASF_PARAM_ERROR				(15800+48)				///<�����������
#define     RPCTRL_PLAYASF_MGR_NOT_REG				(15800+49)				///<��������û�й�����
#define     RPCTRL_RECASF_BIGFILE_SWITCH			(15800+50)				///<���ļ��л�

/** ¼�����Ͷ���*/
#define RP_NORMAL_RECORD					0			///<֡���ݣ�¼��Ϊasf�ļ���ʽ
#define RP_UNKNOWN_KEDA_RECORD				1			///<�����ݣ�֧���ش�����Ҫ�����ؼ�֡�Ϳ����Ϣ��¼��Ϊkds�ļ���ʽ��
#define RP_UNKNOWN_OTHERNOTRTP_RECORD		100			///<�����ݣ���֧���ش���������������¼��Ϊkds�ļ���ʽ��ä¼
#define	RP_PS_RECORD						101			///<��ǰδʹ��
#define	RP_KSJ_RECORD						102			///<֡���ݣ�¼��Ϊksj�ļ���ʽ
#define RP_MP4_RECORD						103			///<֡���ݣ�¼��Ϊmp4�ļ���ʽ
#define RP_UNKNOWN_OTHERRTP_RECORD			104			///<�����ݣ�֧���ش���������������¼��Ϊkds�ļ���ʽ,ä¼

/** �������Ͷ���*/
#define RP_NORMAL_PLAY						0			///<���ŵ��ļ�Ϊasf�ļ���Ŀǰ��֧�ֵ���
#define RP_UNKNOWN_KEDA_PLAY				1			///<���ŵ��ļ�Ϊkds�ļ���֧�����еĲ��Ų���
#define RP_UNKNOWN_OTHERNOTRTP_PLAY			100			///<���ŵ��ļ�Ϊkds�ļ�����֧����ؼ�֡�йص��κβ���
													///<���磬���š���λ���ؼ�֡����ؼ�֡����
//#define	RP_PS_PLAY						101
#define RP_KSJ_PLAY						102			///<���ŵ��ļ�Ϊksj�ļ���Ŀǰ��֧�ֵ���
#define RP_MP4_PLAY						103			///<���ŵ��ļ�Ϊmp4�ļ���Ŀǰ��֧�ֵ���
#define RP_UNKNOWN_OTHERRTP_PLAY			104			///<���ŵ��ļ�Ϊkds�ļ�����֧����ؼ�֡�йص��κβ���
													///<���磬���š���λ���ؼ�֡����ؼ�֡����

/** �������Ͷ���*/
#define RP_NORMAL_DOWN						0			///<���ص�ԭ�ļ�Ϊasf�ļ������ص����ݸ��ϲ�ҵ��rpctrl���ڲ�ֱ��д�ļ�
#define RP_KSJ_DOWN						1			///<���ص�ԭ�ļ�Ϊksj�ļ������ص����ݸ��ϲ�ҵ��rpctrl���ڲ�ֱ��д�ļ�
#define RP_UNKNOWN_KEDA_DOWN				2			///<���ص�ԭ�ļ�Ϊkds�ļ����յ��İ����ݻ�ص����ϲ�ҵ�����udp2asfд��asf����mp4�ļ�
#define	RP_PS_DOWN						3
#define RP_UNKNOWN_OTHER_DOWN				4			///<���ص�ԭ�ļ�Ϊkds�ļ����յ��İ����ݻ�ص����ϲ�ҵ���������ݰ�Ϊ��������������������ʹ��udp2asf
#define RP_THIRDCOMPANY_FILE_DOWN			5			///<���������ԭ�����ɵ������ṩ���������ط����rpctrlͨ��ֱ�Ӷ�ȡ���̷����ϵ��ļ��õ�
#define RP_MP4_DOWN						6			///<���ص�ԭ�ļ�Ϊmp4�ļ������ص����ݸ��ϲ�ҵ��rpctrl���ڲ�ֱ��д�ļ�

/** ��Ч����Դ���*/
#define	RP_INVALID_ID				((u32)-1)

/** RPCTRL��֧�ֵ���������*/
#define		MAX_PARTION_NUM						64

/** ����ʹ�ÿռ�ص��¼�����*/
#define	RP_DISK_RESUME_NORMAL						(u8)0			///<���̿ռ�ָ�������FreeSpaceAlarmCallBack�ص������ĵ���������Ϊ����ʣ��ռ��С
#define	RP_DISK_ALARM_LEVEL_ONE					(u8)1			///<����ʣ��ռ�ﵽ����һ���澯��ֵ��FreeSpaceAlarmCallBack�ص������ĵ���������Ϊ����ʣ��ռ��С
#define	RP_DISK_ALARM_LEVEL_TWO					(u8)2			///<����ʣ��ռ�ﵽ���̶����澯��ֵ��FreeSpaceAlarmCallBack�ص������ĵ���������Ϊ����ʣ��ռ��С
#define	RP_DISK_SPACE_NOTIFY						(u8)3			///<���̿ռ��ϱ���FreeSpaceAlarmCallBack�ص������ĵ���������Ϊ����ʣ��ռ��С

/** ����״̬�ص��¼�����*/
#define	RP_DISK_STATE_DIED						(u8)4			///<���̷������ߣ�FreeSpaceAlarmCallBack�ص������ĵ���������ΪNULL_REC_NO
#define	RP_DISK_STATE_RECOVER						(u8)5			///<���̷����ָ����ߣ�FreeSpaceAlarmCallBack�ص������ĵ���������ΪNULL_REC_NO

#define	RP_DISK_COVER_RECORD						(u8)6			///<��vbfs�����ϵ�¼�񱻸���ɾ����FreeSpaceAlarmCallBack�ص������ĵڶ�������Ϊ�����ǵ��ļ���������������Ϊ�ļ�������

/** RPCTRL��֧�ֵ����ý��������*/
#define MAX_STREAM_NUM								(u8)3

/** ��Ч��¼������*/
#define NULL_REC_NO								(u32)~0

/** ��Ч�ķ������ţ����������Դʧ��ʱ��������RPCreatePlay()ʧ��ʱ���ظñ��*/
#define NULL_PLY_NO								(u32)~0

/** ��Ч�����ؽ��ձ�ţ�RPCreateFtpRcv()����ʧ��ʱ���ظñ��*/
#define NULL_FTPRCV_NO								(u8)255

/** ��Ч�����ط��ͱ�ţ�RPCreateFtpSnd()����ʧ��ʱ���ظñ��*/
#define NULL_FTPSND_NO								(u8)255

/** �ȴ����ط��ͷ��������ӵĳ�ʱʱ��*/
#define MAX_FTPRCV_LISTEN_TIMEOUT					(u32)8

/** RPCTRL����������¼�������*/
#define MAX_RECORDER_CAPACITY						(u32)1024

/** RPCTRL������������������*/
#define MAX_PLAYER_CAPACITY						(u32)1024

/** RPCTRL������ͬʱ���е����TCP������·��*/
#define MAX_FTP_CHNS								(u8)4

/** RPCTRL��֧�ֵ�������������*/
#define  MAX_PARTION_NAME_LEN						(u8)255

/** RPCTRL��֧�ֵ�����ļ�·������*/
#define	MAX_FILE_PATH_LEN							(u8)255

/** RPCTRL��֧�ֵ�����ļ�������*/
#define	MAX_FILE_NAME_LEN							(u8)255

/** RPCTRL��֧�ֵ����Ԥ¼ʱ�䣬��λ��*/
#define MAX_PREREC_TIME							(u32)60

/** RPCTRL��֧�ֵ����Ԥ¼�����С��Ŀǰδʹ��*/
//#define MAX_PREREC_SIZE							(u32)((MAX_PREREC_TIME * 25 * 8) << 10)

/** RPCTRL��֧�ֵ����Ԥ¼֡����Ŀǰδʹ��*/
//#define MAX_PREREC_FRMN_NUM						(u32)10000

/** ����ű���*/
//#define MAX_QUICKPLAY_TIMES						(u8)4

/** ������ű���*/
//#define MAX_SLOWPLAY_TIMES						(u8)4

/** RPCTRL���ļ��б������ɴ�ŵ��ļ���*/
#define MAX_FILE_NUMBER							(u32)1000

/** RPCTRL��֧�ֵ���С���ļ��ص����ȣ���λ��*/
#define MIN_READ_CALLBACK_GRAN						(u8)1

/** RPCTRL��֧�ֵ�Ĭ�϶��ļ��ص����ȣ���λ��*/
//#define DEFAULT_READCB_GRAN						(u8)5

/** RPCTRL��֧�ֵ������ļ��ص����ȣ���λ��*/
#define MAX_READ_CALLBACK_GRAN						(u8)60

/** RPCTRL��֧�ֵ���СС�ļ�ʱ��������λ��*/
#define MIN_TIME_ITVL								(u32)10

/** RPCTRL��Ĭ�ϵ�С�ļ�ʱ��������λ��*/
//#define DEFAULT_TIME_ITVL						(u32)60

/** RPCTRL��֧�ֵ����С�ļ�ʱ��������λ��*/
#define MAX_TIME_ITVL								(u32)600

/** ���л������*/
//#define MAX_BUF_NUM								(s32)16

//#define BUF_STARTREAD_NUM						(s32)1
//#define BUF_FASTREAD_NUM							(s32)13

/** ������ʱ�䣬Ŀǰ��Ҫ���������У����ϲ����õ�����ʱ��Ϊ0ʱ���ڲ�ʹ�ø�ֵ��Ϊ����ʱ��*/
#define MAX_PLAY_TIME								(u32)-1

/** MediaPack�����Ƶ���ջ����С����λ�ֽڣ�Ŀǰ�ѷ�������*/
#define MEDIAPACK_RECV_BUFF						256*1024

/** MediaPack�����Ƶ���ջ����С����λ�ֽڣ�Ŀǰ�ѷ�������*/
#define MEDIAPACK_RECV_BUFF_AUDIO					64* 1024

/** MediaPack�����Ƶ���ͻ����С����λ�ֽ�*/
#define MEDIAPACK_SEND_BUFF						1024*1024		///<��128*1024�������� 20091117

/** MediaPack�����Ƶ���ͻ����С����λ�ֽ�*/
#define MEDIAPACK_SEND_BUFF_AUDIO					512* 1024

/** MediaPack�����Ƶ���ͻ����С�Ĺؼ�֡����*/
#define MEDIAPACK_SEND_BUFF_KEYFRAME_NUM			5
#define MEDIAPACK_SEND_BUFF_KEYFRAME_NUM_MIN		2

/** RPCTRL����������ע�ắ������*/
#define MAX_SND_CB_REG_NUM							256

/**
 *@brief			ʣ��洢�ռ�澯�ص���������
 *@param[out]		u8 byAlarmLevel  �ص��¼�������RP_DISK_RESUME_NORMAL��
 *@param[out]		s8* pchPartionName ���̷��������ڻص��¼�ΪRP_DISK_COVER_RECORDʱ���ò���Ϊ����ɾ�����ļ���
 *@param[out]		u64 dwValue		����������ص��¼�ʱ��ע��
 *@param[out]		KD_PTR pvContext  �ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*FreeSpaceAlarmCallBack)( u8 byAlarmLevel, s8* pchPartionName, u64 dwValue, KD_PTR pvContext );

/**
 *@brief			�ļ����ؽ��Ȼص��������壬�����TCP��ʽ����ʱ
 *@param[out]		u8 byFtpSndID      ������Դ���
 *@param[out]		u32 dwCurrentFtpTime  ��ǰ���ؽ��ȣ�����ʱ�䣬��λ��
 *@param[out]		KD_PTR pvContext  �ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*FtpFileRateCallBack)( u8 byFtpSndID, u32 dwCurrentFtpTime, KD_PTR pvContext );

/**
 *@brief			С�ļ������ص���������
 *@param[out]		u32 dwRecorderID      ¼�����ԴID
 *@param[out]		s8* pchFileName       ������С�ļ���
 *@param[out]		u32 dwStoptime        С�ļ��Ľ���ʱ�䣬����ʱ��
 *@param[out]		u32 dwRecSize         ������С�ļ��Ĵ�С����λK
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecFileEndCallBack)( u32 dwRecorderID, s8* pchFileName, u32 dwStoptime, u32 dwRecSize, KD_PTR pvContext );

/**
 *@brief			���ļ��л��ص���������
 *@param[out]		u32 dwRecorderID      ¼�����ԴID
 *@param[out]		s8* pchFileName       �����Ĵ��ļ����ļ���
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecBigFileSwCallBack)( u32 dwRecorderID, s8* pchFileName, KD_PTR pvContext );

/**
 *@brief			��ȡ�ļ����Ȼص���������
 *@param[out]		u32 dwPlayerID		�����ID
 *@param[out]		u32 dwCurrentPlayTime	��ǰ����ʱ�䣬����ʱ�䣬MAX_PLAY_TIMEʱ��ʾ�������
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*ReadFileRateCallBack)( u32 dwPlayerID, u32 dwCurrentPlayTime, KD_PTR pvContext );

/** ¼��״̬�ص����Ͷ���*/
typedef enum
{
    RECORDER_REC_START = 1,			///<¼��ɹ��������ص�����ΪTRecorderRecStartInfo
    RECORDER_REC_STOP,				///<¼��ֹͣ���ص�����ΪTRecorderRecStopInfo
	RECORDER_REC_FILE_START ,			///<¼���ļ��򿪳ɹ���������
	RECORDER_REC_START_TIME,			///<¼���ļ���ʼʱ��ص����ص�����ΪTRecorderRecStartTime
    RECORDER_PREREC_START,				///<Ԥ¼��ʼ��������
    RECORDER_PREREC_STOP,				///<Ԥ¼ֹͣ���ص�����ΪTRecorderPreRecStopInfo
	RECORDER_STREAM_STOP,				///<δʹ��
	RECORDER_BIGFILE_SWITCH,			///<���ļ��л����ص�����ΪTBigRecFileSwitchInfo
} ERecorderCBType;

/** ����״̬�ص����Ͷ���*/
typedef enum
{
	PLAYER_CB_START = 1 ,				///<����ʼ��δʹ��
	PLAYER_CB_STOP ,					///<����������ص�����ΪTPlayStopInfo
	PLAYER_CB_PAUSE,					///<������ͣ��δʹ��
	PLAYER_CB_RESUME,					///<����ָ���δʹ��
	PLAYER_CB_QUICK,					///<��ţ�δʹ��
	PLAYER_CB_SLOW,					///<���ţ�δʹ��
	PLAYER_CB_ONEFRAME ,				///<��֡���ţ�δʹ��
} EPlayerCBType ;

/** ¼��ֹͣԭ��*/
typedef enum
{
    STOPREASON_NORMAL = 0,				///<¼������ֹͣ
    STOPREASON_FORMAT_CHG = 1,			///<�������غ����ͻ��߷ֱ��ʸı������ֹͣ¼��
    STOPREASON_FILE_FAIL = 2,			///<д�ļ�ʧ�������¼��ֹͣ
	STOPREASON_BIGFILE_SWITCH = 3,		///<���ļ��л������¼��ֹͣ
	STOPREASON_VBFS_NOMEM = 4,			///<vbfs û���ڴ�
} ERecorderStopReason;

/*typedef struct  
{

} TRecorderFileStart ;*/

/** ¼��ʼʱ��ص��¼�RECORDER_REC_START_TIME�����ݽṹ��*/
typedef struct 
{
	u32 m_dwStartTime ;				///<¼��ʼʱ�䣬����ʱ��
} TRecorderRecStartTime ;

/** ¼��ʼ�ص��¼�RECORDER_REC_START�����ݽṹ��*/
typedef struct
{
    
} TRecorderRecStartInfo;

/** ¼��ֹͣ�¼�RECORDER_REC_STOP�����ݽṹ��*/
typedef struct
{
    u16 m_wStopReason;				///<¼��ֹͣԭ�򣬲μ�ERecorderStopReason
	u64 m_qwRecTotalSize;				///<¼��ֹͣʱ���ļ���С����λ�ֽ�
    u64 m_qwTotalDiskUsage;			///<¼��ֹͣʱռ�õĴ��̿ռ��С����λ�ֽڣ���С�ļ�¼��ʱ������m_qwRecTotalSize+reclog.txt�ļ��Ĵ�С
	u32 m_dwDuration;					///<¼��ֹͣʱ��¼���ļ���ʱ������λ��
} TRecorderRecStopInfo;

/*typedef struct
{
    
} TRecorderPreRecStartInfo;*/

/** Ԥ¼ֹͣ�¼������ݽṹ�嶨��*/
typedef struct
{
    u16 m_wStopReason ;				///<ֹͣԭ��
} TRecorderPreRecStopInfo;

/** ���ļ��л��¼������ݽṹ�嶨��*/
typedef struct
{
	u16 m_wSwitchResult;							///<���ļ��л��Ľ��
	s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];		///<���ļ��л�������ļ�·��
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];		///<���ļ��л�������ļ���
    u32 m_dwStartTime;							///<���ļ��л�������ļ��Ŀ�ʼʱ�䣬����ʱ��
}TBigRecFileSwitchInfo;

/** �����ַ�ṹ�嶨��*/
typedef  struct  tagTRPNetAddr 
{
	u32  m_dwIp;						///<IP��ַ
	u16  m_wPort;						///<�˿ں�
}TNetAddr;

/** ����ֹͣ�¼������ݽṹ�嶨��*/
typedef struct tagTPlayStopInfo
{
	u16	m_wReason ;					///<����ֹͣ��ԭ��
} TPlayStopInfo ;

/** RPCTRL���еĶ�����д����ʱͳ����Ϣ*/
typedef struct tagTRpStatics
{
	u32		m_dwFullLost;				///<RPCTRL���ڲ����ڻ��������µĶ�������
	u32		m_dwInLost;				///<���綪������
	u32		m_dwTaskDelay;			///<¼���̵߳������ʱͳ����Ϣ
	u32		m_dwIoDelay;				///<���ݰ�д���ļ�ʱ�������ʱͳ����Ϣ
	double  m_lfUdpRate;				///<���ݰ�������ͳ����Ϣ
}TRpStatics;

/**
 *@brief			�������ݰ����ջص��������壬�ṩ��DATASWITCHʹ�ã���DATASWITCH�������
 *@param[out]		u8* pPackBuf           ���յ����������ݰ�
 *@param[out]		u16 wPackLen           ���ݰ�����
 *@param[out]		KD_PTR ptDstAddr		���ݰ�������Ŀ�ĵ�ַ
 *@param[out]		KD_PTR ptSrcAddr		���ݰ�����Դ��ַ
 *@param[out]		u64 qwTimeStamp		���ݰ���ʱ���
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void* (*RPUdpPackRecvCallback)(u8* pPackBuf, u16 wPackLen, KD_PTR ptDstAddr, KD_PTR ptSrcAddr, u64 qwTimeStamp, KD_PTR pvContext);

/**
 *@brief			¼��״̬�ص���������
 *@param[out]		u32 dwRecorderID		¼���ID
 *@param[out]		u32 dwCBType			�ص����ͣ��μ�ERecorderCBType
 *@param[out]		void* pData			�ص����ݣ�����μ��ص����͵�ע��˵��
 *@param[out]		s32 nDataLen			�ص����ݵĴ�С
 *@param[out]		KD_PTR hUserHandle	�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecorderStateCallback)(u32 dwRecorderID, u32 dwCBType, void* pData, s32 nDataLen, KD_PTR hUserHandle);

/**
 *@brief			����״̬�ص���������
 *@param[out]		u32 dwPlayerId		�����ID
 *@param[out]		u32 dwCBType			�ص����ͣ��μ�EPlayerCBType
 *@param[out]		void* pData			�ص����ݣ�����μ��ص����͵�ע��˵��
 *@param[out]		s32 nDataLen			�ص����ݵĴ�С
 *@param[out]		KD_PTR hUserHandle	�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*PlayerStateCallback)(u32 dwPlayerId, u32 dwCBType, void *pData, s32 nDataLen, KD_PTR hUserHandle);

/** TSysGlobalParam�ṹ���и���������Ĭ��ֵ����*/
#define	DEFAULT_CONFIG_VIDEOFROMKEYFRAME					TRUE			///<��Ƶ�����Ƿ����ӹؼ�֡��ʼ
#define	DEFAULT_CONFIG_FRAMELOSTWAITKEYFRAME				TRUE			///<����Ƶ��֡���Ƿ�һ��Ҫ�ȵ���һ���ؼ�֡�ſ�ʼ����
#define	DEFAULT_CONFIG_RECTASKSWITCHTIME					40			///<¼�������Ĭ���л�ʱ�䣬��λ����
#define	DEFAULT_CONFIG_PLAYTASKSWITCHTIME					15			///<���������Ĭ���л�ʱ�䣬��λ����
#define	DEFAULT_CONFIG_COMMANDTIMEOUT						10000		///<RPCTRL���ڲ������Ĭ�ϳ�ʱʱ�䣬��λ����
#define	DEFAULT_CONFIG_VIDEOFRAMEDURATION					40			///<Ĭ�ϵ���Ƶ֡�������λ����
#define	DEFAULT_CONFIG_AUDIOFRAMEDURATION					30			///<Ĭ�ϵ���Ƶ֡�������λ����
#define	DEFAULT_CONFIG_BUFFTIME							2000			///<֡��������ʱ������λ����
#define	DEFAULT_CONFIG_BUFFSIZE							1024 * 1024	///<������Ļ���Ĭ�ϴ�С����λ�ֽ�
#define	DEFAULT_CONFIG_PLAY_BUFFTIME						100			///<����֡����ʱ�����֡����ʱ������λ����
#define	DEFAULT_CONFIG_PLAY_BUFFSIZE						64 * 1024	///<���Ű�����ʱ�����������С����λ�ֽ�
#define	DEFAULT_CONFIG_INSERTNULLFRAME						TRUE			///<��û��֡��������ʱ���Ƿ�����֡
#define	DEFAULT_CONFIG_INPUTFRAMEMAXDELAY					10000		///<Ĭ�ϵ��������֡��ʱ����������ʱ��֡��ʱ��������¼���
#define	DEFAULT_CONFIG_FRAMEWAITTIME						400			///<Ĭ�ϵȴ�֡��ʱ����������ʱ��������ܲ����֡
#define	DEFAULT_CONFIG_DURATIONCALCNUM						10			///<֡��ͳ���������ﵽ��֡���������һ��ʵ�ʵ�֡���
#define	DEFAULT_CONFIG_USEFIXFRAMERATE						TRUE			///<�Ƿ�ʹ�ù̶�֡�ʣ�TRUE�����ڲ�ʹ��DEFAULT_CONFIG_VIDEOFRAMEDURATION��Ϊ��Ƶ֡���
																	///<						   ʹ��DEFAULT_CONFIG_AUDIOFRAMEDURATION��Ϊ��Ƶ֡���
																	///<					FALSE������ڲ�ÿ��DEFAULT_CONFIG_DURATIONCALCNUM���֡ͳ��һ��ý����֡���
#define	DEFAULT_CONFIG_SWITCHFILEMINSPAN					1000			///<�ļ��л�����С�����Ŀǰδʹ��
#define	DEFAULT_CONFIG_DISK_OP_TIMEOUT						10000		///<���̲�����ʱʱ�䣬��λ����
#define	DEFAULT_TASK_NUM									0			///<Ĭ�ϵ������������ò���Ŀǰ�����޸�
#define	DEFAULT_TASK_NUM_PER_DIR							1			///<ÿ�������ϵ�����������Ŀǰ�ò�������RPCTRL�ڲ��ж���¼��/�����߳�

#define	RP_SYS_MAX_DIR_NUM								32			///<�����̷�������
#define	RP_SYS_MAX_DIR_NAME								256			///<����������󳤶�
#define	THREAD_NUM_DELFILE								25			///<�����̵߳�����

#define	DEFAULT_LOG_PATH									("/var/log/rplog")	///<RPCTRL��־��Ĭ�ϴ��·��
#define	DEFAULT_LOG_COUNT									(11)					///<RPCTRL��־Ĭ������
#define	DEFAULT_LOG_SIZE									(4*1024*1024)			///<RPCTRL������־�ļ��Ĵ�С
#define	DEFAULT_AUDIO_RECVBUFF								(65536)				///<MediaPack��Ĭ�ϵ���Ƶ���ջ����С��Ŀǰδʹ��
#define	DEFAULT_VIDEO_RECVBUFF								(262144)				///<MediaPack��Ĭ�ϵ���Ƶ���ջ����С

/** ¼������ϵͳ�������ýṹ��*/
typedef struct tagSysGlobalParam
{
    tagSysGlobalParam()
    {
        m_bVideoFromKeyFrame = DEFAULT_CONFIG_VIDEOFROMKEYFRAME;
        m_bFrameLostWaitKeyFrame = DEFAULT_CONFIG_FRAMELOSTWAITKEYFRAME;

        m_nClockRate = OspClkRateGet();

        m_nDefaultRecTaskSwitchTime = DEFAULT_CONFIG_RECTASKSWITCHTIME;
        m_nDefaultPlayTaskSwitchTime = DEFAULT_CONFIG_PLAYTASKSWITCHTIME;
        m_nDefaultCommandTimeout = DEFAULT_CONFIG_COMMANDTIMEOUT;

        m_nDefaultVideoFrameDuration = DEFAULT_CONFIG_VIDEOFRAMEDURATION;
        m_nDefaultAudioFrameDuration = DEFAULT_CONFIG_AUDIOFRAMEDURATION;
        m_nDefaultBuffTime = DEFAULT_CONFIG_BUFFTIME;
        
        m_nDefaultPlayBuffTime = DEFAULT_CONFIG_PLAY_BUFFTIME;

        m_nDefaultIsInsertNullFrame = DEFAULT_CONFIG_INSERTNULLFRAME;
        m_nDefaultInputFrameMaxDelay = DEFAULT_CONFIG_INPUTFRAMEMAXDELAY;
        m_nDefaultFrameWaitTime = DEFAULT_CONFIG_FRAMEWAITTIME;
        m_nDefaultFrameDurationCalcNum = DEFAULT_CONFIG_DURATIONCALCNUM;

        m_nDefaultContiueNullInsertNum = m_nDefaultInputFrameMaxDelay/m_nDefaultFrameWaitTime;

        m_bUseFixedFrameRate = DEFAULT_CONFIG_USEFIXFRAMERATE;

        m_nSwitchFileMinSpan = DEFAULT_CONFIG_SWITCHFILEMINSPAN;

		m_nDefaultDiskOpCommandTimeout = DEFAULT_CONFIG_DISK_OP_TIMEOUT ;
		m_nDefaultPlayBuffSize = DEFAULT_CONFIG_PLAY_BUFFSIZE ;
		m_nDefaultBuffSize = DEFAULT_CONFIG_BUFFSIZE ;

		m_nDefaultTaskNum = DEFAULT_TASK_NUM ;

		m_nDefaultTaskNumPerDir = DEFAULT_TASK_NUM_PER_DIR ;

		m_bDoStatis = TRUE;

		m_bMultiDelete = TRUE ;

		m_dwDelThreadNum = THREAD_NUM_DELFILE;

		strcpy(m_achLogPath, DEFAULT_LOG_PATH);

		m_byMaxLogCount = DEFAULT_LOG_COUNT;

		m_dwMaxSizePerLog = DEFAULT_LOG_SIZE;
		
		m_nDefaultAudioRecvBuff = DEFAULT_AUDIO_RECVBUFF;

		m_nDefaultVideoRecvBuff = DEFAULT_VIDEO_RECVBUFF;

		m_nDefaultMPPlayBufSize = MEDIAPACK_SEND_BUFF;

		m_nDefaultMPPlayBufKeyFrameNum = MEDIAPACK_SEND_BUFF_KEYFRAME_NUM;
		
    }


    s32 m_nClockRate;									///<ʱ��Ƶ�ʣ���OspClkRateGet()�ӿڵõ�

    s32 m_nDefaultRecTaskSwitchTime;					///<Ĭ�ϵ�¼�������л�ʱ��
    s32 m_nDefaultPlayTaskSwitchTime;					///<Ĭ�ϵķ��������л�ʱ��

    s32 m_nDefaultCommandTimeout;						///<RPCTRL���ڲ������Ĭ�ϳ�ʱʱ�䣬��λ����

	s32 m_nDefaultDiskOpCommandTimeout;					///<���̲�����ʱʱ�䣬��λ����

    BOOL32 m_bVideoFromKeyFrame;						///<��Ƶ�����Ƿ����ӹؼ�֡��ʼ
    BOOL32 m_bFrameLostWaitKeyFrame;					///<����Ƶ��֡���Ƿ�һ��Ҫ�ȵ���һ���ؼ�֡�ſ�ʼ����
    
    s32 m_nDefaultVideoFrameDuration;				///<Ĭ�ϵ���Ƶ֡�������λ����
    s32 m_nDefaultAudioFrameDuration;					///<Ĭ�ϵ���Ƶ֡�������λ����
    
    s32 m_nDefaultBuffTime;							///<֡��������ʱ������λ����
	
	s32	m_nDefaultPlayBuffSize ;						///<���Ű�����ʱ�����������С����λ�ֽ�

	s32 m_nDefaultBuffSize ;							///<������Ļ���Ĭ�ϴ�С����λ�ֽ�
    s32 m_nDefaultPlayBuffTime;						///<����֡����ʱ�����֡����ʱ������λ����

    s32 m_nDefaultIsInsertNullFrame;					///<��û��֡��������ʱ���Ƿ�����֡

    s32 m_nDefaultInputFrameMaxDelay;					///<Ĭ�ϵ��������֡��ʱ����������ʱ��֡��ʱ��������¼���

    s32 m_nDefaultFrameWaitTime;						///<Ĭ�ϵȴ�֡��ʱ����������ʱ��������ܲ����֡

    s32 m_nDefaultFrameDurationCalcNum;					///<֡��ͳ���������ﵽ��֡���������һ��ʵ�ʵ�֡���

    s32 m_nDefaultContiueNullInsertNum;					///<���������֡������

	BOOL32 m_bUseFixedFrameRate;						///<�Ƿ�ʹ�ù̶�֡�ʣ�TRUE�����ڲ�ʹ��m_nDefaultVideoFrameDuration��Ϊ��Ƶ֡���
													///<						   ʹ��m_nDefaultAudioFrameDuration��Ϊ��Ƶ֡���
													///<					FALSE������ڲ�ÿ��m_nDefaultFrameDurationCalcNum���֡ͳ��һ��ý����֡���

    s32 m_nSwitchFileMinSpan;							///<�ļ��л�����С�����Ŀǰδʹ��

	s32	m_nDefaultTaskNum ;							///<Ĭ�ϵ������������ò���Ŀǰ�����޸�

	s32 m_nDefaultTaskNumPerDir ;						///<ÿ�������ϵ�����������Ŀǰ�ò�������RPCTRL�ڲ��ж���¼��/�����߳�

	BOOL32	m_bDoStatis;								///<�Ƿ��������������д����ʱͳ��

	BOOL32	m_bMultiDelete;							///<�Ƿ���߳�ͬʱɾ���ļ�

	u32		m_dwDelThreadNum;							///<ɾ���ļ����߳�����

	s8	m_pchTaskDirName[RP_SYS_MAX_DIR_NUM][RP_SYS_MAX_DIR_NAME];		///<ÿ���̶߳�Ӧ�ķ�����
	s8 m_achDllPathName[MAX_FILE_PATH_LEN + 1];						///<���ض�̬��ľ���·��

	s8    m_achLogPath[MAX_FILE_PATH_LEN + 1];						///<RPCTRL�ڲ���־�Ĵ��·��
	u8    m_byMaxLogCount;										///<����������־����
	u32   m_dwMaxSizePerLog;										///<�����־�ļ����������С����λ�ֽ�
	s32   m_nDefaultAudioRecvBuff;									///<MediaPack�е���Ƶ���ջ����С��Ŀǰδʹ��
	s32   m_nDefaultVideoRecvBuff;									///<MediaPack��Ĭ�ϵ���Ƶ���ջ����С����λ�ֽ�
	s32   m_nMpAffCpuNo;                                              ///<MediaPack�׺�CPU��>
    s32   m_nDefaultMPPlayBufSize; 	// video send buffer of mediapack
    s32   m_nDefaultMPPlayBufKeyFrameNum;	// video send buffer of mediapack keyframenum	
} TSysGlobalParam;

/** ����ش���ַ�ṹ*/
typedef  struct  tagTNetBackAddr
{
	u32  m_dwIp;						///<IP��ַ
	u16  m_wRtcpPort;					///<rtcp�˿ں�
}TNetBackAddr;

/** ������յ�ַ��Ϣ�ṹ*/
typedef  struct  tagTRPNetRcvParam
{
	BOOL32 m_bNVR;								///<�Ƿ�NVR¼��
	u8    m_byNum;								///<�����ַ����
	TNetAddr  m_atAddr[MAX_STREAM_NUM];				///<�����ַ���飬��0����ʾ��Ƶ����n����ʾ��n·��Ƶ��Ŀǰֻ֧����·��Ƶ
	TNetBackAddr m_atBackAddr[MAX_STREAM_NUM];		///<�ش���ַ����
}TRPNetRcvParam;

/** ���緢�͵�ַ��Ϣ�ṹ*/
typedef  struct  tagTRPNetSndParam
{
	u8			m_byNum;							///<�����ַ����
	BOOL32		m_bUseRawSock ;					///<�Ƿ�ʹ�����׽���
	TNetAddr	m_atLocalAddr[MAX_STREAM_NUM ];			///<���ص�ַ���飬��0����ʾ��Ƶ����n����ʾ��n·��Ƶ��Ŀǰֻ֧����·��Ƶ
	TNetAddr	m_atRemoteAddr[MAX_STREAM_NUM ];		///<Զ�˵�ַ���飬��0����ʾ��Ƶ����n����ʾ��n·��Ƶ��Ŀǰֻ֧����·��Ƶ
	TNetSession m_atAddrForRawSock[MAX_STREAM_NUM];	///<���m_bUseRawSock=TRUE,���ֶ�Ϊsrc ip/port

	tagTRPNetSndParam()
	{
		m_bUseRawSock = FALSE ;
	}
}TRPNetSndParam;

/** �ļ��б�*/
typedef struct tagTListInfo
{
	u32 m_dwNum;											///<�ļ�����
	s8	m_astrFileName[MAX_FILE_NUMBER][MAX_FILE_NAME_LEN];	///<�ļ�������
}TListInfo;

/** �غ���Ϣ*/
typedef struct tagPTInfo
{
	u8 m_byRmtActivePT;				///<�յ��������е��غ�����
	u8 m_byRealPT;					///<�����е��غ����Ͷ�Ӧ��˾�ڲ�������غ�����
}TPTInfo;

/** �غ���Ϣ�ṹ*/
typedef struct tagRemotePTParam
{
	TPTInfo   m_atPTInfo[MAX_STREAM_NUM];		///<�غ�ֵ����
}TRemotePTParam;

/** �����غ���Ϣ�ṹ*/
typedef struct tagPTParam
{
	u8		  m_abyLocalPt[MAX_STREAM_NUM];		///<�����غ�����
}TLocalPTParam;

/** ���ܣ����ܣ���Ϣ*/
typedef struct tagEncDecInfo
{
	s8 *m_pszKeyBuf;					///<���ܣ����ܣ�key�ִ�
	u16 m_wKeySize;					///<���ܣ����ܣ�key�ִ���С
	u8 m_byMode;						///<���ܣ����ܣ�ģʽ
}TEncDecInfo;

/** ���ܣ����ܣ���Ϣ�ṹ*/
typedef struct tagTEncDecParam
{
	u8	m_byNum;								///<���ܣ����ܣ���Ϣ����
	TEncDecInfo m_atEncDecInfo[MAX_STREAM_NUM];	///<���ܣ����ܣ���Ϣ����
}TEncDecParam;

/** ¼����ʱ��ý��ͳ����Ϣ*/
typedef struct tagTMediaStatis
{
	u8  m_byMediaType;						///<ý������
	BOOL32 m_bAud;							///<�Ƿ�Ϊ��Ƶ
	u32 m_dwNetFrmNum;						///<���գ����ͣ���֡��
	u32 m_dwNetFrmRate;						///<���գ����ͣ�֡��
	u32 m_dwBufFullLostFrmNum;					///<���������µĶ�����
	u32 m_dwErrFrmLostFrmNum;					///<��֡���µĶ�����
	u32 m_dwBufEmptyNum;						///<����յĴ���
	u32 m_dwLastFrameId;						///<���һ֡�����
	BOOL32 m_bWaitForKeyFrame;					///<�Ƿ��ڵȴ��ؼ�֡
	u32 m_dwDuration;							///<����ʱΪý�����ĳ���ʱ�䣬¼��ʱΪ֡�������д��Ϊ40��
	u32 m_dwTimeStamp;						///<ʱ���
	u32 m_dwAsfFrmNum;						///<¼��ʱд���ļ�����֡��������ʱδ��
	u32 m_dwAsfFrmRate;						///<¼��ʱд���ļ���֡�ʣ�����ʱδ��
	u32 m_dwWidth;							///<��Ƶ���(��Ƶʱ,������ֶ���������ģʽ)
	u32 m_dwHeight;							///<��Ƶ�߶�
	BOOL32 m_bH263Plus;						///<�Ƿ�263+
	u32 m_dwAverageBitrate;					///<ƽ��������
	BOOL32 m_bPlayOut;						///<�Ƿ񲥷ţ�����ϲ㲻�������ַ������Ϊ��·���ǲ����ŵ�
}TMediaStatis;

/** ¼������ʼ�������ṹ*/
typedef struct tagRPInitParam
{
	u32 m_dwInitialRecorderNum;				///<��Ҫ��¼������·�������������������
	u32 m_dwInitialPlayerNum;					///<��Ҫ�ķ�������·�������������������
	u32 m_dwRemainSpaceAlarmLevelOne;			///<һ���澯����Сʣ��洢�ռ䣬Ŀǰδʹ��
	u32	m_dwRemainSpaceAlarmLevelTwo;			///<�����澯����Сʣ��洢�ռ䣬Ŀǰδʹ��
	FreeSpaceAlarmCallBack	m_pCallback;			///<ʣ��洢�ռ估�澯�ص�����
	KD_PTR m_pvContext;						///<�ص�����������
}TRPInitParam;

/** ���ؽ���״̬*/
enum EMFtpRcvStatus{
	FTP_RCV_STOP,								///<ֹͣ���ؽ���
	FTP_RCV_RUNNING							///<�������ؽ���
};

/** ¼��TCP��ʽ���ؽ��ղ���*/
typedef struct tagFtpRcvParam
{
	TNetAddr m_tFtpSndAddr;					///<���ط��ͷ���ַ
	TNetAddr m_tFtpListenAddr;					///<���ؽ��շ���ַ
	s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];	///<����¼���ļ���·��
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];	///<����¼���ļ����ļ���
}TFtpRcvParam;

/** ���ط���״̬*/
enum EMFtpSndStatus{
	FTP_SND_STOP,								///<ֹͣ���ط���
	FTP_SND_RUNNING							///<�������ط���
};

/** ¼��TCP��ʽ���ط��Ͳ���*/
typedef struct tagFtpSndParam
{
	TNetAddr m_tFtpSndAddr;						///<���ط��ͷ���ַ�������ӽ��յ�ַ
	TNetAddr m_tFtpRcvAddr;						///<���ؽ��շ���ַ
	u32 m_dwStartTime;							///<���ؿ�ʼʱ�䣬����ʱ��
	u32 m_dwStopTime;								///<���ؽ���ʱ��
    s8 m_achPlayFilePath[MAX_FILE_PATH_LEN+1];		///<�����ص��ļ���·��
    s8 m_achPlayFileName[MAX_FILE_NAME_LEN+1];		///<�������ļ����ļ���
	FtpFileRateCallBack m_pCallBack;				///<�����ļ����Ȼص�
	u8 m_byRateGran;								///<���ؽ��Ȼص�����(����Ϊ��λ,Ӧ����[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]��Χ)
	KD_PTR m_pvContext;							///<�ص�����������
}TFtpSndParam;

/** ¼��״̬*/
enum EMRecorderStatus{
	REC_STOP,									///<¼���Ѿ�ֹͣ
	REC_RUNNING,									///<����¼��
	REC_PAUSE,									///<¼������ͣ״̬����ǰδʹ��
    REC_STOPING,									///<¼������ֹͣ������
};	

/** RPCTRL����������ý����չ��Ϣ����*/
#define RP_MAXLEN_MEDIAINFOEXT    1024

/** ý����չ��Ϣ*/
typedef struct tagMediaInfoExt
{
	u16                 m_wMediaInfoDataLen;							///<ý����չ��Ϣʵ�ʳ���
	u8                  m_abyMediaInfoData[RP_MAXLEN_MEDIAINFOEXT];		///<ý����չ��Ϣ
}TMediaInfoExt;

/** ý��������*/
enum ERPStreamType
{
	E_StreamType_Unknown,							///<δ֪��ý��������
	E_StreamType_Audio,							///<��Ƶ��
	E_StreamType_Video,							///<��Ƶ��
};

/** ý��������Ϣ*/
typedef struct tagRPStreamProperty
{
	ERPStreamType m_eStreamType;								///<ý��������
	u8            m_byPayloadType;							///<�غ�����
	s8            m_szEncodingName[KDS_MAXLEN_ENCODINGNAME+1];	///<��������

	union
	{
		TAudioStreamProperty  m_tAudioStreamProperty;			///<��Ƶý��������
		TVideoStreamProperty  m_tVideoStreamProperty;			///<	��Ƶý��������
	};
}TRPStreamProperty;

/** ý����������Ϣ*/
typedef struct
{
	u64                   m_qwStreamStartTime;					///<ý�����Ŀ�ʼʱ�䣬��λ����
	u8                    m_byAudioStreamNum;					///<��Ƶ������
	u8                    m_byVideoStreamNum;					///<��Ƶ������
	TRPStreamProperty     m_atStreamProperty[MAX_STREAM_NUM];		///<ý����������Ϣ
}TMediaStreamDescription;

/** �ļ���Ϣ*/
typedef struct tagTRPFileInfo
{
	u64		m_qwTotalFileSize;								///<¼���ļ���С����λ�ֽ�
	u32		m_dwFileStartTime;								///<¼���ļ���ʼʱ�䣬��λ��
	u32		m_dwFileDuration;									///<¼���ļ����ŵ���ʱ�䳤�ȣ���λ��
	TMediaStreamDescription  m_tMediaStreamDesc;					///<¼���ļ���ý����������Ϣ
	TMediaInfoExt           m_tMediaInfoExt;					///<¼���ļ���ý����չ��Ϣ
}TRPFileInfo;

/** �ļ���ý����Ϣ*/
typedef struct tagTRPMediaInfo
{
	TMediaStreamDescription  m_tMediaStreamDesc;					///<¼���ļ���ý����������Ϣ
	TMediaInfoExt           m_tMediaInfoExt;					///<¼���ļ���ý����չ��Ϣ
}TRPMediaInfo;

/** ¼������ṹ*/
typedef struct tagTRecParam
{
	BOOL32 m_bLocalRec;								///<�Ƿ�Ϊ����¼�񣬱���¼��Ҫ��codecwrapper�е���
    s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];			///<����¼���ļ���·�������Ϊ��WINDOWSϵͳ��������/mnt/��ͷ
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];			///<����¼���ļ����ļ���
	u32 m_dwStartTime;								///<¼���ļ�����ʼʱ��
	BOOL32 m_bVideoFirst;								///<¼���ļ��Ƿ�����Ƶ�ؼ�֡��ʼ��
	BOOL32 m_bSmallFile;								///<�Ƿ�ΪС�ļ���ʽ¼��¼�����Ϊ��WINDOWSϵͳ���Ҵ洢�ļ�ϵͳΪͨ���ļ�ϵͳ����ò�����TRUE
	u32 m_dwTimeInterval;								///<С�ļ���ʱ��������λ�룬m_bSmallFile=TRUEʱ��Ч
	RecFileEndCallBack m_pRecEndCallBack;				///<С�ļ������ص�������m_bSmallFile=TRUEʱ��Ҫ����
	RecBigFileSwCallBack m_pRecBigFileSwCB;				///<���ļ��л��ص�����
	KD_PTR m_pvContext;								///<�ص�����������
	
	TKdsStreamProperty  m_atStreamProperty[MAX_STREAM_NUM];	///<ý����������Ϣ
	TMediaInfoExt       m_tMediaInfoExt;					///<ý����չ��Ϣ
}TRecParam;

/** ¼�����Ƶ������*/
typedef struct tagStreamAverageBitrate
{
	u32 m_adwStreamAverageBitrate[MAX_STREAM_NUM];		///<����Ӧ��ƽ�����ʣ�����0Ϊ��Ƶ��1��Ϊ��Ƶ��2Ϊ��Ƶ
}TStreamAverageBitrate;

/** ¼�������״̬*/
typedef struct tagTRPRecStatus
{
	BOOL32 m_bPreRecStart;								///<Ԥ¼�Ƿ�ʼ
	EMRecorderStatus m_tRecState;						///<¼��״̬
}TRPRecStatus;

/** ¼���״̬��Ϣ*/
typedef struct tagTRecorderStatus
{
	TRPRecStatus  m_tRecStatus;						///<¼��״̬
	TRecParam  m_tRecParam;							///<¼�����
	TRPNetRcvParam	m_tNetparam;						///<������ղ���
	BOOL32 m_bOnlyRecIFrm;								///<�Ƿ�ֻ¼�ؼ�֡
	u32	m_dwFrmItvl;									///<¼�ؼ�֡�ļ��
}TRecorderStatus;

/** ¼���ͳ��*/
typedef struct tagTRecorderStatis
{
	u8	m_byNum;										///<ý��������
	u32 m_dwRecTime;									///<¼��ʱ������λ��
	TMediaStatis m_atMediaRecStatis[MAX_STREAM_NUM];		///<¼��ý��ͳ����Ϣ
}TRecorderStatis;

/** ����״̬*/
enum EmPlayerStatus
{
	PLAYER_PLAY_STOP,						///<����ֹͣ
	PLAYER_PLAY_START,					///<����ʼ
	PLAYER_PLAY_PAUSE,					///<������ͣ
	PLAYER_PLAY_QUICK,					///<���
	PLAYER_PLAY_SLOW,						///<����
    PLAYER_PLAY_STOPING,					///<��������ֹͣ
};

/** ����Ч������*/
enum EmPlayerEffect
{
	PLAYER_EFFECT_ONLY_KEY_FRAME = 1 ,		///<����ʱֻ���͹ؼ�֡

	PLAYER_EFFECT_ONLY_AUDIO = 1 << 2,		///<����ʱֻ������Ƶ

	PLAYER_EFFECT_ONLY_VIDEO = 1 << 3,		///<����ʱֻ������Ƶ

	PLAYER_EFFECT_SINGE_FRAME = 1 << 4,		///<��֡���ţ�δʹ��

	PLAYER_EFFECT_MAX_VALUE ,				///<��Чֵ

} ;

/** ��������ṹ*/
typedef struct tagTPlayParam	
{
	BOOL32 m_bNVR ;								///<�Ƿ񱾵ط���
    s8 m_achPlayFilePath[MAX_FILE_PATH_LEN+1];		///<��������ļ�����·������WINDOWSϵͳ��������/mnt/��ͷ
    s8 m_achPlayFileName[MAX_FILE_NAME_LEN+1];		///<��������ļ����ļ���
	u32 m_dwStartTime;							///<����ʼʱ�䣬����ʱ�䣬����Ϊ0
	u32 m_dwStopTime;								///<�������ʱ�䣬����ʱ�䣬0��ʾ���ŵ��ļ�β
	BOOL32 m_bSmallFile;							///<���ŵ��ļ��Ƿ�ΪС�ļ�������ͨ���ļ�ϵͳ��m_achPlayFilePath+m_achPlayFileNameΪ·������·���������reclog.txt����ò�������ΪTRUE
	ReadFileRateCallBack m_pCallBack;				///<�ļ�������Ȼص�
	u8 m_byRateGran;								///<������Ȼص�����(����Ϊ��λ,Ӧ����[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]��Χ)
	KD_PTR m_pvContext;							///<�ص�������
	TRPNetSndParam	m_tNetSndParam;				///<����ʱ�����緢�͵�ַ
	BOOL32 m_bPlyOnlyIFrm;							///<�Ƿ�ֻ���͹ؼ�֡
	BOOL32 m_bReversePlay;							///<�Ƿ񵹷�
	u32    m_dwPlaySSRC;							///<����ʱ�������е�SSRC��0:��ʾ��ָ�����ڲ��Լ����ɣ���0:�ϲ�ҵ��ָ��SSRC����������ʱ����ʹ�ø�SSRC
}TPlayParam;

/** �����״̬*/
typedef struct tagTPlayerStatus
{
	EmPlayerStatus	m_tPlyStatus;					///<����״̬
	u32             m_dwPlyRate;                    ///<��������
	u32				m_dwPlyEffect ;				///<����Ч���������μ�EmPlayerEffect(PLAYER_EFFECT_ONLY_KEY_FRAME...)
	TPlayParam		m_tPlayParam;					///<�������
}TPlayerStatus;

/** �����ͳ��*/
typedef struct tagTPlayerStatis
{
	u8	m_byNum;										///<ý��������
	u32 m_dwPlyTime;									///<����ʱ������λ��
	u32 m_dwTotalPlyTime;								///<�ļ���ʱ������λ��
	TMediaStatis m_atMediaPlyStatis[MAX_STREAM_NUM];		///<����ʱý����ͳ����Ϣ
	u8  m_abyStreamNoArr[MAX_STREAM_NUM];				///<��������飬��Ϊһ���ļ�����2��������ſ���Ϊ2��3, ����1��2��δʹ��
}TPlayerStatis;

/** ¼��ʱ�ص������Ľṹ��*/
typedef struct tagTCBContext 
{
	u8 m_byIdx;									///<¼��ý������ţ�0��Ƶ��������Ƶ
	u64 m_llAddr;									///<¼��������ַ
}TCBContext; 

/** ¼���ش�����*/
typedef struct tagTRPRecRSParam
{
	TRSParam m_atRsParam[MAX_STREAM_NUM];			///<�ش�������0Ϊ��Ƶ��������Ƶ
	BOOL32	 m_abRepeatSnd[MAX_STREAM_NUM];			///<�Ƿ�֧���ش�
}TRPRecRSParam;

/** �����ش�����*/
typedef struct tagTRPPlyRSParam
{
	u16		 m_awBufTimeSpan[MAX_STREAM_NUM];		///<�ش������0Ϊ��Ƶ��������Ƶ
	BOOL32	 m_abRepeatSnd[MAX_STREAM_NUM];			///<�Ƿ�֧���ش�
}TRPPlyRSParam;


/** UDP���ط�ͬʱ��������*/
#define			MAX_RP_FTP_RCV_NUM			32

/** UDP���ط��ͷ������ķ�����*/
#define			MAX_RP_FTP_SND_NUM			32

/** ���ؽ��շ����û�ID����*/
#define			MAX_FTP_RCV_USER_ID_LEN		32

/** ������Ϣ�汾��*/
#define	        RP_VER_DOWNLOAD				3		///<�ð汾��2��ʼ֧��ý����Ϣ
													///<�ð汾��3��ʼ֧�ִ��ACK

/** UDP���ط��Ͷ�����״̬*/
typedef enum
{
		SND_STATUS_START = 1,							///<���ؿ�ʼ
		SND_STATUS_STATUS_PROBE,						///<�յ�̽��������Ѿ���Ӧ
		SND_STATUS_WAITING_RCV_REQ,					///<�ȴ����ն�����δʹ��
		SND_STATUS_WAITED_RCV_REQ,						///<�յ����ն˵�����
		SND_STATUS_SEND_STREAM_PROPERTY,				///<�ȴ�����������
		SND_STATUS_WAIT_PROPERTY_ACK,					///<�ȴ����ն˶������ԵĻ�Ӧ��Ϣ
		SND_STATUS_SEND_FRAME_DATA,					///<�ȴ�����֡����
		SND_STATUS_WAIT_FRAME_DATA_ACK,					///<�ȴ����ն˶�֡���ݵĻ�Ӧ
		SND_STATUS_STOP								///<����ֹͣ
}TSndStatus;

/** UDP���ؽ��ն�״̬,�ص����(!!!��0)�Ľ��д���*/
typedef enum
{
		RCV_STATUS_START = 1,							///<1��ʼ���أ�δʹ��
		RCV_STATUS_ADD_STREAM_ERROR,					///<2д������δʹ��
		RCV_STASTU_ADD_FRAME_ERROR,					///<3д֡����
		RCV_STATUS_SND_REQ,							///<4������������δʹ��
		RCV_STATUS_WAIT_STREAM_PROPERTY,				///<5�ȴ�������
		RCV_STATUS_SND_PROPERTY_ACK,					///<6���������Ի���
		RCV_STATUS_WAIT_FRAME_DATA,					///<7�ȴ�֡����
		RCV_STATUS_SND_FRAME_DATA_ACK,					///<8����֡������δʹ��
		RCV_STATUS_TIMEOUT,							///<!!!9��������ʱ
		RCV_STATUS_CLOSE,								///<10�رգ�δʹ��
		Rcv_STATUS_RCV_END,							///<!!!11���ؽ���
		RCV_STATUS_REQ_INVALID,						///<!!!12����������Ч��������ز���(���ͷ���Ϊ��������������ܲ��Ϸ�)��RpFtpRcvCallBack�ص������ĵ���������Ϊ����ԭ�򣬵��ĸ�����������
		RCV_STATUS_PROBE,								///<�Ѿ�����̽���
		RCV_DATA_TYPE									///<�����������ͣ�RpFtpRcvCallBack�ص����������ĵ���������ΪTRcvDataType���͵�ֵ�����ĸ�������Ч
}TRcvStatus;

/** ���ؽ��ն˵���������*/
typedef enum
{
	RCV_DATA_KEDA_ASF = 1,								///<���ص��ļ�ΪASF����MP4�ļ��������͵����أ����������������ݰ��ص����ϲ㣬��RPCTRL�ڲ�д�ļ� 
	RCV_DATA_KEDA_KDS,								///<���ص��ļ�ΪKds�ļ�����ΪKEDA���������ݰ��ص����ϲ�ҵ��󣬿���ʹ��Udp2asf��д��asf����mp4�ļ�
	RCV_DATA_OTHER_KDS,								///<���ص��ļ�Ϊkds�ļ�����Ϊ�⳧�����������ݰ��ص����ϲ�ҵ��󣬲���ʹ��udp2asf
	RCV_DATA_KEDA_PS,									///<δʹ��
	RCV_DATA_THIRDCOMPANY,								///<2011.07.14 add by llf ��ʾ���ص�Ϊ���������̵ķǱ�׼�������ݣ����ݰ��ص����ϲ�ҵ����
	RCV_DATA_THIRDCOMPANY_STANDARD						///<2011.11.21 add by llf ��ʾ���ص�Ϊ���������̵ı�׼�������ݣ����ݰ����ص����ϲ�ҵ����RPCTRLֱ��д�ļ�
} TRcvDataType;

/** �ļ�ϵͳ����*/
typedef enum
{
	FS_TYPE_EXT3 = 0,									///<ͨ���ļ�ϵͳ
	FS_TYPE_VBFS,										///<vbfs�ļ�ϵͳ
	FS_TYPE_CLOUD,									///<�ƴ洢
	FS_TYPE_UNKNOWN									///<δ֪����
} TFsType;

/**
 *@brief			����״̬�ص���������
 *@param[out]		u32 dwStatusCode		״̬�룬�μ�TRcvStatus
 *@param[out]		u32 dwSSid			���������Ψһ��ʶ����ʹ��RpFtpRcvCreate()�ӿڷ��ص�ssidֵ
 *@param[out]		u32 dwCurTime			��ǰ���صĽ��ȣ�����ʱ�䣬��ĳЩ�����TRcvStatusֵ���������ĺ��壬�����TRcvStatus��˵��
 *@param[out]		u32 dwCurSpeed		���ص��ٶȣ���ĳЩ�����TRcvStatusֵ���������壬�����TRcvStatus��˵��
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpRcvCallBack)(u32 dwStatusCode, u32 dwSSid, u32 dwCurTime, u32 dwCurSpeed, KD_PTR pvContext);

/**
 *@brief			�������ݰ��ص���������
 *@param[out]		u32 dwSSid			���������Ψһ��ʶ����ʹ��RpFtpRcvCreate()�ӿڷ��ص�ssidֵ
 *@param[out]		u8 * pbData            �ص����������ݰ�
 *@param[out]		u32 dwDataSize		���ݰ���С
 *@param[out]		u64 qwTimeStamp		���ݰ���Ӧ��ʱ��������ʱ�������0��ʼ����λ����
 *@param[out]		u8 byStreamId			���ݰ���Ӧ��ý������ţ�0Ϊ��Ƶ������Ϊ��Ƶ
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpDataCallback)(u32 dwSSid, u8 * pbData, u32 dwDataSize, u64 qwTimeStamp, u8 byStreamId, KD_PTR pvContext);

/** ���ط���˵ķ��ͻ����С*/
#define DEFAULT_RP_DOWNLOAD_SERVER_BUFF		(128 * 1024)

/** ���ؿͻ��˵Ľ��ջ����С*/
#define DEFAULT_RP_DOWNLOAD_CLIENT_BUFF		(128 * 1024)

/** ���ͷ���˲����ṹ*/
typedef struct tagFtpSndServParam
{
	tagFtpSndServParam()
	{
		m_tLocalAddr.m_dwIp = 0;
		m_tLocalAddr.m_wPort = 1723;
		m_dwSndBuf = DEFAULT_RP_DOWNLOAD_SERVER_BUFF;
		m_nSpeedLimit = -1;
	}

	TNetAddr		m_tLocalAddr;				///<���ͷ���˵ı��ص�ַ
	s32			m_dwSndBuf;				///<���ͻ����С����λ�ֽڣ�ȡֵ��Χ[8KB, 2000KB], 0���ʾʹ��Ĭ��ֵDEFAULT_RP_DOWNLOAD_SERVER_BUFF
	s32			m_nSpeedLimit;			///<�����ٶ����ƣ���λbps��ȡֵ���ڵ���20KB/s, С�ڵ���0��ʾ������
}TFtpSndServParam;

/** ���ط��Ͳ���*/
typedef struct tagFtpUdpSndParam
{
	u32 m_dwStartTime;					///<��ʼʱ�䣬����ʱ��
	u32 m_dwStopTime;						///<����ʱ�䣬����ʱ�䣬0��ʾ���ص��ļ�����������ȡֵ������ڿ�ʼʱ��
	s8 m_abyFilePath[MAX_FILE_PATH_LEN];	///<�ļ�·������WINDOWS����ϵͳ�µ�ͨ���ļ�ϵͳ������LINUX�µ�EXT3������������/mnt/��ͷ
	s8 m_abyFileName[MAX_FILE_NAME_LEN];	///<�ļ���
	BOOL32 m_bSmallFile;					///<�Ƿ�С�ļ�
	u16	m_wDownloadType ;					///<���ص��ļ����ͣ�����RP_NORMAL_DOWN��
}TFtpUdpSndParam;

/** ���ؿͻ��˵Ĳ���*/
typedef struct tagFtpSndAllParam
{
	u32				m_dwSSid;							///<���ؿͻ��˵�ssid
	TSndStatus		m_emSndStatus;						///<���ط��Ͷ�״̬
	TNetAddr			m_tRemoteAddr;						///<���ؿͻ���ip/port
	s8				m_abyUserId[MAX_FTP_RCV_USER_ID_LEN+1];	///<���ؽ��ն˵��û����
}TFtpSndAllParam;

/** ����ý����Ϣ*/
typedef struct tagDLMediaInfo
{
public:
	tagDLMediaInfo()
	{
		memset(this, 0, sizeof(tagDLMediaInfo));
	}

	u32 GetVersion()
	{
		return ntohl(m_dwVersion);
	}

	void SetVersion(const u32 dwVersion)
	{
		m_dwVersion = htonl(dwVersion);
	}

	u8  GetStreamID()
	{
		return m_byStreamID;
	}

	void SetStreamID(const u8 byStreamID)
	{
		m_byStreamID = byStreamID;
	}

	u8 GetPayLoad()
	{
		return m_byPayLoad;
	}

	void SetPayLoad(const u8 byPayLoad)
	{
		m_byPayLoad = byPayLoad;
	}

	s32 GetWidth()
	{
		return ntohl(m_tVideoInfo.m_nWidth);
	}

	void SetWidth(s32 nWidth)
	{
		m_tVideoInfo.m_nWidth = htonl(nWidth);
	}

	s32 GetHeigth()
	{
		return ntohl(m_tVideoInfo.m_nHeight);
	}

	void SetHeigth(s32 nHeigth)
	{
		m_tVideoInfo.m_nHeight = htonl(nHeigth);
	}

	s32 GetVideoClockRate()
	{
		return ntohl(m_tVideoInfo.m_nClockRate);
	}

	void SetVideoClockRate(s32 nClockRate)
	{
		m_tVideoInfo.m_nClockRate = htonl(nClockRate);
	}

	s32 GetBitRate()
	{
		return ntohl(m_tVideoInfo.m_nBitRate);
	}

	void SetBitRate(s32 nBitRate)
	{
		m_tVideoInfo.m_nBitRate = htonl(nBitRate);
	}

	s32 GetFrameRate()
	{
		return ntohl(m_tVideoInfo.m_nFrameRate);
	}

	void SetFrameRate(s32 nFrameRate)
	{
		m_tVideoInfo.m_nFrameRate = htonl(nFrameRate);
	}

	s32 GetKeyFrameIntvl()
	{
		return ntohl(m_tVideoInfo.m_nKeyFrameIntvl);
	}

	void SetKeyFrameIntvl(s32 nKeyFrameIntvl)
	{
		m_tVideoInfo.m_nKeyFrameIntvl = htonl(nKeyFrameIntvl);
	}

	s32 GetAudioClockRate()
	{
		return ntohl(m_tAudioInfo.m_nClockRate);
	}

	void SetAudioClockRate(s32 nClockRate)
	{
		m_tAudioInfo.m_nClockRate = htonl(nClockRate);
	}

	s32 GetSampleRate()
	{
		return ntohl(m_tAudioInfo.m_nSampleRate);
	}

	void SetSampleRate(s32 nSampleRate)
	{
		m_tAudioInfo.m_nSampleRate = htonl(nSampleRate);
	}

	s32 GetChannelNum()
	{
		return ntohl(m_tAudioInfo.m_nChannelsNum);
	}

	void SetChannelNum(s32 nChannelNum)
	{
		m_tAudioInfo.m_nChannelsNum = htonl(nChannelNum);
	}

private:
	u32     m_dwVersion;								///<���صİ汾��
	u8      m_byStreamID;								///<ý�������
	u8      m_byPayLoad;								///<ý����غ�����
	u16     m_wReserved;								///<�����ֶ�

	union
	{
		TVideoStreamProperty   m_tVideoInfo;			///<��Ƶý��������Ϣ
		TAudioStreamProperty   m_tAudioInfo;			///<��Ƶý��������Ϣ
	};

}TRPDLMediaInfo;

/** ����ý�������Լ�*/
typedef struct tagRPDLMediaInfoSet
{
	u8              m_byStreamNum;				///<ý��������
	TRPDLMediaInfo  m_atMediaInfo[MAX_STREAM_NUM];	///<ý������Ϣ
}TRPDLMediaInfoSet;

/**
 *@brief			ý����Ϣ�ص���������
 *@param[out]		u32 dwSSid			���������Ψһ��ʶ����ʹ��RpFtpRcvCreate()�ӿڷ��ص�ssidֵ
 *@param[out]		TRPDLMediaInfoSet *ptMediaInfoSet	ý����Ϣ��
 *@param[out]		KD_PTR pvContext		�ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpMediaInfoCallBack)(u32 dwSSid, TRPDLMediaInfoSet *ptMediaInfoSet, KD_PTR pvContext);

/** PROXY���ݵ���󳤶�*/
#define		MAX_PROXY_USER_DATA		16

typedef enum
{
	ERP_FileType_Unknown = 0,
	ERP_FileType_Asf,
	ERP_FileType_Mp4,
}ERPFileType;

/** ftp�ͻ��˽��ղ����ṹ*/
typedef struct tagFtpUdpRcvParam
{
	s8	m_abyUserId[MAX_FTP_RCV_USER_ID_LEN+1];				///<���ؿͻ����û���ţ����Բ���
	TNetAddr m_tRcvLocalAddr;								///<���ؿͻ��˵ı��ص�ַ
	TNetAddr m_tRcvRemoteAddr;								///<���ط���˵ĵ�ַ
	RpFtpRcvCallBack m_pRcvCallBack;						///<����״̬�ص�����
	KD_PTR m_pvContext;									///<�ص�������
	s8 m_abyLocalFilePath[MAX_FILE_PATH_LEN];				///<���ؿͻ��˱����ļ��洢·��
	s8 m_abyLocalFileName[MAX_FILE_NAME_LEN];				///<���ؿͻ��˱��ش洢�ļ���
	ERPFileType  m_eLocalFileType;							///<���ص����ص��ļ�����ĸ�ʽ����������ã���ΪERP_FileType_Unknown��
														///<��ͨ��m_abyLocalFileName�����еĺ�׺ȷ���ļ���ʽ
	u32 m_dwGran;											///<���ؽ��ȵĻص����ȣ���λ��
	u32 m_dwStartTime;									///<���ؿ�ʼʱ�䣬����ʱ��
	u32 m_dwStopTime;										///<���ؽ���ʱ�䣬����ʱ�䣬0��ʾ���ص��ļ�β����0ʱ������������ؿ�ʼʱ��
	s8 m_abyRequestFilePath[MAX_FILE_PATH_LEN];				///<�������صķ��������ļ�����·��,Linuxϵͳ�µ�ͨ���ļ�ϵͳ��������/mnt/��ͷ
	s8 m_abyRequestFileName[MAX_FILE_NAME_LEN];				///<�������صķ��������ļ����ļ���
	BOOL32 m_bSmallFile;									///<�Ƿ�С�ļ������m_abyRequestFilePathĿ¼�������reclog.txt�ļ���������ΪTRUE
	u8	m_abyProxyUserData[MAX_PROXY_USER_DATA];				///<�����û�����
	u32 m_dwProxyUserDataLen;								///<�����û����ݳ���

	RpFtpDataCallback m_pfCallback ;						///<�������ݻص�
	RpFtpMediaInfoCallBack  m_pfMediaInfoCallBack;			///<����ý����Ϣ�ص�
	s32			m_dwRcvBuf;								///<���ؽ��ն˻����С����λ�ֽڣ�ȡֵ��Χ[8KB,2000KB],0��ʾʹ��Ĭ��ֵDEFAULT_RP_DOWNLOAD_CLIENT_BUFF
	s32			m_nSpeedLimit;							///<���ն��ٶ����ƣ�Ŀǰ��Ч
	u8          m_byDownLoadType;							///<���ص��ļ����ͣ�����RP_NORMAL_DOWN�ȣ����Բ���
	tagFtpUdpRcvParam()
	{
		m_dwProxyUserDataLen = 0;
		m_dwRcvBuf = DEFAULT_RP_DOWNLOAD_CLIENT_BUFF;
		m_nSpeedLimit = -1;
		m_pRcvCallBack = NULL;
		m_pfMediaInfoCallBack = NULL;
		m_pfCallback = NULL;
		m_eLocalFileType = ERP_FileType_Unknown;
	}
}TFtpUdpRcvParam;

/** ftp���ͷ���˲�ѯ�ṹ*/
typedef struct tagFtpSndQuery
{
	u32 dwClientNum;										///<�ͻ�����,Ҳ����������Ԫ�ظ���
	TFtpSndAllParam m_atFtpSndAllParam[MAX_RP_FTP_SND_NUM];	///<�ͻ����������ص���Ӧ����
	TFtpUdpSndParam m_tFtpUdpSndParam[MAX_RP_FTP_SND_NUM];	///<�ͻ����������ص���Ӧ����
}TFtpSndQuery;

/** ftp�ͻ��˲�ѯ�ṹ*/
typedef struct tagFtpRcvQuery
{
	u32 dwStauts;											///<���ؽ��ն�Ŀǰ״̬
}TFtpRcvQuery;

/** ������Ϣ*/
typedef struct tagRPDiskInfo
{
	u8 m_byFsType;							///<�ļ�ϵͳ����
	u64 m_llPhysicsSize;						///<���̷�����С����λMB
	u64 m_llUsedSize;							///<��ʹ�ô��̷�����С����λMB
	u64 m_llFreeSize;							///<���ô��̷�����С����λMB
	u64 m_llLockSize;							///<���������ռ��С����λMB
}TRPDiskInfo;

/**
 *@brief			¼������ʼ������
 *@param[in]		const TRPInitParam *ptRPInitParam	��ʼ������
 *@param[in]		TSysGlobalParam	*ptRPSysParam		ϵͳȫ�ֲ���������ΪNULLʱ����ʹ��Ĭ��ֵ
 *@param[in]		BOOL32 bUsedRawSocket				�Ƿ�ʹ�����׽���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ò���������ʹ��RPCTRL���������������ǰ������	
 */
u16 RPInit( const TRPInitParam *ptRPInitParam, TSysGlobalParam	*ptRPSysParam = NULL, BOOL32 bUsedRawSocket = FALSE);

/**
 *@brief			¼����ⷴ��ʼ������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿڱ����ú󣬽�������ʹ��RPCTRL��������κι���	
 */
u16 RPRelease( );

/**
 *@brief			�����Ҫ���Ĵ��̷�����Ϣ
 *@param[in]		s8 *pchDiskMountName	���̷����Ĺ��ص㣬����ext3������/mnt/UUID��vbfs������/vbfs/UUID,
 *@param[in]		u32 dwAlarmLvlOne		���̷���ʣ��ռ��һ���澯��ֵ
 *@param[in]		u32 dwAlarmLvlTwo		���̷���ʣ��ռ�Ķ����澯��ֵ
 *@param[in]		const s8* pchDiskName	���̷�����Ӧ���̷���������/dev/sda1
 *@param[in]		BOOL32 bMount			���̷����Ƿ�Ϊ���ط��������linuxϵͳ��ͨ���ļ�ϵͳ����
 *@param[in]		BOOL32 bCheckDisk       �Ƿ�����̷���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPSetCheckPartion( s8 *pchDiskMountName , u32 dwAlarmLvlOne, u32 dwAlarmLvlTwo, const s8* pchDiskName = NULL, BOOL32 bMount = FALSE, BOOL32 bCheckDisk = TRUE);

/**
 *@brief			���ô��̼����
 *@param[in]		u32 dwInterval	���̷������������λ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�����ʹ�øýӿ����ü�������ڲ�Ĭ��5s���һ��	
 */
u16 RPSetDiskCheckInterval(  u32 dwInterval );

/**
 *@brief			���ô��̶�����ʱʱ�䣬������ʱ���������Ӧ�ϱ����̶���
 *@param[in]		s8 * pchPartitionName	���̷���������RPSetCheckPartion�ӿڴ���ĵ�һ����������һ��
 *@param[in]		u32 dwSec			���̼�ⳬʱʱ�䣬��λ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�����ʹ�øýӿ����ü�ⳬʱʱ�䣬�ڲ�Ĭ��15s��ʱ	
 */
u16 RPSetDiskCheckTimeOut(s8 * pchPartitionName, u32 dwSec);

/**
 *@brief			������״̬��ȷ�ϴ����Ƿ�����
 *@param[in]		s8 * pchPartitionName	���̷���������RPSetCheckPartion�ӿڴ���ĵ�һ����������һ��
 *@param[in]		u32 dwWaitMillSec		�ȴ����ʱ�䣬��λ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPCheckDiskState(s8 * pchPartitionName, u32 dwWaitMillSec) ;

/**
 *@brief			ɾ������Ҫ�ټ��Ĵ��̷�������������Ѿ�����
 *@param[in]		s8 * pchPartitionName	���̷���������RPSetCheckPartion�ӿڴ���ĵ�һ����������һ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16  RPDeleteCheckPartion( s8 *pchPartionName );
	
/**
 *@brief			����¼������õ����õ�¼������
 *@param[out]		u32 *pdwRecoderID		���õ�¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPCreateRec( u32 *pdwRecoderID );

/**
 *@brief			�ͷ�¼���
 *@param[in]		u32 dwRecoderID		��Ҫ�ͷŵ�¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPReleaseRec( u32 dwRecoderID );

/**
 *@brief			����¼�����������ղ���
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const TRPNetRcvParam *ptNetRcvParam		������ղ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPSetRecNetRcvParam( u32 dwRecorderID, const TRPNetRcvParam *ptNetRcvParam );

/**
 *@brief			��������ƽ������
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const TStreamAverageBitrate& tStreamAverageBitrate	ƽ������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ŀǰ��Ч	
 */
u16 RPSetAverageBitrate(u32 dwRecorderID, const TStreamAverageBitrate& tStreamAverageBitrate);

/**
 *@brief			���ý����������غ���Ϣ
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const TRemotePTParam *ptPTParam		�������غ�ֵ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�ֻ��ʹ��kdvmedianet���������ʱ��Ч	
 */
u16 RPSetRemotePT( u32 dwRecoderID, const TRemotePTParam *ptPTParam );

/**
 *@brief			���ý��ܲ���
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const TEncDecParam *ptDecryptParam	���ܲ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�ֻ��ʹ��kdvmedianet���������ʱ��Ч	
 */
u16 RPSetDecryptKey( u32 dwRecorderID, const TEncDecParam *ptDecryptParam );

/**
 *@brief			����¼������
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		u8 RecordType			¼�����ͣ�����RP_NORMAL_RECORD��RP_UNKNOWN_KEDA_RECORD��
 *@param[in]		const s8* pbyCompanyName	������Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPSetRecordType(u32 dwRecorderID , u8 RecordType, const s8* pbyCompanyName = NULL/*add by jh*/);

/**
 *@brief			ʹ�ø���¼���ID��ʼ¼��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const TRecParam *ptRecParam	¼�����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPStartRec( u32 dwRecorderID , const TRecParam *ptRecParam );

/**
 *@brief			�����л�¼���ļ����м���¼�񣬲�ֹͣ����
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const s8 * pszPartitionName	¼���ļ��л���Ŀ�ķ�����NULL��ʾ��ԭ�����ϼ���¼��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note				
 */
u16 RPSwitchRec( u32 dwRecorderID, const s8 * pszPartitionName = NULL);

/**
 *@brief			ѡ��ý��������¼��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		u8 byStreamId			ý������ţ�1����Ƶ��2/3����Ƶ
 *@param[in]		BOOL32 bSave			�Ƿ�¼��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ĭ������������¼�ƣ�����������¼��ĳ����������ʹ�øýӿ����ò�¼�ƣ�
 *                ���Ǳ��뱣֤¼��һ����Ƶ��
 */
u16 RPSetRecStreamSave( u32 dwRecorderID, u8 byStreamId, BOOL32 bSave );

/**
 *@brief			��ͣ¼��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			���øýӿں󣬽���ͣ�����Ľ��գ��Ӷ��ﵽ��ͣ¼���Ŀ��
 */
u16 RPPauseRec( u32  dwRecorderID );

/**
 *@brief			����¼��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			���øýӿں󣬽��ָ������Ľ��գ��Ӷ��ﵽ����¼���Ŀ��
 */
u16 RPResumeRec( u32 dwRecorderID );

/**
 *@brief			����¼�����¼�Ʒ�������֡¼���������¼��,�����Ƶ֡����
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		BOOL32 bOnlySaveKeyFrame	�Ƿ�ֻ¼�ؼ�֡
 *@param[in]		u32 dwSaveFrameInterval	�ؼ�֡������������ؼ�֡¼��һ���ؼ�֡��Ŀǰ�ò�����Ч������¼�����йؼ�֡
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ĭ����Ϊ��¼�����е�֡���ݣ�����ʹ�øýӿڸı�¼�Ʒ���
 */
u16 RPSetRecorderSaveMethod( u32 dwRecorderID, BOOL32 bOnlySaveKeyFrame = FALSE, u32 dwSaveFrameInterval = 1 );

/**
 *@brief			ָֹͣ��¼�����¼��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		u32 dwStopTime		ֹͣʱ�䣬Ŀǰ��Ч
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿڵ��ú󣬻�����ֹͣ������գ������ļ���ȵ������е����ݶ�д��֮���ٹر�
 *                ��˲�Ҫ�ڸýӿڷ��غ�����ʹ��RPReleaseRec�ӿ��ͷ�¼����Դ������ᶪʧ����
 *                ¼�����ݣ����յ�RECORDER_REC_STOP�ص���Ϣ֮�����ͷ�¼����Դ
 */
u16 RPStopRec( u32 dwRecorderID, u32 dwStopTime = 0 );

/**
 *@brief			����¼������ش��������
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		TRPRecRSParam *ptRecRSParam	�ش�����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetRecRsParam( u32 dwRecorderID, TRPRecRSParam *ptRecRSParam );

/**
 *@brief			ʹ��ָ����¼�����ʼԤ¼
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		BOOL32 bPreRecInMem	�Ƿ�ʹ���ڴ�Ԥ¼
 *@param[in]		u32 dwPreRecTimeInterval	Ԥ¼ʱ������λ��
 *@param[in]		const TRecParam *ptRecParam	¼�����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStartPreRec( u32 dwRecorderID , BOOL32 bPreRecInMem, u32 dwPreRecTimeInterval, const TRecParam *ptRecParam );

/**
 *@brief			ֹͣԤ¼
 *@param[in]		u32 dwRecoderID		¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStopPreRec( u32 dwRecorderID );

/**
 *@brief			�õ���ǰԤ¼��ʱ��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[out]		u32& dwPreRecTime		Ԥ¼ʱ������λ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetPreRecTime( u32 dwRecorderID , u32& dwPreRecTime );

/**
 *@brief			��ȡ��ǰ����¼�Ƶ�¼���ļ����ܳ���
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[out]		u64& qwRecordTotalLength	¼���ļ��ĳ��ȣ���λ�ֽ�
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecordTotalLength( u32 dwRecorderID , u64& qwRecordTotalLength );

/**
 *@brief			��ȡ����¼�Ƶ�¼���ļ�����ʱ��
 *@param[in]		u32 dwRecoderID		¼���ID
 *@param[in]		const s8* szFileName	¼���ļ���
 *@param[out]		u32& dwPlayDuration	¼���ļ���ʱ������λ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecDuration(u32 dwRecorderID,const s8* szFileName, u32& dwPlayDuration);

/**
 *@brief			ֹͣ¼���ͷ�¼����Դ
 *@param[in]		u32 dwRecorderID		¼���ID
 *@param[in]		BOOL32 bDelete		�Ƿ�ͬʱɾ��¼���ļ��� ������Ҫ�����������ΪTRUE�������ܻᵼ��������ϴ���ӳ٣���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStopRecAndRelease(u32 dwRecorderID , BOOL32 bDelete = FALSE ) ;

/**
 *@brief			����¼���״̬�ص�����
 *@param[in]		u32 dwRecorderID		¼���ID
 *@param[in]		RecorderStateCallback pfStatCB	¼��״̬�ص�����
 *@param[in]		KD_PTR hUserHandle	�ص�������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetRecStateCallback(u32 dwRecorderID, RecorderStateCallback pfStatCB, KD_PTR hUserHandle);

/**
 *@brief			������������õ����õķ�������
 *@param[out]		u32 *pdwPlayerID		��Ч�ķ����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPCreatePlay(u32 *pdwPlayerID);

/**
 *@brief			���÷����״̬�ص�����
 *@param[in]		u32 dwPlayerId		�����ID
 *@param[in]		PlayerStateCallback pfStateCB	����״̬�ص�����
 *@param[in]		KD_PTR hUserHandle	�ص�������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayStateCallback(u32 dwPlayerId, PlayerStateCallback pfStateCB, KD_PTR hUserHandle);

/**
 *@brief			�ͷŷ����
 *@param[in]		u32 dwPlayerId		�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPReleasePlay( u32 dwPlayerID );

/**
 *@brief			ֹͣ�����ͷŷ�����Դ
 *@param[in]		u32 dwPlayerID		�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStopPlayAndRelease(u32 dwPlayerID ) ;
	
/**
 *@brief			�����ļ����Ž��Ȼص�������
 *@param[in]		u32 dwPlayerID		�����ID
 *@param[in]		u8 byGran			�����ļ��Ľ��Ȼص����ȣ���λ�룬ȡֵ��Χ[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ڲ�Ĭ��1��ص�һ�ν��ȣ������ı�ص����ȣ�����ʹ�øýӿ�����
 */
u16 RPSetReadCallBackGran( u32 dwPlayerID, u8 byGran );

/**
 *@brief			���÷��ͳ�ȥ������ʹ�õ��غ�ֵ
 *@param[in]		u32 dwPlayerID		�����ID
 *@param[in]		const TLocalPTParam *ptPTParam	�غɲ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			��ʹ��Kdvmedianet��������ʱ��Ч
 */
u16 RPSetLocalPT (u32 dwPlayerID, const TLocalPTParam *ptPTParam );

/**
 *@brief			���ü��ܲ���
 *@param[in]		u32 dwPlayerID		�����ID
 *@param[in]		TEncDecParam *ptEncryptParam	���ܲ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			��ʹ��Kdvmedianet��������ʱ��Ч
 */
u16 RPSetEncryptKey( u32 dwPlayerID, TEncDecParam *ptEncryptParam );

/**
 *@brief			��ȡ�ļ���ý����Ϣ
 *@param[in]		u32 dwPlayerID		�����ID
 *@param[in]		s8 *pchFullFileName	�ļ���
 *@param[out]		TRPMediaInfo *ptMediaInfo	ý����Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�һ����RPCreatePlay�ӿ�֮��ʹ�ã����ڻ�ȡ�ļ���ý����Ϣ
 */
u16 RPGetFileMediaInfo( u32 dwPlayerID, s8 *pchFullFileName, TRPMediaInfo *ptMediaInfo );

/**
 *@brief			��ȡ�ļ���Ϣ
 *@param[in]		s8 *pchFullFileName	�ļ���
 *@param[out]		TRPFileInfo *ptFileInfo	�ļ���Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetFileInfo( s8 *pchFullFileName, TRPFileInfo *ptFileInfo );

/**
 *@brief			��ȡ�ļ�����չ��Ϣ
 *@param[in]		s8 *pchFullFileName	�ļ��������ļ���
 *@param[out]		TMediaInfoExt *ptMediaInfoExt	�ļ���չ��Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetFileExtInfo(s8 *pchFullFileName, TMediaInfoExt *ptMediaInfoExt);

/**
 *@brief			�������߽����ļ�
 *@param[in]		const s8* pchRecFilePath	�ļ�����·��
 *@param[in]		const s8* pchRecFileName	�ļ���
 *@param[in]		BOOL32 bLock				TRUE��������FALSE������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			��ϣ��ĳ��¼���ļ��������ǻ�����Ϊɾ��ʱ������ʹ�øýӿ�����¼���ļ�
 */
u16 RPLockFile(const s8* pchRecFilePath, const s8* pchRecFileName, BOOL32 bLock, u64 *pqwFileSize = 0);

/**
 *@brief			ʹ��ָ���ķ���������������
 *@param[in]		u32 dwPlayerID		�����ID
 *@param[in]		TPlayParam *ptPlayParam	�������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStartPlay( u32 dwPlayerID , TPlayParam *ptPlayParam);

/**
 *@brief			���÷���ģʽ
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u32 byPlayMode	����ģʽ��ȡֵ�μ�EmPlayerEffect
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayMode(u32 dwPlayerID, u32 byPlayMode);

/**
 *@brief			��������ģʽ
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u32 byPlayMode	����ģʽ��ȡֵ�μ�EmPlayerEffect
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
//u16 RPLockPlayMode(u32 dwPlayerID, u32 byPlayMode);

/**
 *@brief			��������ģʽ
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u32 byPlayMode	����ģʽ��ȡֵ�μ�EmPlayerEffect
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
//u16 RPUnLockPlayMode(u32 dwPlayerID,u8 byOnlyKeyframeRate=4);

/**
 *@brief			���÷�������
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u8 PlayType		�������ͣ�����RP_NORMAL_PLAY��
 *@param[in]		const s8* pbyCompanyName	������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayType(u32 dwPlayerID, u8 PlayType, const s8* pbyCompanyName = NULL/*add by jh*/);

/**
 *@brief			������һ֡
 *@param[in]		u32 dwPlayerID	�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			���øýӿں󣬽����뵥֡����ģʽ
 */
u16 RPPlayNextFrame(u32 dwPlayerID) ;

/**
 *@brief			ע�ᷢ�ͻص�������kdvmedianet����
 *@param[in]		TNetAddr* ptRcvAddr	������յ�ַ
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	���ݱ��ӿڻص�����
 *@param[in]		void* pvContext		�ص�������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note		
 */
u16 RPRegSndChannel(TNetAddr* ptRcvAddr, RPUdpPackRecvCallback pUdpCallBack, void* pvContext) ;

/**
 *@brief			ע�����ͻص�����
 *@param[in]		TNetAddr* ptRcvAddr	������յ�ַ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note		
 */
u16 RPUnRegSndChannel(TNetAddr* ptRcvAddr) ;

/**
 *@brief			��ͣ����
 *@param[in]		u32 dwPlayerID	�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPPausePlay( u32 dwPlayerID );

/**
 *@brief			�ָ�����
 *@param[in]		u32 dwPlayerID	�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ڵ�����RPPausePlay֮�������øýӿڲ��ָܻ�¼��Ĳ���
 */
u16 RPResumePlay( u32 dwPlayerID );

/**
 *@brief			ָֹͣ����������������
 *@param[in]		u32 dwPlayerID	�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPStopPlay( u32 dwPlayerID );

/**
 *@brief			���÷�������ش��������
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		TRPPlyRSParam *ptPlyRSParam	�ش�����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlyRsParam( u32 dwPlayerID, TRPPlyRSParam *ptPlyRSParam );

/**
 *@brief			���ÿ������
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u8 byQuickTimes	�������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPQuickPlay( u32 dwPlayerID , u8 byQuickTimes );

/**
 *@brief			������������
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u8 bySlowTimes	��������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPSlowPlay( u32 dwPlayerID , u8 bySlowTimes );

/**
 *@brief			����������ʱ�俪ʼ����
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		s32 nDragDropTime	�Ϸ�Ŀ��ʱ�䣬�����RPStartPlay�и����Ŀ�ʼ����ʱ���ƫ��ʱ�䣬��λ��
 *@param[in]		BOOL32 bSeekToKeyFrame		�Ƿ�λ���ؼ�֡��ʼ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPDragDropPlay( u32 dwPlayerID , s32 nDragDropTime, BOOL32 bSeekToKeyFrame = TRUE );

/**
 *@brief			�Ӹ���ʱ�俪ʼ����
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u32 dwReversePlayTime	���ſ�ʼʱ�䣬���ļ��е����ʱ�䣬��λ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPReversePlay( u32 dwPlayerID, u32 dwReversePlayTime);

/**
 *@brief			�Ӹ���ʱ�俪ʼ����
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[in]		u32 dwForwardPlayTime	���ſ�ʼʱ�䣬���ļ��е����ʱ�䣬��λ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPForwardPlay( u32 dwPlayerID, u32 dwForwardPlayTime);

/**
 *@brief			��ѯָ��¼�����¼��״̬
 *@param[in]		u32 dwRecorderID	¼���ID
 *@param[out]		TRecorderStatus *ptRecorderStatus	¼�����¼��״̬
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecorderStatus( u32 dwRecorderID, TRecorderStatus *ptRecorderStatus );

/**
 *@brief			��ѯָ��������ķ���״̬
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[out]		TPlayerStatus *ptPlayerStatus	������ķ���״̬
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayerStatus( u32 dwPlayerID, TPlayerStatus *ptPlayerStatus );

/**
 *@brief			��ѯָ��¼�����¼��ͳ����Ϣ
 *@param[in]		u32 dwRecorderID	¼���ID
 *@param[out]		TRecorderStatis *ptRecorderStatis	¼�����ͳ����Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecorderStatis( u32 dwRecorderID, TRecorderStatis *ptRecorderStatis );

/**
 *@brief			��ѯָ��������ķ���ͳ����Ϣ
 *@param[in]		u32 dwPlayerID	�����ID
 *@param[out]		TPlayerStatis *ptPlayerStatis	�������ͳ����Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayerStatis( u32 dwPlayerID, TPlayerStatis *ptPlayerStatis );

/**
 *@brief			��ӡָ��¼�����¼��״̬
 *@param[in]		u32 dwRecorderID	¼���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ŀǰ�ýӿ���Ч
 */
API u16 recstatus( u32 dwRecorderID );

/**
 *@brief			��ӡָ��������ķ���״̬
 *@param[in]		u32 dwPlayerID	�����ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ŀǰ�ýӿ���Ч
 */
API u16 plystatus( u32 dwPlayerID );

/**
 *@brief			����mediapack�Ľ��ջ����С
 *@param[in]		u32 dwVideoBuffSize	Mediapack���ͻ����С�����ڷ��ƴ洢�ϵ��ļ�������Ч
 *@param[in]		u32 dwKeyFrameNum		Mediapack�п��Ի�������ؼ�֡���������ƴ洢�ϵ��ļ�������Ч
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�Ϊ���������Ҫ�ڲ���ʱʹ�ã����õĲ����ڵ��ø�����֮�����ķ�����Ч
 */
API void rpsmpvb(u32 dwVideoBuffSize, u32 dwKeyFrameNum);

/**
 *@brief			��ӡ¼�����汾��Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�Ϊ��������
 */
API u16 rpver();

/**
 *@brief			¼������������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
API u16 rphelp();

/**
 *@brief			��ȡ�ļ��Ĳ���ʱ������ʼʱ��
 *@param[in]		s8* pchPath		�ļ���	
 *@param[out]		u32& dwPlayDuration	�ļ��Ĳ�����ʱ��
 *@param[out]		u32& dwFileStartTime	�ļ��Ŀ�ʼʱ�䣬����ʱ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayDuration(s8* pchPath, u32& dwPlayDuration, u32& dwFileStartTime );

/**
 *@brief			�о�ָ��Ŀ¼�µ��ļ�
 *@param[in]		s8* pchPath		ָ��Ŀ¼	
 *@param[out]		TListInfo *ptListInfo	�ļ��б�
 *@param[in]		BOOL32 bListAll		�Ƿ��г�Ŀ¼�µ������ļ���
 *									TRUE���б��а���Ŀ¼�����д��ڵ��ļ���������ý���ļ�
 *									FALSE���б���ֻ����Ŀ¼�µ�����ý���ļ�
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPListFile( s8 *pchPath, TListInfo *ptListInfo, BOOL32 bListAll = FALSE );

/**
 *@brief			ɾ��¼���ļ�
 *@param[in]		s8* pchPath		�ļ�Ŀ¼	
 *@param[in]		s8* pchName		�ļ���
 *@param[in]		BOOL32 bBigFile	�Ƿ�Ϊ���ļ�ɾ����TRUE��pchName��������ΪNULL
 *@param[in]		BOOL32 bDeleteAll	�Ƿ�ɾ�����е��ļ���TRUE��ɾ�������ļ���������ý���ļ���FALSE��ֻɾ��ý���ļ�
 *@param[in]		u32 dwWaitTime	�ȴ�ɾ��ʱ�䣬��λ���룬0��ʾʹ��ϵͳĬ�ϵĳ�ʱʱ��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPDeleteFile( s8 *pchPath, s8* pchName = NULL, BOOL32 bBigFile = FALSE, BOOL32 bDeleteAll = FALSE, u32 dwWaitTime = 0);

/**
 *@brief			������ָ���ļ�
 *@param[in]		s8 *pchOldName	ԭ�ļ���	
 *@param[in]		s8 *pchNewName	���ļ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPRenameFile(s8 *pchOldName, s8 *pchNewName);

/**
 *@brief			�����ļ���Ŀ¼�Ƿ����
 *@param[in]		s8 * pchFileName	�ļ�����Ŀ¼��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPIsFileExist(s8 * pchFileName);

/**
 *@brief			����Ŀ¼
 *@param[in]		s8 *pchDirName	Ŀ¼��
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPCreateDir(s8 *pchDirName);

/**
 *@brief			����ý��������ʱ��IP���ݰ���TOSֵ
 *@param[in]		int nTOS		TOSֵ
 *@param[in]		int nType	ý�������ͣ�0:ȫ���� 1:��Ƶ 2:��Ƶ 3: ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			ʹ��Kdvmedianet�⣬���������׽��ַ���ʱ��Ч
 */
int SetRPMediaTOS( int nTOS, int nType );

/**
 *@brief			��ȡIP���ݰ���TOSֵ
 *@param[in]		int nType	ý�������ͣ�0:ȫ���� 1:��Ƶ 2:��Ƶ 3: ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
int GetRPMediaTOS( int nType );

/**
 *@brief			�������ؽ��ն��󣬵õ����õ����ؽ��ձ��
 *@param[out]		u8 *pbyFtpRcvID	��Ч�����ؽ���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			ʹ��TCP���ط�ʽʱ��������ʹ�øýӿڷ������ؽ�����Դ�����Ƽ�ʹ��TCP���ط�ʽ�����нӿ�
 */
u16 RPCreateFtpRcv( u8 *pbyFtpRcvID );

/**
 *@brief			�ͷ����ؽ��ն���
 *@param[in]		u8 byFtpRcvID		��Ч�����ؽ�����ԴID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP��ʽ����
 */
u16 RPReleaseFtpRcv( u8 byFtpRcvID );

/**
 *@brief			��ʼ���ؽ���
 *@param[in]		u8 byFtpRcvID		��Ч�����ؽ�����ԴID
 *@param[in]		TFtpRcvParam *ptFtpRcvParam	���ؽ��ղ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP��ʽ����
 */
u16 RPStartFtpRcv( u8 byFtpRcvID , TFtpRcvParam *ptFtpRcvParam );

/**
 *@brief			ֹͣ���ؽ���
 *@param[in]		u8 byFtpRcvID		��Ч�����ؽ�����ԴID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP��ʽ����
 */
u16 RPStopFtpRcv( u8 byFtpRcvID );
	
/**
 *@brief			�������ط��Ͷ��󣬵õ����õ����ط��ͱ��
 *@param[out]		u8 *pbyFtpSndID	��Ч�����ط���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP���ط�ʽ�����ط��Ͷ˱�����ʹ�øýӿڷ���������Դ
 */
u16 RPCreateFtpSnd( u8 *pbyFtpSndID );
	
/**
 *@brief			�ͷ����ط��Ͷ���
 *@param[in]		u8 byFtpSndID		��Ч�����ط���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP���ط�ʽ
 */
u16 RPReleaseFtpSnd( u8 byFtpSndID );

/**
 *@brief			�����ļ����ؽ��Ȼص�������
 *@param[in]		u8 byFtpSndID		��Ч�����ط���ID
 *@param[in]		u8 byGran		���ؽ��Ȼص����ȣ���λ�룬ȡֵ��Χ[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP���ط�ʽ
 */
u16 RPSetFtpReadCallBackGran( u8 byFtpSndID, u8 byGran );

/**
 *@brief			��ʼ���ط���
 *@param[in]		u8 byFtpSndID		��Ч�����ط���ID
 *@param[in]		TFtpSndParam *ptFtpSndParam	���ط��Ͳ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP���ط�ʽ
 */
u16 RPStartFtpSnd( u8 byFtpSndID , TFtpSndParam *ptFtpSndParam );

/**
 *@brief			ֹͣ���ط���
 *@param[in]		u8 byFtpSndID		��Ч�����ط���ID
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������TCP���ط�ʽ
 */
u16 RPStopFtpSnd( u8 byFtpSndID );

/**
 *@brief			��ѯ�������ڽ��е�TCP�����������Ϣ
 *@param[out]		TFtpSndQuery& tFtpSndQuery	����������Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			ʹ����TCP���ط�ʽ
 */
u16 RpFtpSndQuery(TFtpSndQuery& tFtpSndQuery);

/** ע�⣺����ӿڽ���codecwrapper���ã�����ģ�鲻Ҫ���� !!! */

/**
 *@brief			����¼���ID��ȡ¼�����ַ
 *@param[in]		u32 dwRecId		¼���ID��ͨ��RPCreateRec()�ӿڵõ�
 *@param[out]		u64 *pllAddr		¼�����ַ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 GetRecAddr( u32 dwRecId, u64 *pllAddr );

/**
 *@brief			¼���������뺯��
 *@param[in]		PFRAMEHDR pFrmHdr		��Ҫ¼���֡����
 *@param[in]		void* pvContext		¼��������ģ�TCBContext�ṹ��ָ�룬
 *                                       ���е�m_llAddrֵΪͨ��GetRecAddr�ӿڵõ���¼�����ַ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
void RecDataPrc( PFRAMEHDR pFrmHdr, void* pvContext );

/**
 *@brief			¼���������뺯�������PS��
 *@param[in]		u8 * pbPes		PES��
 *@param[in]		u32 dwLen		PES������
 *@param[in]		void* pvContext		¼��������ģ�TCBContext�ṹ��ָ�룬
 *                                       ���е�m_llAddrֵΪͨ��GetRecAddr�ӿڵõ���¼�����ַ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ŀǰ�ýӿ�δʵ��
 */
//void RecDataPrc( u8 * pbPes, u32 dwLen, void* pvContext);

/**
 *@brief			���һ·PS��
 *@param[in]		u32 dwRecorderID	¼���ID
 *@param[in]		u8 byStreamID		ý����ID��PES���е�StreamID��0xE0,0xC0һ��
 *@param[in]		u8 byStreamType	ý�������ͣ���kdmpsts.h
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			Ŀǰ�ýӿ�δʵ��
 */
//u16 RPAddPSStream( u32 dwRecorderID, u8 byStreamID, u8 byStreamType);

/**
 *@note	����Ϊudp��ʽ���ص���ؽӿ�
 *       �������ط����: �ȵ���RpFtpStartSndService�������ط��ͷ���,
 *       �ٵ���RpFtpSndProbePack����̽���
*/

/**
 *@brief			�������صķ���˷���
 *@param[in]		TFtpSndServParam tFtpSndServParam	���ط���˲���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�ֻ��Ҫ�ڳ��������󣬵���һ�μ��ɣ�����Ҫÿִ��һ����������͵���һ��
 */
u16 RpFtpStartSndService(TFtpSndServParam tFtpSndServParam);

/**
 *@brief			ֹͣ���صķ���˷���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿڵ��ú󣬷���˽������ṩ���ط��񣬽����ڳ����˳�ʱ���ü���
 */
u16 RpFtpStopSndService();

/**
 *@brief			����˷���̽����ӿ�
 *@param[in]		u8* pBuf		Ҫ���͵�̽�������
 *@param[in]		u32 dwBufLen	̽������ݳ���
 *@param[in]		TNetAddr tRemoteAddr	���͵�Ŀ�ĵ�ַ
 *@param[in]		u32 dwNatIntvl	����̽�����Ƶ�ʣ���λ�룬0��ʾ���ϲ�Ļ��巢�ͣ���ֻ����һ��
 *                                  ��0��ʾRPCTRL���Զ�����̽������Һ���pBuf������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpSndProbePack(u8* pBuf,	u32 dwBufLen, TNetAddr tRemoteAddr, u32 dwNatIntvl = 0);

/**
 *@brief			����˹��˺�������
 *@param[in]		u32 dwRecvIP	���ؽ���IP��ַ
 *@param[in]		u16 wRecvPort	���ؽ��ն˿�
 *@param[in]		u32 dwSrcIP	����ԴIP��ַ
 *@param[in]		u16 wSrcPort	����Դ�˿�
 *@param[in]		u8* pData	�������ݻ���
 *@param[in]		u32 uLen		�������ݳ���
 *@return		0����ʾ��Ҫ����������0�����ݰ�������
 *@ref
 *@see
 *@note			
 */
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);

/**
 *@brief			���÷���˹��˺���
 *@param[in]		FilterFunc ptFunc		���˺���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpSetFilterFun(FilterFunc ptFunc);

/**
 *@brief			�������ؿͻ���
 *@param[in]		TNetAddr& tLocalAddr	���ؿͻ��˱���ʹ�õ������ַ
 *@param[out]		u32* pdwSsid			��Ч����������Ψһ��ʶ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvCreate(TNetAddr& tLocalAddr, u32* pdwSsid );

/**
 *@brief			�ͻ��˷���̽���
 *@param[in]		u32 dwSsid		��������Ψһ��ʶ
 *@param[in]		TNetAddr& tRemoteAddr	̽������͵�Ŀ�ĵ�ַ
 *@param[in]		u8* pBuf			̽�������
 *@param[in]		u32 dwBufLen		̽������ݳ���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvSndProbe(u32 dwSsid, TNetAddr& tRemoteAddr, u8* pBuf, u32 dwBufLen );

/**
 *@brief			������������
 *@param[in]		u8 DownloadType	��������
 *@param[in]		u32 dwSsid		��������Ψһ��ʶ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ýӿڲ���ʹ��
 */
//u16 RpFtpDownloadType(u8 DownloadType, u32 dwSsid);

/**
 *@brief			�ͻ��˿�ʼ����
 *@param[in]		TFtpUdpRcvParam tFtpRcvParam	���ؽ��ղ���
 *@param[in]		u32 dwSsid		��������Ψһ��ʶ
 *@param[in]		u32 dwNatIntvl	̽������ͼ����0��ʾ������̽�������0ֵ��ʾ����̽���
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpStartRcv(TFtpUdpRcvParam tFtpRcvParam, u32 dwSsid, u32 dwNatIntvl/*=0*/);

/**
 *@brief			���÷��Ͷ˵���������
 *@param[in]		s32 value	���ط������ʣ�bytes per sec��������ڵ���20480,0����-1��ʾ������
 *@return		
 *@ref
 *@see
 *@note			
 */
API void RpFtpSetSndSpeedLimit(s32 value );

/**
 *@brief			�ͻ���ֹͣ����
 *@param[in]		u32 dwSsid	��������Ψһ��ʶ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpStopRcv(u32 dwSsid);

/**
 *@brief			���ؿͻ���״̬��ѯ
 *@param[in]		u32 dwSsid	��������Ψһ��ʶ
 *@param[out]		TFtpRcvQuery& tFtpRcvQuery		�ͻ���״̬��Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvQuery(u32 dwSsid, TFtpRcvQuery& tFtpRcvQuery);

/**
 *@brief			�������ؿͻ��˵��ش�ʱ�������ش�����
 *@param[in]		u32 dwRepeatInterval	�ش�ʱ��������λ����
 *@param[in]		u32 dwRepeatCount		�ش�����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
API u16 RPSetFtpRepeatPara(u32 dwRepeatInterval , u32 dwRepeatCount);

/**
 *@brief			��ȡRPCTRL�����綪����IO��ʱͳ����Ϣ
 *@param[out]		TRpStatics* pStaics	ͳ����Ϣ
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
u16 RPGetStatics(TRpStatics* pStaics);

/**
 *@brief			��ȡ������Ϣ
 *@param[in]		s8* szDiskName	���̷�����
 *@param[out]		TRPDiskInfo *ptInfo	������Ϣ
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note			
 */
BOOL32 RPGetDiskInfo(s8* szDiskName, TRPDiskInfo *ptInfo);

/**
 *@brief			���ý��ն˵���������
 *@param[in]		u32 dwSsid	��������Ψһ��ʶ
 *@param[in]		s32 nSpeed	���ʣ����ڵ���10240��0����-1��ʾ������
 *@return		
 *@ref
 *@see
 *@note			
 */
API u16 RpFtpSetRcvSpeedLimit(u32 dwSsid, s32 nSpeed);

/**
 *@brief			���ع��ܵĵ��������
 *@param[in]		u32 nDebug	���������
 *							1--������������Ĵ�ӡ��Ϣ�������еĴ�ӡ�����
 *							2--�ش�������ش�ӡ
 *							3--�������ݵ�ʱ�����������ش�ӡ
 *							4--�ͻ������ٴ�ӡ
 *							5--���ز�����ӡ
 *							6--������ش������ʹ�ӡ
 *@return		
 *@ref
 *@see
 *@note			
 */
API void rpftpdebug(u32 nDebug);

API void rpdlsto(u32 dwDownloadSndTimeout);

API void rpdlrto(u32 dwDownloadRecvTimeout);

/**
 *@brief			���ط���˷�����Ϣ��ӡ�����������������������߳�״̬�����ط���˵�ַ
 *@return		
 *@ref
 *@see
 *@note			telnet��������
 */
API void rpftpsnd();

/**
 *@brief			���ؿͻ�����Ϣ��ӡ�����������������������߳�״̬�����ؿͻ����̵߳�WATCH�׽��ֵ�ַ
 *@return		
 *@ref
 *@see
 *@note			telnet��������
 */
API void rpftprcv();

/**
 *@brief			dataswitchע����պ�������
 *@param[in]		void* ptNetAddr	������յ�ַ	
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	�������ݰ����ջص�����
 *@param[in]		KD_PTR pvContext	�ص�������
 *@return		1���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
typedef u32 (*dsRegRcvChannelP)(void* ptNetAddr, RPUdpPackRecvCallback pUdpCallBack, KD_PTR pvContext);

/**
 *@brief			dataswitch��ע����պ�������
 *@param[in]		void* ptNetAddr	������յ�ַ	
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	�������ݰ����ջص�����
 *@param[in]		KD_PTR pvContext	�ص�������
 *@return		1���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
typedef u32 (*dsUnRegRcvChannelP)(void* ptNetAddr, RPUdpPackRecvCallback pUdpCallBack, KD_PTR pvContext);

/**
 *@brief			����ע�ắ��
 *@param[in]		dsRegRcvChannelP p	ע�ắ��	
 *@return		
 *@ref
 *@see
 *@note			
 */
void RPSetDSRegPointer(dsRegRcvChannelP p);

/**
 *@brief			���÷�ע�ắ��
 *@param[in]		dsUnRegRcvChannelP p	��ע�ắ��	
 *@return		
 *@ref
 *@see
 *@note			
 */
void RPSetDSUnRegPointer(dsUnRegRcvChannelP p);

/**
 *@note ���½ӿ������ص��������̵�ǰ��¼��ʱʹ�ã�Ŀǰ��Ҫ��G����ʹ��
 */

/** �����������ļ���Ϣ*/
typedef struct tag3rdComDlFileInfo
{
	u32 m_dwStartTime;						///<���ؿ�ʼʱ�䣬����ʱ��
	u32 m_dwStopTime;							///<���ؽ���ʱ�䣬����ʱ��
	s8 m_abyFilePath[MAX_FILE_PATH_LEN];		///<�����ص��ļ�����·��
	s8 m_abyFileName[MAX_FILE_NAME_LEN];		///<�����ص��ļ����ļ���
}T3rdComDlFileInfo;

/** �������ļ�����ʱ��ý��������Ϣ*/
typedef struct tag3rdStreamProperty
{
	u8 m_byStreamID;							///<ý�������	
	u8 m_byMediaType;							///<ý������	97:MPEG4, 106:H264
	union
	{
		TVideoStreamProperty   m_tVideoInfo;	///<��Ƶ������
		TAudioStreamProperty   m_tAudioInfo;	///<��Ƶ������
	};
}T3rdStreamPropery;

/** �������ļ�����ʱ��ý��������Ϣ��*/
typedef struct tag3rdStreamPropertySet
{
	u8                  m_byTotalStreamNum;					///<ý��������
	T3rdStreamPropery		m_at3rdStreamProperty[MAX_STREAM_NUM];	///<ý����������Ϣ
}T3rdStreamPropertySet;

/** �������ļ�����ʱ��ý��������*/
typedef struct tag3rdComStreamPayLoad
{
	u8		m_byStreamID;					///<ý�������
	BOOL32	m_bIsKeyFrame;					///<�غ������Ƿ�Ϊ�ؼ�֡
	u32		m_dwPresentationTime;				///<�غ����ݵ�ʱ�������λ����
	u32		m_dwPayLoadSize;					///<�غɴ�С
	u8		*m_pbyPayLoad;					///<�غ�ʵ�����ݣ��ڴ����ṩ�����߹���
}T3rdComStreamPayLoad;

/**
 *@brief			��ʼ������¼�����غ�������
 *@param[in]		const u32 dwSsid		��������Ψһ��ʶ	
 *@param[in]		T3rdComDlFileInfo t3ComDlFileInfo	���ص��ļ���Ϣ
 *@param[out]		T3rdStreamPropertySet *pt3rdStreamPropertySet	�ļ�ý��������Ϣ����
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
typedef u16 (*RP3rdComStartDownLoad)(const u32 dwSsid, T3rdComDlFileInfo t3ComDlFileInfo, T3rdStreamPropertySet *pt3rdStreamPropertySet);

/**
 *@brief			��ȡ�����ļ���ý������
 *@param[in]		const u32 dwSsid		��������Ψһ��ʶ	
 *@param[out]		BOOL32* pbReadFileEnd	�Ƿ񵽴��ļ�β
 *@param[out]		T3rdComStreamPayLoad *pt3ComStreamPayLoad	ý������
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
typedef u16 (*RP3rdComGetStreamData)(const u32 dwSsid, BOOL32* pbReadFileEnd, T3rdComStreamPayLoad *pt3ComStreamPayLoad);

/**
 *@brief			ֹͣ������¼������
 *@param[in]		const u32 dwSsid		��������Ψһ��ʶ	
 *@return		
 *@ref
 *@see
 *@note			
 */
typedef void (*RP3rdComStopDownLoad)(const u32 dwSsid);

/** ���������صĲ���������*/
typedef struct tag3rdComDlOperSet
{
	RP3rdComStartDownLoad  m_pf3rdComStartDownLoad;		///<��ʼ���غ���
	RP3rdComGetStreamData  m_pf3rdComGetStreamData;		///<��ȡ�������ݺ���
	RP3rdComStopDownLoad   m_pf3rdComStopDownLoad;		///<ֹͣ���غ���
}T3rdComDlOperSet;

/**
 *@brief			ע�����ص�����¼���ļ��Ĳ�������
 *@param[in]		T3rdComDlOperSet t3rdComDlOperSet	���ز�����	
 *@return		RP_OK���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			���ڶ�·���صĿ��ǣ��뱣֤�������и������Ŀ�������
 */
u16 RP3rdComDlOperSetRegister(T3rdComDlOperSet t3rdComDlOperSet);

#endif  //_RPCtrl_H_
