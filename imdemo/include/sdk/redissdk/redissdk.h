
#ifndef _REDISSDK_H
#define _REDISSDK_H

#include <string>
#include <map>
#include <set>
#include <vector>
using namespace std;

#include "cms/cms_errorcode.h"
#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/module_struct.h"
#include "sdk/redissdk/switch_struct.h"
#include "sdk/redissdk/nru_struct.h"
#include "sdk/redissdk/partition_struct.h"
#include "sdk/redissdk/record_struct.h"
#include "sdk/redissdk/config_struct.h"
#include "sdk/redissdk/vtdu_struct.h"
#include "sdk/redissdk/cui_struct.h"
#include "sdk/redissdk/redissdkinner.h"

typedef string TRedisSdkOperType;
#define RedisSdkOperType_Set             ("set")           // ����ȫ������
#define RedisSdkOperType_Add             ("add")           // �������
#define RedisSdkOperType_Del             ("del")           // ɾ������

typedef string TRedisBalanceType;
#define RedisBalanceType_Num             ("num")           // ��·������
#define RedisBalanceType_Band            ("band")          // ���������

// ��������
class CFastDBInterface;


/*
 * todo by xiongxh
 *
 * RedisSdk�ṩ����Ҫ�������£�
 *     1. 
 *
 * ����������������δʵ�ֵ��Ż��㣺
 *     1. ���ݲ�ͬ�Ĺ��ܣ���key�ֲ��ڲ�ͬ�����ݿ�ռ��У�Ŀǰ���е�keyȫ�������ڵ�0�����ݿ��С�
 *        ���չ������ֿ�Ļ�������֮ǰfastdb�Ľӿڶ��Ǽ򵥵�get��set������ʵ�֣�û�����ݿ��ţ���ôΪ�˷�ֹ�л�
 *        �����������ݿ����ȥ������ǰ�Ľӿڣ����»�ȡ�������ݵ����⣬�������µ�ʵ����ÿ��ִ�н������л��ص�0
 *        �����ݿ�
 *     2. ģ��ע������������ˢ��ʱʧ�ܣ�����δ����Ƿ���Ҫһ�ֻ��ƽ������Ϣ֪ͨ��Ӧ�ò㣿
 *
 */
class CRedisSdk
{
public:
    CRedisSdk();
    virtual ~CRedisSdk();

public:
    typedef void (* RedisSdkCallback)(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3);

public:
    /*
     * ��ʼ���ͷ���ʼ���ӿ�
     * 
     * ע�⣺
     *     ��ʼ���ӿڣ����еĲ������£�
     *       1. ����redis
     *       2. ���Ĳ���ȡlua�ű���sha1ӳ����Ϣ���������ػ���Ľű�����sha1֮���ӳ���
     *
     *     ����ʼ���ӿڣ����еĲ������£�
     *       1. ģ������Ѿ�ע�ᣨ������RegisterModule�������Զ�ȡ��ע��
     *       2. ��redis�Ͽ�����
     *       3. ������صĻ�����Ϣ
     */
    ECMS_ERRORCODE Initialize(const string &strClientName, const string &strServer, u16 wPort, 
        const string &strPassword, RedisSdkCallback pfCallback, void *pvUserData);
    void Quit();

    /*
     * ����Fastdb��ʵ��
     *
     * ע�⣺
     *     1. RedisSdk���ڲ�����ͨ��fastdb������redis�ģ����ֹ������RedisSdkû�з�װ������ͨ����ȡfastdb��ʵ��������
     */
    CFastDBInterface *GetFastDBInstancePtr() { return &m_cFastDBInstance; }
    CFastDBInterface &GetFastDBInstance()    { return m_cFastDBInstance; }

    /*
     * ģ����Ƚӿ�
     * 
     * ע�⣺
     *     1. ģ����Ƚӿڷ�Ϊ���࣬һ��ֻ���ص����㷨ѡ���ģ����Ϣ����һ�໹������������Դid��
     *        ��Щ�������漰����Դ��صģ�ʹ��ǰ�ߣ�����Щ������������Դ���Ƶģ���Ҫʹ�ú���
     *        �������nru���������Ҫ����¼������¼����Դ�����޵ģ����Ե���ʱ��Ҫ������Դ�����ⳬ����Դ���ޣ�
     *        ���ֻ�ǽ���¼���ѯ�Ȳ������������û�����������ƣ�����ʹ�ò�����Դ�����ĵ��Ⱥ���
     *     2. ���ڴ���Դ�����Ľӿڣ����ȳɹ�����Դ������һ��ʱ�䣬Ȼ���Զ��ͷţ�����redis��expireʵ�֣���
     *        Ӧ�ò�Ҳ�����ֶ�����ReleaseResource�����ͷ�
     */
    ECMS_ERRORCODE SelectCui(const string &strSession, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectPui(const string &strDevId, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectCmu(string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectRcs(string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectNru(const TRedisModuleNruSelectCond &tCond, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectVtdu(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst, 
        string &strId, string &strDomainName, TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource *ptResource = NULL);
    ECMS_ERRORCODE SelectVtduEx(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst, 
        const string &strDstAddr, bool& bSameIpVtdu, string &strId, string &strDomainName, 
        TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource *ptResource = NULL);

    // ���롢��ȡ���ͷ�vtdu����Դ��ͨ�������㷨��ȡ����Դ����ʱ�޵ģ�����ʱ���Զ��ͷţ���������Զ��ͷ�ʱ��̫����Ҳ��ͨ��ReleaseVtduResource�����ͷţ�
    ECMS_ERRORCODE AllocVtduResource(const TRedisModuleResourceId &tOriginResourceId, TRedisModuleResourceId &tResourceId, 
        TRedisModuleVtduResource *ptResource = NULL);
    ECMS_ERRORCODE GetVtduResource(const TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource &tResource);
    ECMS_ERRORCODE ReleaseVtduResource(const TRedisModuleResourceId &tResourceId);

    // ����ģ�鸺�أ����ز�ͬ�����Ȳ��Բ�ͬ��
    ECMS_ERRORCODE UpdateModuleLoad(const string &strId, const TRedisModuleLoad &tLoad);

    // ����ģ���˽������
    ECMS_ERRORCODE CleanModulePrivateData(const TRedisModuleId &tId);

    // ��ȡģ����Ϣ
    ECMS_ERRORCODE GetModuleDetailInfo(const string &strId, TRedisModuleDetailInfo &tModuleDetailInfo);

    /*
     * ���Ľӿ�
     */
    ECMS_ERRORCODE UpdateSsChannel(const CRedisSsChannel &cSsChn, const TRedisSdkOperType &tType = RedisSdkOperType_Set);
    ECMS_ERRORCODE UpdateSsChannel(const set<CRedisSsChannel> &cSsList, const TRedisSdkOperType &tType = RedisSdkOperType_Set);

    /*
     * ����Fastdb�Ķ���ͨ����ʹ��Fastdb��RedisSdk�Ķ���ͨ����һ�µĴ��������������ĺ��壬�����벻Ҫʹ�ã�
     */
    ECMS_ERRORCODE UpdateCompaSsChannel(const string &strSsChn, const TRedisSdkOperType &tType = RedisSdkOperType_Set);
    ECMS_ERRORCODE UpdateCompaSsChannel(const set<string> &cSsList, const TRedisSdkOperType &tType = RedisSdkOperType_Set);

    // ��ȡƽ̨ģ�������ߵĶ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisModuleStatus����Json���л������ݶ��ģ�
    set<CRedisSsChannel> GetModuleSsChannelById(const set<string> &cModuleId);
    CRedisSsChannel      GetModuleSsChannelById(const string &strModuleId)
    { 
        return CRedisSsChannel(RedisSsChannelType_Module_Status, strModuleId);
    }

    set<CRedisSsChannel> GetModuleSsChannelByType(const set<TRedisModuleType> &cTypes);
    CRedisSsChannel      GetModuleSsChannelByType(const TRedisModuleType &tType)
    { 
        return CRedisSsChannel(RedisSsChannelType_Module_Type_Status, tType);
    }

    // ��ȡƽ̨¼����ԵĶ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecCoverPolicy����Json���л������ݶ��ģ�
    CRedisSsChannel      GetPlatRecCoverPolicySsChannel()
    { 
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_Cover_Policy); 
    }

    // ��ȡƽ̨¼���������������Ķ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecNoStreamCheckParam����Json���л������ݣ�
    CRedisSsChannel      GetPlatRecNoStreamCheckParamSsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_No_Stream_Check_Param);
    }

    // ��ȡƽ̨¼���ļ��л����ԵĶ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecFileCutPolicy����Json���л������ݣ�
    CRedisSsChannel      GetPlatRecFileCutPolicySsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_File_Cut_Policy);
    }

    // ��ȡƽ̨¼���������Ķ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecReserveDays����Json���л������ݣ�
    CRedisSsChannel      GetPlatRecReserveDaysSsChannel()
    { 
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_Reserve_Days); 
    }

    // ��ȡ�豸¼��ʱ���Ķ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecDuration����Json���л������ݣ�
    set<CRedisSsChannel> GetPlatRecDurationSsChannel(const set<TRedisPuRecUnit> &cRecUnits);
    CRedisSsChannel      GetPlatRecDurationSsChannel(const TRedisPuRecUnit &tRecUnit)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Duration, tRecUnit.ToString());
    }

    // ��ȡ�豸ƽ̨¼��״̬�Ķ���ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisPlatRecStatus����Json���л������ݣ�
    set<CRedisSsChannel> GetPlatRecStatusSsChannelByRecUnit(const set<TRedisPuRecUnit> &cRecUnits);
    CRedisSsChannel      GetPlatRecStatusSsChannelByRecUnit(const TRedisPuRecUnit &tRecUnit)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, tRecUnit.ToString());
    }

    set<CRedisSsChannel> GetPlatRecStatusSsChannelByDevice(const set<string> &cDevId);
    CRedisSsChannel      GetPlatRecStatusSsChannelByDevice(const string &strDevId)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, strDevId);
    }

    set<CRedisSsChannel> GetPlatRecStatusSsChannelByDomain(const set<string> &cDomain);
    CRedisSsChannel      GetPlatRecStatusSsChannelByDomain(const string &strDomain)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, strDomain);
    }

    // ��ȡ�豸ƽ̨¼��״̬�Ķ���ͨ����֪ͨ�ص��е�pvParam2����Ϊvector<TRedisPartition>����Json���л������ݣ�
    set<CRedisSsChannel> GetPartitionSsChannel(const set<string> &cNruIds);
    CRedisSsChannel      GetPartitionSsChannel(const string &strNruId)
    {
        return CRedisSsChannel(RedisSsChannelType_Partition, strNruId);
    }

    // ��ȡnru�ı����Ϣ����ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisNruTopoNotify����Json���л������ݣ�
    CRedisSsChannel      GetNruTopoSsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Nru_Topo);
    }

    // ��ȡnru��ƽ̨״̬����ͨ����֪ͨ�ص��е�pvParam2����ΪTRedisNruPlatStatus����Json���л������ݣ�
    set<CRedisSsChannel> GetNruPlatStatusSsChannel(const set<string> &cNruIds);
    CRedisSsChannel      GetNruPlatStatusSsChannel(const string &strNruId)
    {
        return CRedisSsChannel(RedisSsChannelType_Nru_Plat_Status, strNruId);
    }

public:

    /*
     * cui�����ݽӿ�
     */
    ECMS_ERRORCODE CuOnline(const string &strCuiId, const TRedisCuAgent &tCuAgent);
    ECMS_ERRORCODE CuOffline(const string &strCuiId, const string &strSession);
	ECMS_ERRORCODE GetCuInfo(const string &strCuId, TRedisCuAgent &tCuAgent);

    ECMS_ERRORCODE UpdateCuiInfo(const string &strCuiId, const TRedisModuleCui &tCuiInfo);
    ECMS_ERRORCODE UpdateCuiOnlineMobileNum(const string &strCuiId, u32 dwOnlineMobileNum);

    ECMS_ERRORCODE SetPlatRecParam(const TRedisPuRecUnit &tRecUnit, const TRedisPlatRecParam &tRecParam, 
        bool bUpdateComplete = true, bool bAutoPublish = true);
    ECMS_ERRORCODE GetPlatRecParam(const TRedisPuRecUnit &tRecUnit, TRedisPlatRecParam &tRecParam);

    ECMS_ERRORCODE AddPlatRecTask(const TRedisPlatRecType &tRecType, const TRedisPuRecUnit &tRecUnit);
	ECMS_ERRORCODE DelPlatRecTask(const TRedisPlatRecType &tRecType, const TRedisPuRecUnit &tRecUnit);

	ECMS_ERRORCODE GetPlatRecTaskTypes(const TRedisPuRecUnit &tRecUnit, set<TRedisPlatRecType> &setRecType);

    ECMS_ERRORCODE UpdatePlatRecCoverPolicy(const TRedisPlatRecCoverPolicy &tPolicy, bool bAutoPublish = true);     // �����豸��ƽ̨¼�񸲸ǲ���
    ECMS_ERRORCODE UpdatePlatRecNoStreamCheckParam(u8 byTimes, u16 wInterval, bool bAutoPublish = true);            // ����ƽ̨¼��������������
    ECMS_ERRORCODE UpdatePlatRecFileCutPolicy(const TRedisPlatRecFileCutPolicy &tPolicy, bool bAutoPublish = true); // ����ƽ̨¼���ļ��л�����
    ECMS_ERRORCODE UpdatePlatRecGbBlindFlag(bool bFlag);                                                            // ����ƽ̨¼������豸ä¼��־
    ECMS_ERRORCODE UpdatePlatRecReserveDays(const TRedisPlatRecReserveDays &tReserveDays, 
        bool bAutoPublish = true);                                                                                  // ����ȫ��¼��������
    ECMS_ERRORCODE UpdatePlatRecDuration(const TRedisPuRecUnit &tRecUnit, 
        u16 wRecDuration, bool bAutoPublish = true);                                                                // �����豸��ƽ̨¼��ʱ��

    ECMS_ERRORCODE UpdatePlatRecPartitionBind(const string &strNruId, const string &strPartitionMountName, 
        const vector<TRedisPuRecUnit> &cRecUnits, const TRedisSdkOperType &tType = RedisSdkOperType_Set);           // ���̷�����
    ECMS_ERRORCODE GetPlatRecPartitionBind(const string &strNruId, const string &strPartitionMountName, 
        vector<TRedisPuRecUnitWithDomain> &cRecUnits);                                                              // ��ȡ���̷�����Ϣ

    // UpdatePlatRecBalanceTime����ӿڣ�Ŀǰû�����ã�nru����Ҫ��todo by xiongxh��
    ECMS_ERRORCODE UpdatePlatRecBalanceTime(const u16 wInterval, bool bAutoPublish = true);                         // ���¾���ʱ����
    
    /*
     * pui�����ݽӿ�
     */
    ECMS_ERRORCODE PuOnline(const string &strPuiId, const string &strDevId);
    ECMS_ERRORCODE PuOffline(const string &strPuiId, const string &strDevId);

    /*
     * vtdu�����ݽӿ�
     */
    ECMS_ERRORCODE UpdateVtduInfo(const string &strVtduId, const TRedisModuleVtdu &tVtduInfo);                      // ����vtdu��Ϣ

    /*
     * rcs�����ݽӿ�
     */
    ECMS_ERRORCODE GetNruListByPlatRecUnit(const TRedisPuRecUnit &tRecUnit, vector<TRedisUri> &cNruList);

    ECMS_ERRORCODE AddNru(const string &strNruId, bool bEnable, const TRedisModuleNruBaseInfo &tTopInfo, 
        bool bAutoPublish = true);                                                                                  // ����nru
    ECMS_ERRORCODE ModNru(const string &strNruId, bool bEnable, const string &strNruName, 
        bool bAutoPublish = true);                                                                                  // �޸���������nru��Ϣ
    ECMS_ERRORCODE DelNru(const string &strNruId, bool bAutoPublish = true);                                        // ����nru
    ECMS_ERRORCODE ClearPuPlatRecConfig(const string &strPuId);                                         //����redis�е�pu������Ϣ(partition-map-rec-unit:xxx && plat-rec-param:xxx:x)

    /*
     * nru�����ݽӿ�
     */
    ECMS_ERRORCODE GetNruPlatStatus(const string &strNruId, TRedisNruPlatStatus &tStatus);                          // ��ȡNru��ƽ̨״̬���Ƿ��������Ƿ����ã�                                                                              // ����nru��������з�����Ϣ
    ECMS_ERRORCODE UpdatePartitionInfo(const string &strNruId, const vector<TRedisPartition> &cPartitionInfo, 
        bool bAutoPublish = true);                                                                                  // ����nru��������з�����Ϣ
    ECMS_ERRORCODE UpdateNruInfo(const string &strNruId, const TRedisModuleNru &tNruInfo);                          // ����nru������Ϣ
    ECMS_ERRORCODE UpdateNruLoad(const string &strNruId, const TRedisModuleNruLoad &tLoad);                         // ����nru������Ϣ
    ECMS_ERRORCODE UpdateNruStreamLimit(const string &strNruId, const TRedisModuleNruStreamLimit &tStreamLimit);    // ����nru����������Ϣ
    ECMS_ERRORCODE UpdateNruStreamCur(const string &strNruId, const TRedisModuleNruStreamCur &tStreamCur);          // ����nru��ǰ��������Ϣ
    ECMS_ERRORCODE UpdateNruStreamRec(const string &strNruId, u16 wNum, u64 qwBandWidth);                           // ����nru��ǰ��¼����Ϣ
    ECMS_ERRORCODE UpdateNruStreamPlay(const string &strNruId, u16 wNum, u64 qwBandWidth);                          // ����nru��ǰ�ķ�����Ϣ
    ECMS_ERRORCODE UpdateNruStreamDownload(const string &strNruId, u16 wNum, u64 qwBandWidth);                      // ����nru��ǰ��������Ϣ
    ECMS_ERRORCODE UpdateNruRecordInfo(const string &strNruId, const TRedisModuleNruRecordInfo &tRecordInfo, 
        const TRedisSdkOperType &tType = RedisSdkOperType_Set);                                                     // ����nru��¼��ǰ�˵���Ϣ
    
    ECMS_ERRORCODE JoinPlatRecSubsystem(const string &strNruId);                                                    // nru����ƽ̨¼����ϵͳ
    ECMS_ERRORCODE GetPlatRecTask(const string &strNruId, u32 dwExpectedNum, vector<TRedisPlatRecTask> &cRecTasks, 
        u32 &dwRestSize, u32 &dwUnassignedSize);
    ECMS_ERRORCODE GetPlatRecTaskParam(const TRedisPuRecUnit &tRecUnit, TRedisPlatRecTaskParam &tTaskParam, 
        string &strChnState, string &strVideoEncodeParam, string &strAudioEncodeParam);                             // nru�ڴ���¼������ʱ��ȡ�������ز��������豸�Ƿ����ߵȣ�
    ECMS_ERRORCODE ReassignPlatRecTask(const string &strNruId, u32 dwExpectedNum, u32 &dwReassignNum, 
        u32 &dwExpiredNum, u32 &dwRestSize);                                                                        // nru����unassigned��������ȥ

    ECMS_ERRORCODE TransferPlatRecTask(const set<TRedisPuRecUnit> &cRecUnits);                                      // nru����¼��������ת��unassigned
    ECMS_ERRORCODE TransferNruPlatRecTask(const string &strNruId);                                                  // nru����¼��������ת��unassigned
    ECMS_ERRORCODE TransferPartitionPlatRecTask(const string &strNruId, const string &strPartitionMountName);       // nru����ָ�������ϵ�������ת��unassigned

    // ��Ҫ����¼������ļ������
    // 1. �󶨷������� 2. ����󶨷��� 3. ������δʵ�֣���������Ҫ��
    ECMS_ERRORCODE AdjustPlatRecTask(const TRedisPuRecUnit &tRecUnit, 
        string &strNewPartitionName, string &strNruId);                                                             // ����¼������

    ECMS_ERRORCODE ClearPlatRecTask(const set<TRedisPuRecUnit> &cRecUnits);                                         // ����¼�������������
    ECMS_ERRORCODE RemoveExpiredPlatRecTask(const string &strNruId, const TRedisPuRecUnit &tRecUnit);               // �Ƴ����ڵ�¼������
    
    ECMS_ERRORCODE BalancePlatRecTask(TRedisPuRecUnit & recUnit, string &oldNruId, string &newNruId, string &oldPartitionName, string &newPartitionName);   // �������¼�������ؾ���
    ECMS_ERRORCODE GetNruPlatRecTask(const string &strNruId, set<TRedisPuRecUnit> &cRecUnits);                      // ��ȡָ��nru������¼������

    ECMS_ERRORCODE UpdatePlatRecStatus(const string &strNruId, const TRedisPlatRecStatus &tRecStatus, 
        bool bBindRecordToNru = false, bool bAutoPublish = true);                                                   // �����豸��ƽ̨¼��״̬


    /*
     * proxy�ӿ�
     */
    ECMS_ERRORCODE RegisterModule(const TRedisModule &tModule, const TRedisModuleState &tState = RedisModuleState_Start);   // ע��ģ��
    ECMS_ERRORCODE UnregisterModule(const string &tModuleId, const TRedisModuleState &tState = RedisModuleState_Stop);      // ȡ��ע��ģ��

    /*
     * ͨ�����ݷ��ʽӿ�
     */
    ECMS_ERRORCODE GetPlatRecTaskAmount(u32 &dwAmount);                                                             // ��ȡƽ̨����¼��·��
    ECMS_ERRORCODE GetPlatSwitchAmount(u32 &dwAmount);                                                              // ��ȡƽ̨����·��
    ECMS_ERRORCODE GetPlatRecCoverPolicy(TRedisPlatRecCoverPolicy &tPolicy);                                        // ��ȡ�豸��ƽ̨¼�񸲸ǲ���
    ECMS_ERRORCODE GetPlatRecNoStreamCheckParam(u8 &byTimes, u16 &wInterval);                                       // ��ȡƽ̨¼��������������
    ECMS_ERRORCODE GetPlatRecFileCutPolicy(TRedisPlatRecFileCutPolicy &tPolicy);                                    // ��ȡƽ̨¼���ļ��л�����
    ECMS_ERRORCODE GetPlatRecDuration(const TRedisPuRecUnit &tRecUnit, u16 &wRecDuration);                          // ��ȡ�豸��ƽ̨¼��ʱ��
    ECMS_ERRORCODE GetPlatRecReserveDays(TRedisPlatRecReserveDays &tReserveDays);                                   // ��ȡ�豸��ȫ��¼��������
    ECMS_ERRORCODE GetPlatRecStatusByDevice(const string &strDevId, TRedisDevicePlatRecStatus &tDeviceRecStatus);   // ��ȡ�豸��ƽ̨¼��״̬
    ECMS_ERRORCODE GetNruList(vector<TRedisModuleNruDetailInfo> &cDetailInfo, 
        bool bInTopo = true, bool bNotInTopo = true);                                                               // ��ȡ��������δ����nru�Ļ�����Ϣ
    ECMS_ERRORCODE GetNruDetailInfo(const string &strNruId, TRedisModuleNruDetailInfo &tDetailInfo);                // ��ȡ����nru����ϸ��Ϣ
    ECMS_ERRORCODE GetPartitionInfo(const string &strNruId, vector<TRedisPartition> &cPartitionInfo);               // ��ȡָ��nru��صķ�����Ϣ
    ECMS_ERRORCODE GetPlatRegisterModule(set<string> &cModules);                                                    // ��ȡƽ̨������ע��ģ��
    ECMS_ERRORCODE GetPlatRegisterModuleByType(const TRedisModuleType &tModuleType, set<string> &cModules);         // ��ȡƽָ̨�����͵���ע��ģ��

    // todo by xiongxh ��ʱ����Ҫ���ű�û��ʵ��
    ECMS_ERRORCODE GetDomainMap(map<string, string> &cDomainMap);                                                   // ��ȡƽ̨���ϵ��Ϣ

    /*
     * ��ʱ�ӿڣ�Ŀǰ��v2r3b3�汾�����еģ������汾���ܻ�ɾ������Ҫ�����¾ɽ�����������������ã�
     */
    TRedisSwitchFlow GetSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst);
    TRedisSwitchFlow GetSrcSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc);
    TRedisSwitchFlow GetDstSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tDst);

public:

    void PrintData() const;

public:

    /*
     * Fastdb��Ҫ�Ļص��������
     *
     *     1. fastdb����Ҫ������״̬�ص�
     *     2. fastdb����Ҫ�Ķ��Ļص�����
     */
    static void FastdbStatusCallback(CFastDBInterface *pcInst, EFDB_StatusCode emCode, void *, void *pvUserData);
    static void FastdbSubscribeCallback(CFastDBInterface *pcInst, const string &strChannel, const string &strContent, void *pvUserData);
    static void FastdbCompaSubscribeCallback(CFastDBInterface *pcInst, const string &strChannel, const string &strContent, void *pvUserData);

    // Fastdb�ص�ʵ��ִ��ҵ��ĺ���
    void FastdbDisconnectProc(EFDB_StatusCode emCode);
    void FastdbNotifyProc(const string &strChannel, const string &strContent);              // RedisSdk�еĶ���ͨ���Ļص�����
    void FastdbCompaNotifyProc(const string &strChannel, const string &strContent);         // Fastdb�ж���Ķ���ͨ���Ļص�����

protected:

    /*
     * lua�ű�����sha1֮���ӳ��ת����
     */
    string          LuaScript2Sha(const string &strScript) const;
    ECMS_ERRORCODE  LoadLua2Sha();

    /*
     * �ڲ�����ִ��lua�ű��ĺ���
     */
    ECMS_ERRORCODE ExecuteLuaByScriptContent(const string &strLuaScript, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptContent(const string &strLuaScript, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);
    ECMS_ERRORCODE ExecuteLuaByScriptFile(const string &strFile, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptFile(const string &strFile, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);
    ECMS_ERRORCODE ExecuteLuaByScriptSha(const string &strSha, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptSha(const string &strSha, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);

    /*
     * CRedisSsChannel���ַ���֮���ת��
     */
    ECMS_ERRORCODE RedisSsChannel2String(const set<CRedisSsChannel> &cSsChn, set<string> &cChnString);
    ECMS_ERRORCODE RedisSsChannel2String(const set<CRedisSsChannel> &cSsChn, vector<string> &cChnString);

    /*
     * �ж�ָ���Ķ���ͨ����App�����Sdk���Ƿ���
     */
    bool IsAppSsChn(const CRedisSsChannel &cSsChn) const { return (m_cAppSsList.end() != m_cAppSsList.find(cSsChn)); }
    bool IsSdkSsChn(const CRedisSsChannel &cSsChn) const { return (m_cSdkSsList.end() != m_cSdkSsList.find(cSsChn)); }

    /*
     * RedisSdk�ڲ������Ļص��ķ���
     */
    void ProcInnerSubscribeNotify(const CRedisSsChannel &cSsChn, const string &strContent);

private:
    CFastDBInterface  m_cFastDBInstance;               // RedisSdk�ǻ���fastdb���ض�ҵ��ķ�װ��
    RedisSdkCallback  m_pfCallback;                    // �ص�������RedisSdk��Ҫ����֪ͨӦ�ò��ʱ�����
    void *m_pvUserData;                                // �û�����ָ�룬��ʼ����ʱ�����ϲ����ã��ص���ʱ����ΪpvParam1���ظ��ϲ�

    set<CRedisSsChannel> m_cSdkSsList;                 // RedisSdk�ڲ��Ķ���ͨ���б�
    set<CRedisSsChannel> m_cAppSsList;                 // Ӧ�ò�Ķ���ͨ���б�
    set<string>          m_cAppCompaSsList;            // Ӧ�ò�ļ��ݶ���ͨ���б�

    /*
     * ���ã�����ű�����sha1�����ӳ�䣨key��lua�ű�����value��sha1���룩
     * 
     * ע�⣺
     *     Ϊ�˾��������redis��lua�ű������ܣ�ʹ��evalsha��������eval����ִ��lua�ű���
     * ����sha1�����lua�ű������йأ�ֻҪlua�ű����ݱ��������ֻ��һ���ո񣬶�Ӧ��sha1���붼���ı䣩��
     * ���Գ����в�����ֱ��ʹ��lua�ű���sha1���롣Ϊ��ʹlua�ű��߱���̬�������������޸�lua�ű�����Ҫ�����������redis-server����
     * �������»��ƣ�
     *     1. lua�ű���ģ�������룬lua�ű����ı��ļ���ʽ������ƽ̨��
     *     2. ÿһ��lua�ű���Ӧһ������Ĺ��ܵ㣬����ѡ��vtdu���ܣ���Ӧ��lua�ű���Ϊ��select-vtdu.lua
     *     3. redis-server�����󣬽�ƽ̨�����е�lua�ű����ص�redis�У����ɶ�Ӧ��sha1���룬���浽redis�в�������
     *        ����select-vtdu.lua��ʹ��script load��������ݼ��ص�redis�У����ɽű��ļ�select-vtdu.lua��sha1����xxxxxx��ӳ�䣬������
     *     4. ģ����������RedisSdk�������ӳ���ϵ��Ȼ���ȡ���������ӳ���ϵ�ĳ�ʼ���ݣ����浽m_cLua2Sha��
     *     5. ��������Ҫ����ĳ��lua�ű�ʱ��RedisSdkֻ��Ҫʹ�ýű��ļ������ɡ�
     *        ����Ҫ����select-vtdu.lua��������д������Ҫִ�еĽű���select-vtdu.lua��Ȼ����RedisSdk���ű���ӳ��Ϊsha1���룬Ȼ��ͨ��evalsha
     *        ����ִ�м��ɡ�����Ҫ��ʱ����vtdu��ѡ���㷨ʱ��ֻҪ�޸�select-vtdu.lua�ű���Ȼ��ʹ��script load��������µĽű����ٸ���
     *        �ű�������sha1��ӳ���ϵ����������ȥ������ģ�����ͨ��RedisSdk�Զ���֪���µ�select-vtdu.lua��Ӧ��sha1����
     */
    map<string, string> m_cLua2Sha;

    // ���ڲ�ʹ�õĳ�������(��Ҫ���ڲ�Ҫ�õ�������lua�ű�������)
    static const string m_cstrScriptRegisterModule;
    static const string m_cstrScriptUnregisterModule;
    static const string m_cstrScriptSelectCmu;
    static const string m_cstrScriptSelectCui;
    static const string m_cstrScriptSelectPui;
    static const string m_cstrScriptSelectRcs;
    static const string m_cstrScriptSelectVtdu;
    static const string m_cstrScriptSelectVtduEx;
    static const string m_cstrScriptSelectNru;
    static const string m_cstrScriptUpdateModuleLoad;
    static const string m_cstrScriptCleanModulePrivateData;
    static const string m_cstrScriptGetModuleDetailInfo;
    static const string m_cstrScriptAllocVtduResource;
    static const string m_cstrScriptGetVtduResource;
    static const string m_cstrScriptReleaseVtduResource;
    static const string m_cstrScriptUpdatePartitionInfo;
    static const string m_cstrScriptUpdateNruInfo;
    static const string m_cstrScriptUpdateNruLoad;
    static const string m_cstrScriptUpdateNruStreamLimit;
    static const string m_cstrScriptUpdateNruStreamCur;
    static const string m_cstrScriptUpdateNruStreamRec;
    static const string m_cstrScriptUpdateNruStreamPlay;
    static const string m_cstrScriptUpdateNruStreamDownload;
    static const string m_cstrScriptUpdateNruRecordInfo;
    static const string m_cstrScriptUpdatePlatRecCoverPolicy;
    static const string m_cstrScriptUpdatePlatRecNoStreamCheckParam;
    static const string m_cstrScriptUpdatePlatRecBalanceTime;
    static const string m_cstrScriptUpdatePlatRecGbBlindFlag;
    static const string m_cstrScriptUpdatePlatRecReserveDays;
    static const string m_cstrScriptUpdatePlatRecFileCutPolicy;
    static const string m_cstrScriptUpdatePlatRecDuration;
    static const string m_cstrScriptUpdatePlatRecPartitionBind;
    static const string m_cstrScriptGetPlatRecPartitionBind;
    static const string m_cstrScriptUpdatePlatRecStatus;
    static const string m_cstrScriptReassignPlatRecTask;
    static const string m_cstrScriptTransferPlatRecTask;
    static const string m_cstrScriptClearPlatRecTask;
    static const string m_cstrScriptRemoveExpiredPlatRecTask;
    static const string m_cstrScriptTransferNruPlatRecTask;
    static const string m_cstrScriptTransferPartitionPlatRecTask;
    static const string m_cstrScriptAdjustPlatRecTask;
    static const string m_cstrScriptBalancePlatRecTask;
    static const string m_cstrScriptGetNruPlatRecTask;
    static const string m_cstrScriptGetPlatRecCoverPolicy;
    static const string m_cstrScriptGetPlatRecNoStreamCheckParam;
    static const string m_cstrScriptGetPlatRecFileCutPolicy;
    static const string m_cstrScriptGetPlatRecDuration;
    static const string m_cstrScriptGetPlatRecReserveDays;
    static const string m_cstrScriptGetPlatRecStatusByDevice;
    static const string m_cstrScriptJoinPlatRecSubsystem;
    static const string m_cstrScriptSetPlatRecParam;
    static const string m_cstrScriptGetPlatRecParam;
    static const string m_cstrScriptAddPlatRecTask;
	static const string m_cstrScriptDelPlatRecTask;
	static const string m_cstrScriptGetPlatRecTaskTypes;
    static const string m_cstrScriptGetPlatRecTask;
    static const string m_cstrScriptGetNruPlatStatus;
    static const string m_cstrScriptGetPlatRecTaskParam;
    static const string m_cstrScriptUpdateVtduInfo;
    static const string m_cstrScriptGetPlatSwitchAmount;
    static const string m_cstrScriptGetPlatRecTaskAmount;
    static const string m_cstrScriptCuOnline;
    static const string m_cstrScriptCuOffline;
	static const string m_cstrScriptGetCuInfo;
    static const string m_cstrScriptPuOnline;
    static const string m_cstrScriptPuOffline;
    static const string m_cstrScriptGetNruListByPlatRecUnit;
    static const string m_cstrScriptAddNru;
    static const string m_cstrScriptModNru;
    static const string m_cstrScriptDelNru;
    static const string m_cstrScriptClearPuPlatRecConfig;
    static const string m_cstrScriptGetNruList;
    static const string m_cstrScriptGetNruDetailInfo;
    static const string m_cstrScriptGetPartitionInfo;
    static const string m_cstrScriptUpdateCuiInfo;
    static const string m_cstrScriptUpdateCuiOnlineMobileNum;
    static const string m_cstrScriptGetSwitchFlow;
    static const string m_cstrScriptGetPlatRegisterModule;
    static const string m_cstrScriptGetPlatRegisterModuleByType;
    static const string m_cstrScriptGetDomainMap;
};



#endif

