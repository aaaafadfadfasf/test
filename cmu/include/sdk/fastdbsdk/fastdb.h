/************************************************************************
* �����ʿ������ݿ⣨Ŀǰredis���Ľӿڽ��з�װ��
* �������ģ����ͳһ�Ľӿ���������ݿ��ȡ��д
* ��ҵ����ص��ض����� ��
* 2016-02-25 zsy ����
* 2016-07-21 hzy ���Ȩ����ؽӿ�
* 2016-09-19 hzy ȫ���ӿڰ��չ��ܱ��ţ��޶��;���ע��
*
************************************************************************/

#pragma once
#include <string>
#include <map>
using std::multimap;
#include "keydef.h"
#include "system/hiredis/hiredis.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

struct TBatchItem
{
    std::string key;
    std::vector<std::string> fields;
    std::vector<std::string> values;
};

enum EFDB_ErrorCode
{
	EC_OK = 0,
	EC_FAILED,						//ʧ���ˣ��ӿڱ������ֻ�гɹ�ʧ�ܵ�ʱ��
	EC_DISCONNECTED,				//������
	EC_TIMEOUT,						//��ʱ��
	EC_AUTHEN_FAILED,				//��֤ʧ��	
	EC_NOT_IMPLEMENT,				//�ӿ�δʵ��
	EC_INVALID_PARAM,				//��������
	EC_NO_MATCH_MEMBER,				//redis�ֶ����޷��ҵ���Ӧ�Ľṹ���Ա
	EC_LOCKED,						//����
	EC_AGAIN,                       //δִ�н��������ڱȽϺ�ʱ��ǰ���أ�ʹ���߿��Լ������ã�
    EC_NOT_EXISTED,                 //���ݲ�����

	EC_NUM
};

inline bool IsFdbResultPositive( EFDB_ErrorCode err )
{
    return err == EC_OK;
}

enum EFDB_StatusCode
{
	SC_DISCONNECTED,						//������


	SC_NUM
};
class CFastDBInterface;
//���Ļص���������
typedef void(*SubscribeCallback)(CFastDBInterface * /*pInter*/, const std::string &/*strChnName*/, const std::string &/*strMsgData*/, void * /*userdata*/);
//redis����֪ͨ���������ܵ�״̬֪ͨ�ص���������(�����Ӧ��־��Ҫ��ӵģ�
typedef void(*RedisStatusCallback)(CFastDBInterface * /*pInter*/, EFDB_StatusCode /*sc*/, void * /*reserve*/, void * /*userdata*/);
typedef struct tagFDB_ConnInfo
{
	tagFDB_ConnInfo()
	{
		bAutoReconn = true;
		connTimeOutMs = (30 * 1000);
		accessTimeOutMs = (30 * 1000);
		pfStatusCallBack = NULL;
		pfStatusCallBack_userdata = NULL;
	}
	std::string clientname;	//�ͻ����Լ�ȡ�����ڱ�ʶ�Լ�������
	std::string username;	//����Ŀǰ��redis��˵��û���������Ԥ��
	std::string password;	//������趨����
	bool bAutoReconn;		//�Ƿ��Զ�����(�ù������ڶ��Ļ��ƵĴ��ڣ�Ŀǰ����Ч�������������֪ͨ�����ϲ������ô�����������Reconnect()��������)
	int connTimeOutMs;		//����ʱ�ĳ�ʱʱ�䵥λ����(<=0��ʾʹ��ϵͳĬ��)
	int accessTimeOutMs;	//��ȡ�����ĳ�ʱʱ�䵥λ����(<=0��ʾʹ��ϵͳĬ��)
	RedisStatusCallback pfStatusCallBack;//�����Ҫ����֪ͨ����������ص�����
	void *pfStatusCallBack_userdata;			//���ݸ�pfStatusCallBack�ص��������û�����
}FDB_ConnInfo;

class CFastDBInterface
{
public:
	friend void ThreadDisconnectedCallBack(void *pHolder, void *userdata);
	friend void ThreadSubscribeResultCallback(void *pHolder, const std::string &strChnName, bool bSuccess, void *userdata);	
	friend void ThreadUnSubscribeResultCallback(void *pHolder, const std::string &strChnName, bool bSuccess, void *userdata);
	friend void ThreadPublishMessageCallback(void *pHolder, const std::string &strChnName, const std::string &strMsg, void *userdata);

	CFastDBInterface();
	virtual ~CFastDBInterface();
	EFDB_ErrorCode Connect(const std::string &strIP, int nPort, const FDB_ConnInfo &connInfo);
	//EFDB_ErrorCode Reconnect();//����һ������
	EFDB_ErrorCode SelectDB(int nIndex); //ѡ��һ�����������ݲֿ���Ϊ��ǰ���������ݿ�
	EFDB_ErrorCode Disconnect();
	bool IsConnected();
	//ģ���쳣���ڲ�ֱ�ӶϿ����ӣ�
	bool InternalDisconnect();
    //����redis����ĳ�ʱʱ�䣬��λ����
    bool SetCommandTimeOut(int nCmdTimeOut);

	/************************************************************************/
	/*  ����key-value�������                                                */
    /*  �ֶ����ĺ궨��ο� keydef.h, ֻ֧��redis����ʵ���ڵ��ֶ�               */
    /*  ע�⣺ҵ����غ����޷���������ʱ��ʹ�ô���ӿ�                         */
	/************************************************************************/
    EFDB_ErrorCode IsKeyExists(const std::string &strKey); // EC_NOT_EXISTED��ʾ�����ڣ�EC_OK��ʾ���� ������ʾ����
	EFDB_ErrorCode Set(const std::string &strKey, const std::string &strValue);
	EFDB_ErrorCode Set(const std::string &strKey, int nValue);
	EFDB_ErrorCode MSet(const std::vector<std::string> &strKeys, const std::vector<std::string> &values);
	EFDB_ErrorCode MSet(const std::vector<std::string> &strKeys, const std::vector<int> &values);

	EFDB_ErrorCode Get(const std::string &strKey, std::string &strValue);
	EFDB_ErrorCode Get(const std::string &strKey, int &nValue);
	EFDB_ErrorCode MGet(const std::vector<std::string> &strKeys, std::map<std::string, std::string> &values);
	EFDB_ErrorCode MGet(const std::vector<std::string> &strKeys, std::map<std::string, int> &values);

	EFDB_ErrorCode Del(const std::string &strKey);
	EFDB_ErrorCode Del(const std::vector<std::string> &strKeys);

	/* �б����. +by lzx@2018-04-18 */
	EFDB_ErrorCode LPush(IN const string &strKey, IN const string &strValue);//��������С����˲���һ���µ�strValue
	EFDB_ErrorCode LPush(IN const string &strKey, IN int nValue);
	EFDB_ErrorCode RPush(IN const string &strKey, IN const string &strValue);//�������Ŵ����˲���һ���µ�strValue
	EFDB_ErrorCode RPush(IN const string &strKey, IN int nValue);
	//���б������е�ֵstrPivotValue֮ǰ(bBefore==true)��֮��(bBefore==false)�����µ�strValue
	EFDB_ErrorCode LInsert(IN const string &strKey, IN const string &strValue, IN const string &strPivotValue, IN bool bBefore);
	EFDB_ErrorCode LInsert(IN const string &strKey, IN int nValue, IN int nPivotValue, IN bool bBefore);

	EFDB_ErrorCode LLen(IN const string &strKey, int& len);//��ȡ�б�Ԫ�ظ���

	EFDB_ErrorCode LPop(IN const string &strKey, OUT string &strValue);//�����б�������ֵ��С���Ǹ�ֵ
	EFDB_ErrorCode LPop(IN const string &strKey, OUT int &nValue);
	EFDB_ErrorCode RPop(IN const string &strKey, OUT string &strValue);//�����б�������ֵ�����Ǹ�ֵ
	EFDB_ErrorCode RPop(IN const string &strKey, OUT int &nValue);


	/** �����ܰ汾 HMGet
	*	ͬһ��һ��fastdb����֧�ֶ��߳�ͬʱʹ��, ���߳�ʹ�ò�Ҫ��������ӿ�!
	*	+by lzx@2017-03-02
	*/
	EFDB_ErrorCode HMGet(const char* strKey, const char* field, char* valueBuf, size_t valueBufLen );

	/** ������ȡ�汾 HMGet
	*	�������Key��ͬ�����ֶ�. һ���Ƕ���豸��ͬ�������ֶε�ֵ.
	*	+by lzx@2017-03-03
	*/
	EFDB_ErrorCode HMGetBatch(IN const std::vector<std::string>& strKeys, IN const std::vector<std::string>& fields, OUT std::vector<std::vector<std::string> >& values);
	EFDB_ErrorCode HMGetBatch(IN const std::vector<std::string>& strKeys, IN const char * field, OUT std::vector<std::string>& values);

    EFDB_ErrorCode HMSetBatch(IN const std::vector<TBatchItem> &cItemList, OUT std::vector<int> &cResults);

    // hset��ȡ�ӿ� values��keyΪfiled,valueΪfiled��ֵ�����fields��ָ����filed�����ڣ�values�н���������Ӧ����Ŀ
    EFDB_ErrorCode HMGet(const std::string &strKey, const std::vector<std::string> &fields, std::map<std::string, std::string> &values);
    // hset���ýӿ�
    EFDB_ErrorCode HMSet(const std::string &strKey, const std::vector<std::string> &fields, const std::vector<std::string> &values);
    // hsetɾ���ӿ�
    EFDB_ErrorCode HDel(IN const std::string &strKey, IN const std::vector<std::string> &fields);
	/************************************************************************/
	/*              ���ϲ������												*/
	/************************************************************************/
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::string &strValue); //������ݵ�����
	EFDB_ErrorCode SAdd(const std::string &strKey, int nValue);
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::vector<std::string> &values);
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::vector<int> &values);
	
	EFDB_ErrorCode SRem(const std::string &strKey, const std::string &strValue); //�Ӽ�����ɾ������
	EFDB_ErrorCode SRem(const std::string &strKey, int nValue);
	EFDB_ErrorCode SRem(const std::string &strKey, const std::vector<std::string> &values);
	EFDB_ErrorCode SRem(const std::string &strKey, const std::vector<int> &values);
	
	EFDB_ErrorCode SCard(const std::string &strKey, int &nNum); //��ȡ�������Ԫ���ܸ���
	
	EFDB_ErrorCode SMembers(const std::string &strKey, std::vector<std::string> &values); //��ȡ�����������Ԫ��
	EFDB_ErrorCode SMembers(const std::string &strKey, std::vector<int> &values);
	
	//����EC_NOT_EXISTED��ʾ���ݲ����ڣ�����EC_OK����ʾ���ݴ��ڣ������������ʾ�ǽӿڱ������˴���
	EFDB_ErrorCode SisMember(const std::string &strKey, const std::string &strValue); //�ж�һ��Ԫ���Ƿ�����ڼ�����
	EFDB_ErrorCode SisMember(const std::string &strKey, int nValue);
	
	EFDB_ErrorCode SUnionStore(const std::string &strKey, const std::string &strNewKey);//����һ��set���µ�key

    /************************************************************************/
    /*              ����ҵ���޹غ��� 											*/
    /************************************************************************/
    EFDB_ErrorCode FDBSubscribe(const string &subName, SubscribeCallback func, void *userdata, bool bPattern = false); //���ĺͷ�����ؽӿ�
    EFDB_ErrorCode FDBSubscribe(const vector<string> &subName, SubscribeCallback func, void *userdata, bool bPattern = false);
    EFDB_ErrorCode FDBUnsubscribe(const string &subName, bool bPattern = false);
    EFDB_ErrorCode FDBUnsubscribe(const vector<string> &subName, bool bPattern = false);
    EFDB_ErrorCode FDBPublish(const string &subName, const string &strMsgData);

    EFDB_ErrorCode SetExpireTime(const string &strKey, int nTimeOutInSec); //���ù���ʱ��Ľӿ�
    EFDB_ErrorCode GetExpireTime(const string &strKey, int &nTimeOutInSec);

    // ���ӿ� EFDB_OK��ʾ�ɹ�������EC_LOCKED��ʾ��Դ�ѱ�����nAutoUnlockTimeSec��ʾ�ɹ���ס�ɹ�ʱ��������Զ�����
    EFDB_ErrorCode FDBLock(const string &lockerName, int nAutoUnlockTimeSec); 
    EFDB_ErrorCode FDBUnlock(const string &lockerName);

    // ��ȡ��ǰredis��ָ������ͨ���Ķ���������
    EFDB_ErrorCode GetSserNumBySubChannel(const string &strSubChannel, int &nNum);
    EFDB_ErrorCode GetSserNumBySubChannel(const vector<string> &cSubChannel, map<string, int> &cNum);

    // luaScript��lua�ű�����, keys��lua�ű���keys, args: lua�ű���args, luaNumber��lua�ű��ķ��ص�Number
    EFDB_ErrorCode Eval(const string &luaScript, const vector<string> &keys, const vector<string> &args, int &luaNumber);

    // �����Eval�ӿ�̫���򵥣���Ҫ�Ƿ���ֵ�̶���int���ͣ���ʵ��lua�ű���������������ȡ���ݣ��䷵��ֵ���ͱȽϸ��ӣ�
    // �������·�װһ��ֱ�ӷ��ؽ���ģ�������Ҫע�⣬ʹ������Ҫ�����ͷ�redisReply��Դ
    EFDB_ErrorCode Eval(const string &luaScript, const vector<string> &keys, const vector<string> &args, redisReply **reply);

    // ��evalsha�ķ�ʽִ��lua��ͬ���ģ�ʹ������Ҫ�����ͷ�redisReply��Դ
    EFDB_ErrorCode Evalsha(const string &luaScript, const vector<string> &keys, const vector<string> &args, redisReply **reply);


	/************************************************************************/
	/*              ҵ����صĺ����б��ֶ����ĺ궨��ο� keydef.h            */
	/************************************************************************/
    /*���ϵ*/
    EFDB_ErrorCode AddDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo);
    EFDB_ErrorCode DelDomainRelationInfo(const UUIDString &domainId);
    EFDB_ErrorCode GetDomainIDListNum(int &nTotalNum); // ��ȡ���ϵ������
    EFDB_ErrorCode GetDomainIDList(std::vector<UUIDString> &domainidList); // ��ȡ���ϵ����id�б�
    EFDB_ErrorCode GetFullDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo);
    EFDB_ErrorCode GetDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo, const string &field);
    EFDB_ErrorCode GetDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo, const vector<string> &fields);
    EFDB_ErrorCode SetDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo, const string &field);
    EFDB_ErrorCode SetDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo, const vector<string> &fields);

    /*�豸��Ϣ*/
    EFDB_ErrorCode AddDevice(const CDeviceInfo &dev);
    EFDB_ErrorCode DelDevice(const UUIDString &devId);
    EFDB_ErrorCode GetDeviceIDListNum(int &nTotalNum); // ��ȡ�豸��id��Ŀ
    EFDB_ErrorCode GetDeviceIDList(std::vector<UUIDString> &devidList);	// ��ȡ�豸��id�б�

    EFDB_ErrorCode DelDevField(const UUIDString &devId, const std::vector<std::string> &strFields);
    EFDB_ErrorCode DelDevField(const UUIDString &devId, const std::string &strField);
    EFDB_ErrorCode GetDevInfo(const UUIDString &devId, const std::string &strField, std::string &strValue);
    EFDB_ErrorCode GetDevInfo(const UUIDString &devId, const std::string &strField, int &nValue);
    EFDB_ErrorCode SetDevInfo(const UUIDString &devId, const std::string &strField, const std::string &strValue);
    EFDB_ErrorCode SetDevInfo(const UUIDString &devId, const std::string &strField, const int nValue); 

    EFDB_ErrorCode GetFullDeviceInfo(const UUIDString &devId, CDeviceInfo &dev);
    EFDB_ErrorCode GetDeviceInfo(const UUIDString &devId, CDeviceInfo &dev, const string &field);
    EFDB_ErrorCode GetDeviceInfo(const UUIDString &devId, CDeviceInfo &dev, const vector<string> &fields);
    EFDB_ErrorCode SetDeviceInfo(const UUIDString &devId, const CDeviceInfo &dev, const string &field);
    EFDB_ErrorCode SetDeviceInfo(const UUIDString &devId, const CDeviceInfo &dev, const vector<string> &fields);

    /*�豸Ȩ��*/
    EFDB_ErrorCode GetUserEncoderPrivilege(const UUIDString &userId, const UUIDString &devId, CEncoderPrivilege &priv);
	//֧�ֱ������ͽ�������������Ȩ������ӵ������Ȩ��
	EFDB_ErrorCode GetUserDevicePrivilege(const UUIDString &userId, const UUIDString &devId, CBitsContainer &priv);
    EFDB_ErrorCode GetUserVideoPrivilege(const UUIDString &userId, const UUIDString &devId, int index, CVideosourcePrivilege &priv);
    //vidPriv��keyΪ��ƵԴ������valueΪ��ƵԴȨ��
    EFDB_ErrorCode GetUserEncoderFullPrivilege(const UUIDString &userId, const UUIDString &devId, CEncoderPrivilege &devPriv, map<int, CVideosourcePrivilege> &vidPriv);
    // �ж�ָ���û��Ƿ�ӵ�и�������Ƶ�б�, vidLst��keyΪdevid, valueΪ��ƵԴ��ţ�result���ṹ��vidLstһ�£�ֻ�������ڵ�
    EFDB_ErrorCode IsUserVideoListExist(const UUIDString &userId, const map<string, int> &vidLst, map<string, int> &result);  
    // ������ѯ qryTag���״β�ѯʱΪ�գ����������ϴε��õķ���ֵ(����ֵΪ��ʱ��ʾ��ѯ����), dat��keyΪdevid, valueΪ��ƵԴ��ţ�timeOutInSec��������ѯ��ʱʱ��
    //EFDB_ErrorCode GetUserVideoList(const UUIDString &userId, string& qryTag, map<string, int> &dat, int timeOutInSec=600); 

    EFDB_ErrorCode AddUserDevPrivilege(const CUserDeviceInfo &devPriv);
    EFDB_ErrorCode ModUserDevPrivilege(const CUserDeviceModReq &devPriv);
    EFDB_ErrorCode DelUserDevPrivilege(const CUserDeviceInfoKey &devPriv);
    EFDB_ErrorCode AddUsergrpDevPrivilege(const CUserGroupDeviceInfo &devPriv);
    EFDB_ErrorCode ModUsergrpDevPrivilege(const CUserGroupDeviceModReq &devPriv);
    EFDB_ErrorCode DelUsergrpDevPrivilege(const CUserGroupDeviceInfoKey &devPriv);

    EFDB_ErrorCode SetUserUsergrpId(const UUIDString &userId, const UUIDString &usergrpId);
    EFDB_ErrorCode DelUser(const UUIDString &userId);
    EFDB_ErrorCode DelUsergrp(const UUIDString &usergrpId);//�������� ����EC_AGAIN��ʾ��Ҫ�������ã��Խ�������ɾ��
    EFDB_ErrorCode GetUserIdLst(vector<UUIDString> &idLst);
    EFDB_ErrorCode GetUsergrpIdLst(vector<UUIDString> &idLst);



	/*TPuStatus*/
	EFDB_ErrorCode GetFullPuStatus(const UUIDString &devId, TPuStatus &puStatus);
	EFDB_ErrorCode GetPuStatus(const UUIDString &devId, TPuStatus &puStatus, const string &field);
	EFDB_ErrorCode GetPuStatus(const UUIDString &devId, TPuStatus &puStatus, const vector<string> &fields);
	EFDB_ErrorCode SetPuStatus(const UUIDString &devId, const TPuStatus &puStatus, const string &field);
	EFDB_ErrorCode SetPuStatus(const UUIDString &devId, const TPuStatus &puStatus, const vector<string> &fields);

	/*TPuConfig*/
	EFDB_ErrorCode GetFullPuConfig(const UUIDString &devId, TPuConfig &puConfig);
	EFDB_ErrorCode GetPuConfig(const UUIDString &devId, TPuConfig &puConfig, const string &field);
	EFDB_ErrorCode GetPuConfig(const UUIDString &devId, TPuConfig &puConfig, const vector<string> &fields);	
	EFDB_ErrorCode SetPuConfig(const UUIDString &devId, const TPuConfig &puConfig, const string &field);
	EFDB_ErrorCode SetPuConfig(const UUIDString &devId, const TPuConfig &puConfig, const vector<string> &fields);
    EFDB_ErrorCode AddPuConfig(const UUIDString &devId, const TPuConfig &puconfig); //�������TPuconfig�ṹ ����PUI��

	/** 
	*	׷�������Ż���ͨ��״̬��ȡ����.
	*	+by lzx@2017-03-02
	*/
	EFDB_ErrorCode GetPuChnStatus( const char* devId, int chnId, int& chnStat );

	/*TPuBaseCapSet*/ 
	EFDB_ErrorCode GetFullPuBaseCapSet(const UUIDString &devId, TPuBaseCapSet &puBaseCapSet);
	EFDB_ErrorCode GetPuBaseCapSet(const UUIDString &devId, TPuBaseCapSet &puBaseCapSet, const string &field);
	EFDB_ErrorCode GetPuBaseCapSet(const UUIDString &devId, TPuBaseCapSet &puBaseCapSet, const vector<string> &fields);
	EFDB_ErrorCode SetPuBaseCapSet(const UUIDString &devId, const TPuBaseCapSet &puBaseCapSet, const string &field);
	EFDB_ErrorCode SetPuBaseCapSet(const UUIDString &devId, const TPuBaseCapSet &puBaseCapSet, const vector<string> &fields);

	/*TPuServiceStatus*/
	EFDB_ErrorCode GetFullPuServiceStatus(const UUIDString &devId, TPuServiceStatus &puServiceStatus);
	EFDB_ErrorCode GetPuServiceStatus(const UUIDString &devId, TPuServiceStatus &puServiceStatus, const string &field);
	EFDB_ErrorCode GetPuServiceStatus(const UUIDString &devId, TPuServiceStatus &puServiceStatus, const vector<string> &fields);
	EFDB_ErrorCode SetPuServiceStatus(const UUIDString &devId, const TPuServiceStatus &puServiceStatus, const string &field);
	EFDB_ErrorCode SetPuServiceStatus(const UUIDString &devId, const TPuServiceStatus &puServiceStatus, const vector<string> &fields);
    EFDB_ErrorCode ClearServiceStatus(const UUIDString &devId);

    /*PuAlarmCollector*/
    EFDB_ErrorCode GetAlarmChnInfo(const UUIDString &devId, const std::string &strAlarmType, CAlarmInputList &alarmInfo);
    EFDB_ErrorCode GetAlarmChnInfo(const UUIDString &devId, std::map<std::string, CAlarmInputList> &alarmInfo);
    EFDB_ErrorCode SetAlarmChnInfo(const UUIDString &devId, const std::string &strAlarmType, CAlarmInputList &alarmInfo);
    EFDB_ErrorCode SetAlarmChnInfo(const UUIDString &devId, std::map<std::string, CAlarmInputList> &alarmInfo);
    EFDB_ErrorCode ClearAlarmChnInfo(const UUIDString &devId);

	/*CRedisPuData*/
	EFDB_ErrorCode GetFullPuData(const UUIDString &devId, CRedisPuData &puData);
	EFDB_ErrorCode GetPuData(const UUIDString &devId, CRedisPuData &puData, const string &field);
	EFDB_ErrorCode GetPuData(const UUIDString &devId, CRedisPuData &puData, const vector<string> &fields);
	EFDB_ErrorCode SetPuData(const UUIDString &devId, const CRedisPuData &puData, const string &field);
	EFDB_ErrorCode SetPuData(const UUIDString &devId, const CRedisPuData &puData, const vector<string> &fields);		



private:
	void ReInitObj();
	void DeInitObj();
	void OnDisconnected();//����
	bool StartHB();//���������߳�
	/*�����������̬�ֶεĺ���*/
	EFDB_ErrorCode Internal_SetPuData(const UUIDString &devId, const CRedisPuData &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetDeviceInfo(const UUIDString &devId, const CDeviceInfo &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetPuServiceStatus(const UUIDString &devId, const TPuServiceStatus &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetPuBaseCapSet(const UUIDString &devId, const TPuBaseCapSet &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetPuStatus(const UUIDString &devId, const TPuStatus &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetPuConfig(const UUIDString &devId, const TPuConfig &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_SetDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &struData, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetPuConfig(const UUIDString &devId, TPuConfig &puConfig, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetPuServiceStatus(const UUIDString &devId, TPuServiceStatus &puServiceStatus, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetPuStatus(const UUIDString &devId, TPuStatus &puStatus, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetPuBaseCapSet(const UUIDString &devId, TPuBaseCapSet &puBaseCapSet, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetDeviceInfo(const UUIDString &devId, CDeviceInfo &dev, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetPuData(const UUIDString &devId, CRedisPuData &puData, const vector<string> &fields);
	EFDB_ErrorCode Internal_GetDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo,  const vector<string> &fields);


    EFDB_ErrorCode DelUserPriviByDev(const UUIDString &devId);
    EFDB_ErrorCode DelUserPriviByDev(const UUIDString &devId, const vector<int> &vidId);
	//ֱ����redis��Ҫÿ���澯���Ͱ���ͨ���������ֶΣ�����ֶ���sdk˽��ʹ�õģ� type2chns����������豸ÿ�ָ澯���Ͷ�Ӧ��ͨ��������
	EFDB_ErrorCode GetAlarmTypeChnNum(const UUIDString &devId, std::map<std::string, std::set<int> > &type2chns);

	//�������� CHNS_WITH_ALARM_TYPE(type)�ĺ���
	EFDB_ErrorCode SetAlarmNum(const UUIDString &devId, const std::map<std::string, CAlarmInputList> &setData);

	//��ָ���ṹ����Դ�ֶ�srcFieldת��redis���ֶΣ�Դ�ֶ�srcField�����������ֶΰ��������ֶκͶ�̬���ֶΣ�ת������ֶ�ֻ����������ʱ���ʹ���������
	//refData��ʾ���ڲ��õ��Ķ�̬�ֶ������һЩ��Ϣ��redis�л�ȡ����Զ��浽������������ʹ�ã���ֹ���ͬһ��ת���ڲ������redis��ȡ��Щ����
	EFDB_ErrorCode FieldToRedisFieldForRead(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData);
	//����Щ����ṹ�壬������϶�̬�ֶ���������Ľṹ��ʹ��
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData);
	//�ṹ��������˶�̬�ֶ���������ģ���������ṹ����Щ��̬�ֶ�����Ҫ��Щ�����������ɵ�ʱ����,��ʱ�����Ҫ�����ٴ��ϣ�����ṹ���Լ�������Ҫд����ֶεļ��ϣ����ڲ��ο��Ǵӽṹ����ȡ�������ǵ�redis��ȥȡ����
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const TPuConfig &struData);
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const CRedisPuData &struData);
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const TPuServiceStatus &struData);
private:
	std::string m_ip;
	int m_port;
	FDB_ConnInfo m_connInfo;
	void *m_redis;          //CRedisBase����
	void *m_subpubThread;   // CSubPubThread����
	void *m_hbThread;//�����߳� CHBThread
	std::map<std::string, std::pair<SubscribeCallback, void *> > m_CBData; //����ȡ��������صĲ���
	void *m_cbLocker;
};

//���Դ�ӡ�Ļص�������nLevel:ERROR_LEV��ʾ���󼶱�CRITICAL_LEV��ʾ��Ҫ���EVENT_LEV��ʾ��ͨ����PROGRESS_LEV��ʾƵ������
typedef void(*LogCallBack)(int /*nLevel*/, const char * /*strLogMsg*/, void * /*userdata*/);
void SetLogCallBack(LogCallBack func, void *userdata);