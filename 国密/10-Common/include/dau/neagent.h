#ifndef _NE_AGENT_H_
#define _NE_AGENT_H_

#include "daustruct.h"

#ifdef _MSC_VER
#ifdef __cplusplus
#define NE_API               extern "C" __declspec(dllexport)
#else
#define NE_API               __declspec(dllexport)
#endif //__cplusplus
#else
#ifdef __cplusplus
#define NE_API               extern "C"
#else
#define NE_API
#endif //__cplusplus
#endif

enum EM_TOPO_TYPE
{
    TOPO_SYNC = 0,	
    TOPO_ADD,		// �����豸
    TOPO_DEL,		// ɾ��
    TOPO_MOD		// �豸�޸�
};

//���������ϻص�����
typedef void (*CBConnectedNtf)();

//ע����Ӧ�ص�����
typedef void (*CBRegisterRsp)(int dwSerialNO, TNEReportRegisterRsp tRegisterRsp);

//Get�ص�����
typedef void (*CBGetValue)(const char* pszID, int dwSerialNO, const char* pszInfo, int dwInfoLen);

//set�ص�����
typedef void (*CBSetValue)(const char* pszID, int dwSerialNO, const char* pszInfo, int dwInfoLen);

//��ȡtopo�ص�����
typedef void (*CBGetTopo)(int dwSerialNO, const char* pszNEID);

//�����ܶ����ص�����
typedef void (*CBDisconnectNtf)();

//�����ܶ����ص�����
typedef void (*CBDisconnectNENtf)(int dwSerialNO, const char* pszInfo, int dwInfoLen);

//����filebeat�ص�����
typedef void (*CBConfigFileBeatReq)(int dwSerialNO, const char* pszInfo, int dwInfoLen);

struct TMainTopo
{
public:
    TMainTopo()
    {
        m_szNEID = "";
        m_szNEName = "";
        m_szDesc = "";
        m_szMainType = "";
        m_szSubType = "";
        m_szManuFactor = "";
    }
public:
    std::string          m_szNEID;
    std::string          m_szNEName;
    std::string          m_szDesc;
    std::string          m_szMainType;
    std::string          m_szSubType;
    std::string          m_szManuFactor;
};

struct TInitParam
{
public:
    TInitParam()
    {
        pfConnectedCB = NULL;
        pfRegisterRspCB = NULL;
        pfGetValueCB = NULL;
        pfSetValueCB = NULL;
        pfGetTopoCB = NULL;
        pfDisconnCB = NULL;
        pfDisconnNECB = NULL;

        m_vecTopoList.clear();
        m_vecCapacity.clear();
        m_strProtocol.clear();
    }
public:
    CBConnectedNtf      pfConnectedCB;      //�������ܳɹ��ص�
    CBRegisterRsp       pfRegisterRspCB;    //ע����Ӧ�ص�
    CBGetValue          pfGetValueCB;       //GetValue�ص�
    CBSetValue          pfSetValueCB;       //SetValue�ص�
    CBGetTopo           pfGetTopoCB;        //GetTopo�ص�
    CBDisconnectNtf     pfDisconnCB;        //�����ܶ����ص�
    CBDisconnectNENtf   pfDisconnNECB;      //�������������豸�ص�
    CBConfigFileBeatReq pfConfigFileBeatCB; //����filebeat�ص�

    std::vector<TMainTopo>   m_vecTopoList;  // ��ǰtopo�б�
    std::vector<std::string> m_vecCapacity;  // ��ǰAgent���е�������

    std::string             m_strProtocol;  //Э��
};



/*====================================================================
�� �� ��: InitNeAgent
��    ��: ��ʼ��NEAgent����ע��ص�����(һ������ֻ�ɵ���һ��)
�㷨ʵ��: 
ȫ�ֱ���:
��    ��: const TInitParam* ptInitParam   ��ʼ��������Я������ص�����

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int InitNeAgent(const TInitParam* ptInitParam);

/*====================================================================
�� �� ��: InitFileBeatPath
��    ��: ��ʼ��filebeat�İ�װ·��
�㷨ʵ��: 
ȫ�ֱ���:
��    ��: const char *pszPath filebeat�ľ��԰�װ·��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int InitFileBeatPath(const char *pszPath);

/*====================================================================
�� �� ��: RegistReq
��    ��: ע������ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: u32 dwSerialNO              ��ˮ��
          TNEReportRegisterReq tReq   ע��������Ϣ��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int RegistReq(u32 dwSerialNO, TNEReportRegisterReq tReq);

/*====================================================================
�� �� ��: SetValueRsp
��    ��: SetValue��Ӧ�ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: const char* pszID       ��ԪID
          u32 dwSerialNO          ��ˮ�� 
          u32 dwErrCode           ������
          const char* pszInfo     ��Ӧ��Ϣ��
          int dwInfoLen           ��Ϣ�峤��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int SetValueRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, const char* pszInfo, int dwInfoLen);

/*====================================================================
�� �� ��: GetValueRsp
��    ��: GetValue��Ӧ�ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: const char* pszID       ��ԪID
          u32 dwSerialNO          ��ˮ�� 
          u32 dwErrCode           ������
          const char* pszInfo     ��Ӧ��Ϣ��
          int dwInfoLen           ��Ϣ�峤��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int GetValueRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, const char* pszInfo, int dwInfoLen);

/*====================================================================
�� �� ��: AlarmReport
��    ��: �澯�ϱ��ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: const char* pszID                 ��ԪID
          u32 dwSerialNO                    ��ˮ�� 
          TNEAlarmReport tAlarmReport       ��Ϣ��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int AlarmReport(const char* pszID, u32 dwSerialNO, TNEAlarmReport tAlarmReport);

/*====================================================================
�� �� ��: GetTopoRsp
��    ��: GetTopo��Ӧ�ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: const char* pszID                 ��ԪID
          u32 dwSerialNO                    ��ˮ�� 
          TNEResult tResult                 ��Ϣ��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int GetTopoRsp(const char* pszID, u32 dwSerialNO, TNEResult tResult);

/*====================================================================
�� �� ��: TopoReport
��    ��: topo�ϱ��ӿ�
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: const char* pszID                 ��ԪID
          u32 dwSerialNO                    ��ˮ�� 
          EM_TOPO_TYPE emTopoType           �ϱ�����
          TNEReportTopoInfoNty tTopoInfoNty ��Ϣ��

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int TopoReport(const char* pszID, u32 dwSerialNO, EM_TOPO_TYPE emTopoType, TNEReportTopoInfoNty tTopoInfoNty);

/*====================================================================
�� �� ��: ConfigFileBeatRsp
��    ��: ����filebeat��Ӧ
�㷨ʵ��: 
ȫ�ֱ���: 
��    ��: u32 dwSerialNO                    ��ˮ�� 
          u32 dwErrCode           ������

�� �� ֵ: �ɹ����� 0; ���� ���ط�0
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
====================================================================*/
NE_API int ConfigFileBeatRsp(int dwSerialNO, u32 dwErrCode);

#endif //_NE_AGENT_H
