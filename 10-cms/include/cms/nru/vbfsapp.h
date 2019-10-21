#ifndef _VBFS_APP_H
#define _VBFS_APP_H

#include "kdvtype.h"

#define MAXLEN_VBFSPATH      256
#define MAXLEN_MANUFACTURE   64


#define VBFSAPP_ERROR                 (-1)
#define VBFSAPP_OK                    (0)
#define VBFSAPP_ERR_INVALID_PARAM     (1)
#define VBFSAPP_ERR_OPENDISK          (2)
#define VBFSAPP_ERR_READDISK          (3)
#define VBFSAPP_ERR_NOTVBFS           (4)
#define VBFSAPP_ERR_FILEDESTROY       (5)
#define VBFSAPP_ERR_FSDESTROY         (6)
#define VBFSAPP_ERR_NOMEMORY          (7)
#define VBFSAPP_ERR_INVALID_FILENAME  (8)


#define DTYPE_DIR                      1
#define DTYPE_FILE                     2

struct vbfsapp_direct
{
	s8 d_type;    //D_DIR or D_FILE
	s8 d_name[MAXLEN_VBFSPATH];
};

struct vbfsapp_stat
{
	u32 st_starttime;
	u32 st_endtime;
	u64 st_filesize;   //bytes
};

struct vbfsapp_fextinfo
{
	s8 fe_format;
	s8 fe_manufacture[MAXLEN_MANUFACTURE];
};


/*====================================================================
������      : vbfsapp_opendisk
����        : �򿪴���
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN] psdiskname -- ��Ҫ�򿪵Ĵ�����
����ֵ˵��  :  ���ش򿪵Ĵ��̾��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2011/01/05             linlifen         ����
====================================================================*/
s32 vbfsapp_opendisk(const s8* psdiskname, s32& hdisk);


/*====================================================================
������      : vbfsapp_closedisk
����        : �رմ���
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN] hdisk -- ��Ҫ�رյĴ��̾��
����ֵ˵��  :  0 -- �ɹ��� ���� -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2011/01/05             linlifen         ����
====================================================================*/
s32 vbfsapp_closedisk(const s32 hdisk);


/*====================================================================
������      : vbfsapp_scandir
����        : ɨ�����
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN]  psdir -- ��Ҫɨ���Ŀ¼
               [OUT] namelist -- ɨ��·���µ�·�����ļ��б�
			   [OUT] nNameNum -- ·�����ļ�����
����ֵ˵��  :  0 -- �ɹ��� ���� -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2011/01/05             linlifen         ����
====================================================================*/
s32 vbfsapp_scandir(const s8* psdir, struct vbfsapp_direct ***namelist, s32& nNameNum);


/*====================================================================
������      : vbfsapp_fstat
����        : ��ȡ�ļ�������Ϣ������ʱ�䣬����ʱ�䣬�ļ���С��
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN]  pszfilename -- �ļ���
               [OUT] vstat       -- �ļ���Ϣ
����ֵ˵��  :  0 -- �ɹ��� ���� -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2011/01/05             linlifen         ����
====================================================================*/
s32 vbfsapp_fstat(const s8* pszfilename, struct vbfsapp_stat *vstat);


/*====================================================================
������      : vbfsapp_getextinfo
����        : ��ȡ�ļ���չ��Ϣ��Ϣ���ļ���ʽ��������Ϣ�ȣ�
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN]  pszfilename -- �ļ���
               [OUT] fextinfo    -- �ļ���չ��Ϣ
����ֵ˵��  :  0 -- �ɹ��� -1 -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2011/01/05             linlifen         ����
====================================================================*/
s32 vbfsapp_getextinfo(const s8* pszfilename, struct vbfsapp_fextinfo *fextinfo);

#endif