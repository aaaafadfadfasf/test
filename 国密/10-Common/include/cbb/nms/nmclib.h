/*****************************************************************************
   �ļ�ʵ�ֹ���: ���ܿͻ��˿�Ľӿڷ�װ������ƽ̨������Ԫ
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/03/30   1.0        huzhiyun        ����
******************************************************************************/
#ifndef _NMCLIB_H_
#define _NMCLIB_H_

#include "kdvtype.h"
#include <string>
using std::string;
#include "snmp_msg.h"

class CNmcLib
{
public:
    CNmcLib();
    virtual ~CNmcLib();
    
    // strNmcId��Ԫ��ţ�ģ���uuid��û��uuid��ʹ��ģ����������cmu����strNmcSubType��Ԫ������,strNmcName��Ԫ���ƣ�һ����������ͬ��
    // strNmsIP���ܷ�����IP��wNmsPort���ܷ�����Port, strNmcIP��ԪIP
    bool Init(const string &strNmcId, const TNmcSubType &strNmcSubType, const string &strNmcName, 
              const string &strNmsIP, u16 wNmsPort, const string &strNmcIP);
    void Quit();

    // ����������Ϣ�ӿ�, dwMsgType��Ϣ��(�ο�snmp_msg.h), pMsgContentΪ��Ϣ�����ݣ�wMsgContentLenΪ��Ϣ�峤��
    bool SendMsg(u32 dwMsgType, const void *pMsgContent, u16 wMsgContentLen);
    bool HasConnectedNms() const;
    void EnableDebug();
    void DisableDebug();


    // dwMsgType����Ϣ��, bRWFlag: trueʱ��ʾ��ȡ�����ʾд��
    // (��ȡʱҵ�������������д������ͬʱ����pwBufLen��д��ʱҵ�����ӻ����ж�ȡ����)
    // pMsgContentΪ��Ϣ�建�棬pwMsgContentLenΪ��Ϣ�建�泤��
    virtual bool CBNmcMsg(u32 dwMsgType, bool bRWFlag, void *pMsgContent, u16 *pwMsgContentLen){return false;}
};

#endif // _NMCLIB_H_
