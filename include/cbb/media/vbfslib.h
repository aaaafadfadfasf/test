/**
* @file		vbfslib.h
* @brief		����vbfs�ļ�ϵͳ�Ľӿ��ļ�
* @author	�����
* @date		2016-06-16
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/
#ifndef _VBFSLIB_20100104_H_
#define _VBFSLIB_20100104_H_

/** vbfs�ļ�ϵͳ����������¼���ļ���*/
#define MAXLEN_RECNAME   256

/** vbfs�ļ�ϵͳ����������ǰ��ͨ����*/
#define MAXLEN_CHANNAME  32

/** vbfs�ļ�ϵͳ����������ǰ�˳�����*/
#define MAXLEN_PRODUCT   8

/** vbfs�ļ�ϵͳ��������������ʽ��*/
#define MAXLEN_FORMAT    4

/** vbfs�ļ�ϵͳ����������ǰ��ͨ����ų���*/
#define LEN_CID          36

/** vbfs�ļ�ϵͳ����������¼���ļ�����*/
#define MAXNUM_REC       (1<<20)

#define STRACPY(dst,src) {s32 len=sizeof(dst)>sizeof(src)?sizeof(src):sizeof(dst); strncpy(dst,src,len); dst[sizeof(dst)-1]=0;}

/** �ļ��������*/
typedef int   hs;

/** ��Ч���ļ����ֵ����*/
#define INVALID_HS -1

/** �����붨��*/
#define VBFS_OK                 					(u16)0						///<VBFS�����ɹ�
#define VBFSERR_BASE								(u16)3000					///<VBFS����ʧ�ܵ���ʼ������ֵ
#define VBFSERR_FILE_NOT_EXIST                     	(u16)(VBFSERR_BASE + 1)		///<��ȡRIAʧ��
#define VBFSERR_GET_DISK_FAIL                      	(u16)(VBFSERR_BASE + 2)		///<��ȡ����ʧ��
#define VBFSERR_WRITE_TO_RIA_ERR                   	(u16)(VBFSERR_BASE + 3)		///<��RIA��Ϣд�����ʧ��
#define VBFSERR_UNKNOW                            	(u16)(VBFSERR_BASE + 4)		///<δ֪����
#define VBFSERR_FILE_IS_LOCKED                     	(u16)(VBFSERR_BASE + 5)		///<�ļ��Ѿ�����
#define VBFSERR_NO_MEMORY		                    (u16)(VBFSERR_BASE + 6)		///<�޷����䵽�ڴ�

/** �ص��¼�����*/
#define EVENT_RECDELETE							1							///<¼��ɾ��

/** vbfsϵͳ�е��ļ���Ϣ�ṹ�嶨��*/
struct vbfs_statinfo
{
	u32 recid;								///<¼���ļ���ţ�Ŀǰδʹ��
	s8	recname[MAXLEN_RECNAME+4];				///<¼���ļ�������·���������ļ���
	u32 starttime;							///<¼���ļ��Ŀ�ʼʱ�䣬����ʱ��
	u32 endtime;								///<¼���ļ��Ľ���ʱ�䣬����ʱ��
	s8	cid[LEN_CID+4];						///<¼���ļ�������ǰ�˱��+ͨ���ţ�ǰ�˱��ռ��32λ��ͨ����ռ��4��Ŀǰδʹ��
	s8  cnm[MAXLEN_CHANNAME+4];				///<¼���ļ�������ǰ��ͨ�����ƣ�Ŀǰδʹ��
	u64 recbytes;								///<¼���ļ����ܴ�С����λ�ֽ�
    s8  product[MAXLEN_PRODUCT+4];				///<¼���ļ�������ǰ�����ڵĳ�������Ŀǰδʹ��
	s8  format[MAXLEN_FORMAT+4];				///<¼���ļ��е���Ƶ�������ƣ�����H264��MP4...��Ŀǰδʹ��
};

/** ��ĳ�ʼ������*/
struct TVbfsInitParam
{
	u64 llPreAllocMem;						///<Ԥ�����vbfs�ļ�ϵͳ���ڻ������ݵ��ڴ��С����λ�ֽڣ����ڴ��ڵ�һ��vbfs��������ʱ���з���
	u8  byDiskWriteMode;						///<����д�뷽ʽ��0 : ʹ��ϵͳ����, 1 : ʹ��O_DIRECT��ʽд��
	u8  byDiskAccessMode;						///<���̷����Ľ���ģʽ��0 : ISCSI�� 1 : ����
};

/**
 *@brief			��ĳ�ʼ������
 *@param[in]		TVbfsInitParam* ptInitParm  ��ĳ�ʼ������
 *@return		TRUE:�ɹ� FALSE:ʧ��
 *@ref
 *@see
 *@note			�ýӿڱ�����ʹ��vbfslib��������κι���֮ǰ���ɹ�����
 */
BOOL32  vbfs_init(TVbfsInitParam* ptInitParm);

/**
 *@brief			��ķ���ʼ������
 *@return		TRUE:�ɹ� FALSE:ʧ��
 *@ref
 *@see
 *@note			�ýӿڽ��ͷ����е���Դ
 */
BOOL32  vbfs_free();

/**
 *@brief			���������ڴ���ж��
 *@param[in]		const s8 *filename  ��Ҫж�صĴ��̷�������
 *@param[out]		BOOL32 *bNormalUnLoad  TRUE:����ж�سɹ���FALSE:������ж��
 *@return		TRUE:�ɹ� FALSE:ʧ��
 *@ref
 *@see
 *@note			ʹ�øýӿ�֮�󣬽�������ͨ��vbfslib����ʵ���Ӧ�ķ���
 */
BOOL32  vbfs_unloaddisk(const s8 *filename, BOOL32 *bNormalUnLoad);

/**
 *@brief			���������ص��ڴ���
 *@param[in]		const s8 *filename  ��Ҫ���صĴ��̷������ƣ�һ��Ϊ/vbfs/UUID����ʽ
 *@param[in]		const s8* diskname  ��Ҫ���صĴ��̷�����Ӧ���̷�����һ��Ϊ/dev/sda1��������ʽ
 *@return		TRUE:�ɹ� FALSE:ʧ��
 *@ref
 *@see
 *@note			����Ҫͨ��vbfslib�����ĳ��vbfs����ǰ��������øýӿڶԷ������м���
 *                �ýӿڵļ����ǽ�vbfs�������ص��û��ռ���ڴ��У����һ��Ҫ�����ڶ��������ͬʱ
 *                ����ͬһ��vbfs��������д����������ֻ�Ƕ�ȡ������Զ����ͬʱ����
 */
BOOL32  vbfs_loaddisk(s8 *filename, const s8* diskname);

/**
 *@brief			��ֻд��ʽ���������ļ�
 *@param[in]		const s8 *filename  ��Ҫд����ļ��������ļ���
 *@param[in]		const u32 dwFileStartAbsTime  �ļ��Ŀ�ʼʱ�䣬����ʱ�䣬�����0������vbfs��ȡ��ǰ����ʱ����Ϊ�ļ���ʼʱ��
 *@param[out]		u32* pdwFileHeadSize  ���ص�vbfs�ļ�ϵͳ�����������ļ�ͷ��С
 *@return		(0,2047]:�ɹ�����Ч���ļ���� INVALID_HS:ʧ��
 *@ref
 *@see
 *@note
 */
hs		vbfs_openforwrite( const s8 *filename, const u32 dwFileStartAbsTime = 0, u32* pdwFileHeadSize = NULL);

/**
 *@brief			��ֻ����ʽ���ļ�
 *@param[in]		const s8 *filename  ��Ҫ��ȡ���ļ��������ļ���
 *@param[out]		u32* pdwFileHeadSize  ����vbfs�ļ�ϵͳ�����������ļ�ͷ��С
 *@return		(0,2047]:�ɹ�����Ч���ļ���� INVALID_HS:ʧ��
 *@ref
 *@see
 *@note			
 */
hs		vbfs_openforread( const s8 *filename, u32* pdwFileHeadSize = NULL);

/**
 *@brief			��λ���ļ���ĳ��λ�ý��в���
 *@param[in]		hs stream  �ļ��������vbfs_openforwrite����vbfs_openforread�������ص�ֵ
 *@param[in]		s64 offset  ��λ��ƫ��������λ�ֽ�
 *@param[in]		s32 origin  ��λ�ķ�ʽ��ȡֵ��SEEK_SET��SEEK_CUR��SEEK_END
 *@return		0 : �ɹ���<0��ʧ��
 *@ref
 *@see
 *@note
 */
s32		vbfs_seek(hs stream, s64 offset, s32 origin);

/**
 *@brief			���ļ���д������
 *@param[in]		const void *buffer  ��Ҫд������ݻ���
 *@param[in]		u32 size  ���ݻ����С
 *@param[in]		u32 count  ��Ҫд��Ļ���������һ�㶼����Ϊ1
 *@param[in]		const u32 dwRecTime  д�뱾�λ����е����ݺ��ļ�����ʱ������λ��
 *@param[in]		hs stream  ��Ч���ļ����
 *@return		size*count : �ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note
 */
u32		vbfs_write(const void *buffer, u32 size, u32 count, const u32 dwRecTime, hs stream, u16 *pwErr = NULL);

/**
 *@brief			��ȡ�����ļ�������ļ��ĵ�ǰ��дλ��
 *@param[in]		hs stream  ��Ч���ļ����
 *@return		>0 : �ļ���ǰ��дλ�ã�<0��ʧ��
 *@ref
 *@see
 *@note
 */
s64		vbfs_tell( hs stream );

/**
 *@brief			��ȡ�ļ�������
 *@param[in]		void *buffer  ��Ŷ�ȡ���ݵĻ��壬���ϲ��ṩ
 *@param[in]		u32 size		���ݻ����С
 *@param[in]		u32 count	��Ҫ��ȡ�����ݿ������һ���ֵ������Ϊ1
 *@param[in]		hs stream	��Ч���ļ����
 *@return		>0 : �ɹ���<=0��ʧ��
 *@ref
 *@see
 *@note
 */
u32		vbfs_read( void *buffer, u32 size, u32 count, hs stream );

/**
 *@brief			�ж��ļ��Ƿ����
 *@param[in]		hs stream	��Ч���ļ����
 *@return		1 : �ļ�������0���ļ�δ������<0��ʧ��
 *@ref
 *@see
 *@note			��vbfs_read���صĶ�ȡ��СС��������ȡ�����ݴ�Сʱ��ʹ�øýӿ��ж��Ƿ��ȡ���ļ�β
 */
s32		vbfs_eof( hs stream );

/**
 *@brief			���ļ����ڴ��̷����ϵ���������ͬ�������̷���
 *@param[in]		hs stream	��Ч���ļ����
 *@return		0���ɹ���<0��ʧ��
 *@ref
 *@see
 *@note			��Linux����ϵͳ�У���ִ��fdatasync���������ܻ�Ӱ�����ܣ��벻Ҫ����ʹ�øýӿ�
 */
s32		vbfs_fsync( hs stream );

/**
 *@brief			�ر���ص��ļ����
 *@param[in]		hs stream	��Ч���ļ����
 *@param[in]		BOOL32 bFlushBuf TRUE:���ļ����ڴ����ϵ�����ͬ�������̷�����
 *@return		0���ɹ���<0��ʧ��
 *@ref
 *@see
 *@note			���bFlushBuf����ΪTRUE�����ִ����vbfs_fsync��ͬ�Ĳ���
 */
s32		vbfs_close( hs stream, BOOL32 bFlushBuf=FALSE );//

/**
 *@brief			Ϊ��Ӧ�ļ�������̿ռ�
 *@param[in]		u32 size  ��Ҫ����Ĵ�С
 *@param[in]		u32 count ��Ҫ�������ٿ�size��С�Ŀռ�
 *@param[in]		hs stream ��Ч���ļ����
 *@return		size*count���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			������ܴ�СΪsize*count
 */
u32		vbfs_allocspace(u32 size, u32 count, hs stream );

/**
 *@brief			�жϷ����Ƿ�Ϊvbfs����
 *@param[in]		const s8 *uuidname  ��������һ��Ϊ��Uuid������
 *@param[in]		s32 nReadDisk  �Ƿ��ȡ���̷��������ж�
 *@return		TRUE����vbfs������FALSE������vbfs����
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_isvbfsdisk(const s8 *uuidname, s32 nReadDisk=0);

/**
 *@brief			��ȡ���̷����Ŀռ�ʹ����Ϣ
 *@param[in]		const s8 *diskname  ��������һ��Ϊ��Uuid������
 *@param[out]		u64* pfreebytes  ʣ����õĴ��̷�����С����λ�ֽ�
 *@param[out]		u64* ptotalbytes  ���̷����ܴ�С����λ�ֽ�
 *@param[out]		u64* plockbytes  ���̷����ϱ��������ļ�ռ�õ��ܿռ䣬��λ�ֽ�
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_getdiskfreespace(const s8 *diskname, u64* pfreebytes, u64* ptotalbytes, u64* plockbytes);

/**
 *@brief			��ȡ�ļ���Ϣ
 *@param[in]		const s8 *filename  �����ļ���
 *@param[out]		vbfs_statinfo* pvbfs_stat  ���ص��ļ���Ϣ
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_stat(const s8 *filename, vbfs_statinfo* pvbfs_stat);

/**
 *@brief			����Ŀ¼
 *@param[in]		const s8 *dirname ��Ҫ������Ŀ¼����
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note			�ýӿ�Ŀǰδʵ��
 */
BOOL32  vbfs_mkdir(const s8 *dirname);

/**
 *@brief			ɾ���������ļ�
 *@param[in]		const s8 *filename ��Ҫɾ�����ļ�������ļ���
 *@return		0���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			
 */
s32  vbfs_remove(const s8 *filename);

/**
 *@brief			�������ļ�
 *@param[in]		const char *oldname ԭ�ļ�������·���������ļ���
 *@param[in]		const char *newname ���ļ�������·���������ļ���
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note			oldname��newname���뱣��ǰ���·��һ��
 */
BOOL32  vbfs_rename(const char *oldname, const char *newname );

/**
 *@brief			�������߽�������ļ�
 *@param[in]		const s8 *filename ��Ҫ�������߽������ļ�ȫ��
 *@param[in]		BOOL32 bLock  TRUE����������ļ���FALSE����������ļ�
 *@return		0���ɹ�������ֵ��ʧ��
 *@ref
 *@see
 *@note			�ļ�������״̬�����ܱ�ɾ����ֻ�н���֮����ܱ�ɾ��	
 */
s32  vbfs_lockfile(const s8 *filename, BOOL32 bLock);

/**
 *@brief			״̬�ص���������
 *@param[out]		u32 dwEvent   �¼���
 *@param[out]		u8 *pbyBuf    �¼�����
 *@param[out]		s32 nLen      �¼����ݳ���
 *@param[out]		KD_PTR pvContext  �ص�������
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*PStatCallBack)(u32 dwEvent, u8 *pbyBuf, s32 nLen, KD_PTR pvContext);

/**
 *@brief			����״̬�ص�����
 *@param[in]		PStatCallBack pfstat  �ص�����ָ��
 *@param[in]		KD_PTR pvContext      �ص�������
 *@return		TRUE���ɹ���FALSE��ʧ��
 *@ref
 *@see
 *@note				
 */
BOOL32  vbfs_setstatcallback(PStatCallBack pfstat, KD_PTR pvContext);

#endif//_VBFSLIB_20100104_H_

