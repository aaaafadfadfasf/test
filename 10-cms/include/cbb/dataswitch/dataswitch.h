/**
* @file         dataswitch.h
* @brief        data switch
* @details      ����ת��
* @author       ������
* @date         2016/06/20
* @version      2016/06/20
* @par Copyright (c):
*    kedacom
* @par History:
*   version:
*/

#ifndef DATASWITCH_H        ///< ͷ�ļ���,��ֹ�ذ���
#define DATASWITCH_H        ///< ͷ�ļ���,��ֹ�ذ���

#define DSVERSION       "Dataswitch_64bit v1.1.0 2008.4.24" ///< �汾


#include "osp.h"

/** DataSwitch ��� */
#define DSID    u32

/** DataSwitch ����ֵ*/
#define DSOK    1
#define DSERROR 0

#define INVALID_DSID  0xffffffff   //<��Ч�ľ��ֵ


/**
 * ��ַ���ͽṹ
 * dwIP : IP��ַ,������
 * wPort : �˿�,������
 */
typedef struct
{
	u32 dwIP;
	u16 wPort;
}TDSNetAddr;

/**
 * UDP ���Ľ��ջص���������
 * pPackBuf : ���Ļ����ַ
 * wPackLen : ���ĳ���
 * ptDstAddr : ���ؽ��յ�ַ
 * ptSrcAddr : ����Դ��ַ
 * qwTimeStamp : ʱ���
 * dwContext : �û��ص�������
 */
typedef void* (*DSUDPPACKCALLBACK)(u8* pPackBuf, u16 wPackLen, TDSNetAddr* ptDstAddr, TDSNetAddr* ptSrcAddr, u64 qwTimeStamp, void* pvContext);


/**
 *@brief ���ý��ձ��Ļص�
 *@param[in]   const TDSNetAddr* ptRcvAddr ���յ�ַ
 *@param[in]   DSUDPPACKCALLBACK pUdpCallBack �ص�����
 *@param[in]   void* pvContext �ص�������
 *@return      DSOK:�ɹ� ������:ʧ��
 *@ref
 *@note
 */
API u32 dsRegRcvChannel(const TDSNetAddr* ptRcvAddr, DSUDPPACKCALLBACK pUdpCallBack, void* pvContext);


 /**
 *@brief �Ƴ����ձ��Ļص�
 *@param[in]   const TDSNetAddr* ptRcvAddr ���յ�ַ
 *@param[in]   DSUDPPACKCALLBACK pUdpCallBack �ص�����
 *@param[in]   void* pvContext �ص�������
 *@return      DSOK:�ɹ� ������:ʧ��
 *@ref
 *@note
 */
API u32 dsUnRegRcvChannel(const TDSNetAddr* ptRcvAddr, DSUDPPACKCALLBACK pUdpCallBack, void* pvContext);


/**
 *@brief ���ķ��ͺ���
 *@param[in]   pPackBuff : ���Ļ���
 *@param[in]   dwPackLen : ���ĳ���
 *@param[in]   ptSrcAddr : Դ��ַ
 *@param[in]   ptDstAddr : Ŀ�ĵ�ַ
 *@return      DSOK:�ɹ� ������:ʧ��
 *@ref
 *@note
 */
API s32 dsUDPPackSend(u8 *pPackBuff, u32 dwPackLen, TDSNetAddr* ptSrcAddr,TDSNetAddr* ptDstAddr );


/**
 * @func FilterFunc
 * @brief ���չ��˺���
 *
 * ÿ�����ս����һ�����˺��������˽����յ�UDP���ݰ�ʱִ�д˺�����
 * �����ݺ����ķ���ֵ������̬�ؾ����Ƿ�Դ����ݰ�ת����
 *
 * @param ptRecvAddr        - ���յ�ַ
 * @param ptSrcAddr         - Դ��ַ
 * @param pData          - [in, out]���ݰ��������޸ģ�
 * @param uLen           - [in, out]���ݰ����ȣ��޸ĺ�ĳ��Ȳ��ɳ������ֵ
 * @return DSOK, �����ݰ���Ч�� ����ֵ�������ݰ���Ч��
 */
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);


#define SENDMEM_MAX_MODLEN        (u8)32        ///<����ʱ���������޸ĳ���


/** �����鶨��*/
typedef struct tagNetSndMember
{
	TDSNetAddr  tDstAddr;               //<ת��Ŀ�ĵ�ַ
    s64         lIdx;                   //<�ӿ�����
    u32         errNum;                 //<�������
    s64         errNo;                  //<�����
    void *      lpuser;                 //<user info
    u16         wOffset;                //<�޸Ĳ��ֵ���ʼλ��; Ŀǰδʹ��
    u16         wLen;		            //<�޸Ĳ��ֵĳ��ȣ�����Ϊ4�ı�����Ϊ���ʾ���޸�
    s8          pNewData[SENDMEM_MAX_MODLEN]; //<�û��Զ�������
    void *      pAppData;                     //<�û��Զ���Ļص������Ĳ���
}TNetSndMember;


/**
 *@brief  �������ݻص�
 *@param[in]  u32 dwRecvIP  ������ip
 *@param[in]  u16 wRecvPort ������˿�
 *@param[in]  u32 dwSrcIP   Դip
 *@param[in]  u16 wSrcPort  Դ�˿�
 *@param[in]  TNetSndMember *ptSends    ת��Ŀ���б�
 *@param[in]  u16* pwSendNum            ת��Ŀ�����
 *@param[in]  u8* pUdpData      ���ݱ���
 *@param[in]  u32 dwUdpLen      ���ĳ���
 *@return
 *@ref
 *@see
 *@note
 */
typedef void (*SendCallback)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, TNetSndMember *ptSends,
                             u16* pwSendNum,
                             u8* pUdpData, u32 dwUdpLen);


/**
 *@brief ���ýӿڳ�ʱʱ��
 *@param[in]   s32 timeout : ʱ��,��λ:ms
 *@return
 *@ref
 *@note
 */
API void dsSetApiTimeOut(s32 timeout);


/**
 *@brief ����ds������
 *@param[in]   u32 dwMaxRcvGrp  ����������Ŀ
 *@param[in]   u32 dwMaxSndMmbPerRcvGrp  ����������������õ�ת������Ŀ
 *@return      DSOK:�ɹ� ������:ʧ�ܣ�
 *@ref
 *@see
 *@note
 */
API u32 dsSetCapacity( u32 dwMaxRcvGrp , u32 dwMaxSndMmbPerRcvGrp );


/**
 *@brief ����DataSwitchģ��
 *@param[in]   u32 num      �ӿ�IP�������
 *@param[in]   u32 adwIP[]  �ӿ�IP����
 *@param[in]   const char* pszSendIf  ָ���ķ����豸����
 *@return      DSID:�û�id,ʧ�ܷ���INVALID_DSID
 *@ref
 *@see
 *@note
 */
API DSID dsCreate( u32 num, u32 adwIP[], const char* pszSendIf = NULL );


/**
 *@brief ����DataSwitchģ��
 *@param[in]   dsId  - �û���ʶ
 *@return
 *@ref
 *@see
 *@note
 */
API void dsDestroy( DSID dsId );


/**
 *@brief ����ת��Ŀ��Ψһ��ת������
 *@param[in]   dsId         �û���ʶ
 *@param[in]   tRecvAddr    ���յ�ַ�����������ݰ���Ŀ��IP
 *@param[in]   dwInLocalIP  �������ݰ�������ӿ�
 *@param[in]   tSendToAddr  ת��Ŀ�ĵ�ַ
 *@param[in]   dwOutLocalIP ת�����ݰ����ñ���IP
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note ����Ѿ����ڵĽ���������ת��Ŀ���뵱ǰ�������ͬ������ɾ����Щ����
 */
API u32 dsAdd(DSID dsId, TDSNetAddr tRecvAddr, u32 dwInLocalIP, TDSNetAddr tSendToAddr, u32 dwOutLocalIP = 0);


/**
 *@brief ɾ��ת��Ŀ��Ϊָ����ַ��ת������
 *@param[in]   dsId         �û���ʶ
 *@param[in]   tSendToAddr  ת��Ŀ�ĵ�ַ
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemove(DSID dsId, TDSNetAddr tSendToAddr);


/**
 *@brief ����Dump����
 *@param[in]   dsId          �û���ʶ
 *@param[in]   tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]   dwInLocalIP   �������ݰ�������ӿ�
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note ��ָ����ַ���յ����ݰ�����ת�������һ�����յ�ַֻ��DUMP��������յ����ݰ�����ת����
 *      �����������ת������������������ת����
 */
API u32 dsAddDump(DSID dsId, TDSNetAddr tRecvAddr, u32 dwInLocalIP);


/**
 *@brief ɾ��Dump����
 *@param[in]   dsId          �û���ʶ
 *@param[in]   tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveDump(DSID dsId, TDSNetAddr tRecvAddr);


/**
 *@brief ���Ӷ��һ��ת������
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    dwInLocalIP   �������ݰ�������ӿ�
 *@param[in]    tSendToAddr   ת��Ŀ��IP��Port
 *@param[in]    dwOutLocalIP  ת�����ݰ����ñ���IP
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsAddManyToOne(DSID dsId , TDSNetAddr tRecvAddr, u32  dwInLocalIP, TDSNetAddr tSendToAddr, u32  dwOutLocalIP = 0);


/**
 *@brief ɾ������ת��Ŀ��Ϊָ����ַ�Ķ��һ����
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tSendToAddr   ת��Ŀ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAllManyToOne(DSID dsId , TDSNetAddr tSendToAddr);


/**
 *@brief ɾ��ָ���Ķ��һ����
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    tSendToAddr   ת��Ŀ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveManyToOne(DSID dsId , TDSNetAddr tRecvAddr, TDSNetAddr tSendToAddr);


/**
 *@brief ���Ӱ�Դת���Ĺ���
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    dwInLocalIP   �������ݰ�������ӿ�
 *@param[in]    tSrcAddr      �������ݰ���ԴIP��Port
 *@param[in]    tSendToAddr   ת��Ŀ��IP��Port
 *@param[in]    dwOutLocalIP  ת�����ݰ����ñ���IP
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note ʹ�ô˹���ʱ���������ݰ���Դ��ַ����������е�Դ
 *     ��ַ��ȡ�ÿ��ת��ʱ����Ҫ�ȸ������ݰ��е�Դ��ַ����ת����
 * ������Ҳ�������ʹ��Ĭ��Դ��ַ����0@0����ת������
 * ע�⣺�ýӿ�֧�ֶ�㵽һ�㡣���dwSrcIP��wSrcPort��Ϊ�㣬
 * ������ȫ��ͬ��dsAddManyToOne��
 */
API u32 dsAddSrcSwitch(DSID dsId , TDSNetAddr tRecvAddr, u32  dwInLocalIP, TDSNetAddr tSrcAddr, TDSNetAddr tSendToAddr, u32  dwOutLocalIP = 0);


/**
 *@brief ɾ������ָ���İ�Դת������
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    tSrcAddr      �������ݰ���ԴIP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAllSrcSwitch(DSID dsId, TDSNetAddr tRecvAddr, TDSNetAddr tSrcAddr);


/**
 *@brief ɾ��ָ���İ�Դת���Ĺ���
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    tSrcAddr      �������ݰ���ԴIP��Port
 *@param[in]    tSendToAddr   ת��Ŀ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveSrcSwitch(DSID dsId, TDSNetAddr tRecvAddr,  TDSNetAddr tSrcAddr,  TDSNetAddr tSendToAddr);


/**
 *@brief �����û�����
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tLocalAddr    ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    dwInLocalIP    �������ݰ�������ӿ�
 *@param[in]    tSrcAddr      �������ݰ���ԴIP��Port
 *@param[in]    tDstAddr      Ŀ��IP��Port
 *@param[in]    dwOutLocalIP  ת�����ݰ����ñ���IP
 *@param[in]    bSend       ���Ƿ���Ҫ����
 *@param[in]    pbyUserData   �û�����buffer
 *@param[in]    nUserLen      �û����ݳ���
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsSetUserData(DSID dsId,
					  TDSNetAddr tLocalAddr,
					  u32        dwLocalIfIp,
					  TDSNetAddr tSrcAddr,
					  TDSNetAddr tDstAddr,
					  u32        dwDstOutIfIP,
					  BOOL32      bSend,
                      u8* pbyUserData,
                      s32 nUserLen);


/**
 *@brief ���ù��˺���
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tRecvAddr     ���յ�ַ�����������ݰ���Ŀ��IP��Port
 *@param[in]    ptFunc        ���˺���ָ��
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note ����ָ�����Ϊ�գ���ʾ���ԭ�е����ã����⣬����
 * DataSwitch����������߳��е��ô˺����ģ�Ҫ���õĺ�����
 * ����ȫ�ֺ��������õĲ�����Ҳ������ȫ����Ч�ġ�
 */
API u32 dsSetFilterFunc(DSID dsId, TDSNetAddr tRecvAddr, FilterFunc ptFunc);


/**
 *@brief ɾ�����е�ת������
 *@param[in]    dsId          �û���ʶ
 *@param[in]    bKeepDump     �Ƿ������е�dump����Ĭ�ϲ�����
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAll( DSID dsId, BOOL32 bKeepDump = FALSE );


/**
 *@brief Ϊָ�����յ�ַ����ת�����ݰ�������Դ��ַ(���ڽ�����αװת��Դ)
 *@param[in]    dsId          �û���ʶ
 *@param[in]    tOrigAddr     ԭʼIP��Port
 *@param[in]    tMappedAddr   ӳ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsSpecifyFwdSrc(DSID dsId, TDSNetAddr tOrigAddr, TDSNetAddr tMappedAddr);


/**
 *@brief �ָ�ָ����ַת�����ݰ���Դ��ַ
 *@param[in]    dsId         �û���ʶ
 *@param[in]    tOrigAddr    ԭʼIP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsResetFwdSrc(DSID dsId, TDSNetAddr tOrigAddr);


/**
 *@brief �������ݱ���Ŀ�ĵ�ַ�ͽ��յ�ַ���ת����Դ��ַ(����ĳһת��ͨ��αװת��Դ)
 *@param[in]    dsId         �û���ʶ
 *@param[in]    tRcvAddr    ���յ�IP��Port
 *@param[in]    tDstAddr    Ŀ��IP��Port
 *@param[in]    tRawAddr    ӳ���IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsSetSrcAddrByDst(DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tDstAddr, TDSNetAddr tRawAddr);

/**
 *@brief �ָ����ڽ��յ�ַ��Ŀ�ĵ�ַ�����ݵ�Դ��ַαװ
 *@param[in]    dsId         �û���ʶ
 *@param[in]    tRcvAddr    ���յ�IP��Port
 *@param[in]    tDstAddr    Ŀ��IP��Port
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsResetSrcAddrByDst(DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tDstAddr);


/**
 *@brief ���÷��ͻص�����(���ڽ�����)
 *@param[in]    dsId         �û���ʶ
 *@param[in]    tRcvAddr    ���յ�IP��Port
 *@param[in]    tSrcAddr    ԴĿ��IP��Port
 *@param[in]    pfCallback  �ص�����
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsSetSendCallback( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, SendCallback pfCallback);


/**
 *@brief Ϊ����Ŀ������һ���Զ����ָ��
 *@param[in]    dsId         �û���ʶ
 *@param[in]    tRcvAddr    ���յ�IP��Port
 *@param[in]    tSrcAddr    ԴĿ��IP��Port
 *@param[in]    tDstAddr    ת��Ŀ��IP��ַ��Port
 *@param[in]    pAppData    �Զ���ָ��
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsSetAppDataForSend( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, TDSNetAddr tDstAddr, void * pAppData);


/**
 *@brief ��ѯ�����ܰ���
 *@param[in]    dsId            �û���ʶ
 *@param[in]    tRcvAddr        ���յ�IP��Port
 *@param[in]    tSrcAddr        ԴĿ��IP��Port
 *@param[in]    dwRecvPktCount  �����ܰ���
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsGetRecvPktCount( DSID dsId , TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, u32& dwRecvPktCount );


/**
 *@brief  ��ѯ�����ܰ���
 *@param[in]    dsId            �û���ʶ
 *@param[in]    tRcvAddr        ���յ�IP��Port
 *@param[in]    tSendToAddr     ת��Ŀ��IP��ַ��Port
 *@param[in]    dwSendPktCount  �����ܰ���
 *@return       DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ��
 *@ref
 *@see
 *@note
 */
API u32 dsGetSendPktCount( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, TDSNetAddr tSendToAddr, u32& dwSendPktCount);


/**
 *@brief  ��ѯ�������ֽ���
 *@return       ���ؽ������ֽ���
 *@ref
 *@see
 *@note
 */
API s64 dsGetRecvBytesCount( );


/**
 *@brief  ��ѯ�������ֽ���
 *@return       ���ط������ֽ���
 *@ref
 *@see
 *@note
 */
API s64 dsGetSendBytesCount( );


/**
 *@brief  ��ʾ���е�ת�����򣬼��������ڼ����Ķ˿�
 *@return
 *@ref
 *@see
 *@note
 */
API void dsinfo();


/**
 *@brief  ��ʾDataswitch�İ汾��Ϣ
 *@return
 *@ref
 *@see
 *@note
 */
API void dsver();


/**
 *@brief  ��ʾDataswitch���ṩ������İ�����Ϣ
 *@return
 *@ref
 *@see
 *@note
 */
API void dshelp();


/**
 *@brief        ��/�رյ�����Ϣ
 *@param[IN]    op    ָ�������û��������set, clear
 *@return
 *@ref
 *@see
 *@note
 */
API void dsdebug(const char* op = NULL);


/**
 *@brief        ��/�رո���һ��������Ϣ
 *@return
 *@ref
 *@see
 *@note �������ô˺�������Ϊ������Ϣ��Ӱ����������
 */
API void dsdebug2();


/**
 *@brief  ����ĳһ���������
 *@param[in]    dsId            �û���ʶ
 *@param[in]    pszLocalIP      ���յ�IP
 *@param[in]    wLocalPort      ���յ�port
 *@param[in]    dwTraceNum      ���е��ȴ���
 *@return
 *@ref
 *@see
 *@note
 */
API void dstrace(DSID dsId, s8* pszLocalIP, u16 wLocalPort, u32 dwTraceNum);

#endif
