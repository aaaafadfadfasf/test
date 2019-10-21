/************************************************************************
* 将访问快速数据库（目前redis）的接口进行封装，
* 方便各个模块以统一的接口向快速数据库读取或写
* 入业务相关的特定数据 。
* 2016-02-25 zsy 创建
* 2016-07-21 hzy 添加权限相关接口
* 2016-09-19 hzy 全部接口按照功能编排，修订和精简注释
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
	EC_FAILED,						//失败了（接口本身就是只有成功失败的时候）
	EC_DISCONNECTED,				//断链了
	EC_TIMEOUT,						//超时了
	EC_AUTHEN_FAILED,				//认证失败	
	EC_NOT_IMPLEMENT,				//接口未实现
	EC_INVALID_PARAM,				//参数不对
	EC_NO_MATCH_MEMBER,				//redis字段名无法找到对应的结构体成员
	EC_LOCKED,						//已锁
	EC_AGAIN,                       //未执行结束（由于比较耗时提前返回，使用者可以继续调用）
    EC_NOT_EXISTED,                 //数据不存在

	EC_NUM
};

inline bool IsFdbResultPositive( EFDB_ErrorCode err )
{
    return err == EC_OK;
}

enum EFDB_StatusCode
{
	SC_DISCONNECTED,						//断链了


	SC_NUM
};
class CFastDBInterface;
//订阅回调函数定义
typedef void(*SubscribeCallback)(CFastDBInterface * /*pInter*/, const std::string &/*strChnName*/, const std::string &/*strMsgData*/, void * /*userdata*/);
//redis断链通知或其他可能的状态通知回调函数定义(这个是应胡志云要求加的）
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
	std::string clientname;	//客户端自己取的用于标识自己的名称
	std::string username;	//对于目前的redis来说，没有这项，仅作预留
	std::string password;	//如果有设定密码
	bool bAutoReconn;		//是否自动重连(该功能由于订阅机制的存在，目前不生效，断链会给断链通知，由上层决定怎么处理，比如调用Reconnect()进行重连)
	int connTimeOutMs;		//连接时的超时时间单位毫秒(<=0表示使用系统默认)
	int accessTimeOutMs;	//存取操作的超时时间单位毫秒(<=0表示使用系统默认)
	RedisStatusCallback pfStatusCallBack;//如果需要断链通知请设置这个回调函数
	void *pfStatusCallBack_userdata;			//传递给pfStatusCallBack回调函数的用户数据
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
	//EFDB_ErrorCode Reconnect();//进行一次重连
	EFDB_ErrorCode SelectDB(int nIndex); //选择一个独立的数据仓库作为当前操作的数据库
	EFDB_ErrorCode Disconnect();
	bool IsConnected();
	//模拟异常（内部直接断开连接）
	bool InternalDisconnect();
    //设置redis命令的超时时间，单位毫秒
    bool SetCommandTimeOut(int nCmdTimeOut);

	/************************************************************************/
	/*  基本key-value操作相关                                                */
    /*  字段名的宏定义参看 keydef.h, 只支持redis中真实存在的字段               */
    /*  注意：业务相关函数无法满足需求时才使用此类接口                         */
	/************************************************************************/
    EFDB_ErrorCode IsKeyExists(const std::string &strKey); // EC_NOT_EXISTED表示不存在，EC_OK表示存在 其它表示错误
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

	/* 列表操作. +by lzx@2018-04-18 */
	EFDB_ErrorCode LPush(IN const string &strKey, IN const string &strValue);//往索引号小的这端插入一个新的strValue
	EFDB_ErrorCode LPush(IN const string &strKey, IN int nValue);
	EFDB_ErrorCode RPush(IN const string &strKey, IN const string &strValue);//往索引号大的这端插入一个新的strValue
	EFDB_ErrorCode RPush(IN const string &strKey, IN int nValue);
	//在列表中已有的值strPivotValue之前(bBefore==true)或之后(bBefore==false)插入新的strValue
	EFDB_ErrorCode LInsert(IN const string &strKey, IN const string &strValue, IN const string &strPivotValue, IN bool bBefore);
	EFDB_ErrorCode LInsert(IN const string &strKey, IN int nValue, IN int nPivotValue, IN bool bBefore);

	EFDB_ErrorCode LLen(IN const string &strKey, int& len);//获取列表元素个数

	EFDB_ErrorCode LPop(IN const string &strKey, OUT string &strValue);//弹出列表中索引值最小的那个值
	EFDB_ErrorCode LPop(IN const string &strKey, OUT int &nValue);
	EFDB_ErrorCode RPop(IN const string &strKey, OUT string &strValue);//弹出列表中索引值最大的那个值
	EFDB_ErrorCode RPop(IN const string &strKey, OUT int &nValue);


	/** 高性能版本 HMGet
	*	同一个一个fastdb对象不支持多线程同时使用, 多线程使用不要调用这个接口!
	*	+by lzx@2017-03-02
	*/
	EFDB_ErrorCode HMGet(const char* strKey, const char* field, char* valueBuf, size_t valueBufLen );

	/** 批量获取版本 HMGet
	*	批量多个Key的同样的字段. 一般是多个设备的同样几个字段的值.
	*	+by lzx@2017-03-03
	*/
	EFDB_ErrorCode HMGetBatch(IN const std::vector<std::string>& strKeys, IN const std::vector<std::string>& fields, OUT std::vector<std::vector<std::string> >& values);
	EFDB_ErrorCode HMGetBatch(IN const std::vector<std::string>& strKeys, IN const char * field, OUT std::vector<std::string>& values);

    EFDB_ErrorCode HMSetBatch(IN const std::vector<TBatchItem> &cItemList, OUT std::vector<int> &cResults);

    // hset获取接口 values的key为filed,value为filed的值，如果fields中指定的filed不存在，values中将不存在相应的条目
    EFDB_ErrorCode HMGet(const std::string &strKey, const std::vector<std::string> &fields, std::map<std::string, std::string> &values);
    // hset设置接口
    EFDB_ErrorCode HMSet(const std::string &strKey, const std::vector<std::string> &fields, const std::vector<std::string> &values);
    // hset删除接口
    EFDB_ErrorCode HDel(IN const std::string &strKey, IN const std::vector<std::string> &fields);
	/************************************************************************/
	/*              集合操作相关												*/
	/************************************************************************/
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::string &strValue); //添加数据到集合
	EFDB_ErrorCode SAdd(const std::string &strKey, int nValue);
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::vector<std::string> &values);
	EFDB_ErrorCode SAdd(const std::string &strKey, const std::vector<int> &values);
	
	EFDB_ErrorCode SRem(const std::string &strKey, const std::string &strValue); //从集合中删除数据
	EFDB_ErrorCode SRem(const std::string &strKey, int nValue);
	EFDB_ErrorCode SRem(const std::string &strKey, const std::vector<std::string> &values);
	EFDB_ErrorCode SRem(const std::string &strKey, const std::vector<int> &values);
	
	EFDB_ErrorCode SCard(const std::string &strKey, int &nNum); //获取集合里的元素总个数
	
	EFDB_ErrorCode SMembers(const std::string &strKey, std::vector<std::string> &values); //获取集合里的所有元素
	EFDB_ErrorCode SMembers(const std::string &strKey, std::vector<int> &values);
	
	//返回EC_NOT_EXISTED表示数据不存在，返回EC_OK，表示数据存在，其他错误码表示是接口本身发生了错误
	EFDB_ErrorCode SisMember(const std::string &strKey, const std::string &strValue); //判断一个元素是否存在于集合中
	EFDB_ErrorCode SisMember(const std::string &strKey, int nValue);
	
	EFDB_ErrorCode SUnionStore(const std::string &strKey, const std::string &strNewKey);//复制一份set到新的key

    /************************************************************************/
    /*              其它业务无关函数 											*/
    /************************************************************************/
    EFDB_ErrorCode FDBSubscribe(const string &subName, SubscribeCallback func, void *userdata, bool bPattern = false); //订阅和发布相关接口
    EFDB_ErrorCode FDBSubscribe(const vector<string> &subName, SubscribeCallback func, void *userdata, bool bPattern = false);
    EFDB_ErrorCode FDBUnsubscribe(const string &subName, bool bPattern = false);
    EFDB_ErrorCode FDBUnsubscribe(const vector<string> &subName, bool bPattern = false);
    EFDB_ErrorCode FDBPublish(const string &subName, const string &strMsgData);

    EFDB_ErrorCode SetExpireTime(const string &strKey, int nTimeOutInSec); //设置过期时间的接口
    EFDB_ErrorCode GetExpireTime(const string &strKey, int &nTimeOutInSec);

    // 锁接口 EFDB_OK表示成功锁定，EC_LOCKED表示资源已被锁，nAutoUnlockTimeSec表示成功锁住成功时多少秒后自动解锁
    EFDB_ErrorCode FDBLock(const string &lockerName, int nAutoUnlockTimeSec); 
    EFDB_ErrorCode FDBUnlock(const string &lockerName);

    // 获取当前redis中指定订阅通道的订阅者数量
    EFDB_ErrorCode GetSserNumBySubChannel(const string &strSubChannel, int &nNum);
    EFDB_ErrorCode GetSserNumBySubChannel(const vector<string> &cSubChannel, map<string, int> &cNum);

    // luaScript：lua脚本内容, keys：lua脚本的keys, args: lua脚本的args, luaNumber：lua脚本的返回的Number
    EFDB_ErrorCode Eval(const string &luaScript, const vector<string> &keys, const vector<string> &args, int &luaNumber);

    // 上面的Eval接口太过简单，主要是返回值固定是int类型，而实际lua脚本可能用于批量获取数据，其返回值类型比较复杂，
    // 这里重新封装一个直接返回结果的，但是需要注意，使用者需要自行释放redisReply资源
    EFDB_ErrorCode Eval(const string &luaScript, const vector<string> &keys, const vector<string> &args, redisReply **reply);

    // 以evalsha的方式执行lua，同样的，使用者需要自行释放redisReply资源
    EFDB_ErrorCode Evalsha(const string &luaScript, const vector<string> &keys, const vector<string> &args, redisReply **reply);


	/************************************************************************/
	/*              业务相关的函数列表，字段名的宏定义参看 keydef.h            */
	/************************************************************************/
    /*域关系*/
    EFDB_ErrorCode AddDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo);
    EFDB_ErrorCode DelDomainRelationInfo(const UUIDString &domainId);
    EFDB_ErrorCode GetDomainIDListNum(int &nTotalNum); // 获取域关系的数量
    EFDB_ErrorCode GetDomainIDList(std::vector<UUIDString> &domainidList); // 获取域关系的域id列表
    EFDB_ErrorCode GetFullDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo);
    EFDB_ErrorCode GetDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo, const string &field);
    EFDB_ErrorCode GetDomainRelationInfo(const UUIDString &domainId, CDomainRelationInfo &domainInfo, const vector<string> &fields);
    EFDB_ErrorCode SetDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo, const string &field);
    EFDB_ErrorCode SetDomainRelationInfo(const UUIDString &domainId, const CDomainRelationInfo &domainInfo, const vector<string> &fields);

    /*设备信息*/
    EFDB_ErrorCode AddDevice(const CDeviceInfo &dev);
    EFDB_ErrorCode DelDevice(const UUIDString &devId);
    EFDB_ErrorCode GetDeviceIDListNum(int &nTotalNum); // 获取设备的id数目
    EFDB_ErrorCode GetDeviceIDList(std::vector<UUIDString> &devidList);	// 获取设备的id列表

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

    /*设备权限*/
    EFDB_ErrorCode GetUserEncoderPrivilege(const UUIDString &userId, const UUIDString &devId, CEncoderPrivilege &priv);
	//支持编码器和解码器，解码器权限虚拟拥有所有权限
	EFDB_ErrorCode GetUserDevicePrivilege(const UUIDString &userId, const UUIDString &devId, CBitsContainer &priv);
    EFDB_ErrorCode GetUserVideoPrivilege(const UUIDString &userId, const UUIDString &devId, int index, CVideosourcePrivilege &priv);
    //vidPriv的key为视频源索引，value为视频源权限
    EFDB_ErrorCode GetUserEncoderFullPrivilege(const UUIDString &userId, const UUIDString &devId, CEncoderPrivilege &devPriv, map<int, CVideosourcePrivilege> &vidPriv);
    // 判断指定用户是否拥有给定的视频列表, vidLst：key为devid, value为视频源序号；result：结构与vidLst一致，只包含存在的
    EFDB_ErrorCode IsUserVideoListExist(const UUIDString &userId, const map<string, int> &vidLst, map<string, int> &result);  
    // 分批查询 qryTag：首次查询时为空，后续传递上次调用的返回值(返回值为空时表示查询结束), dat：key为devid, value为视频源序号，timeOutInSec：整个查询超时时长
    //EFDB_ErrorCode GetUserVideoList(const UUIDString &userId, string& qryTag, map<string, int> &dat, int timeOutInSec=600); 

    EFDB_ErrorCode AddUserDevPrivilege(const CUserDeviceInfo &devPriv);
    EFDB_ErrorCode ModUserDevPrivilege(const CUserDeviceModReq &devPriv);
    EFDB_ErrorCode DelUserDevPrivilege(const CUserDeviceInfoKey &devPriv);
    EFDB_ErrorCode AddUsergrpDevPrivilege(const CUserGroupDeviceInfo &devPriv);
    EFDB_ErrorCode ModUsergrpDevPrivilege(const CUserGroupDeviceModReq &devPriv);
    EFDB_ErrorCode DelUsergrpDevPrivilege(const CUserGroupDeviceInfoKey &devPriv);

    EFDB_ErrorCode SetUserUsergrpId(const UUIDString &userId, const UUIDString &usergrpId);
    EFDB_ErrorCode DelUser(const UUIDString &userId);
    EFDB_ErrorCode DelUsergrp(const UUIDString &usergrpId);//分批处理 返回EC_AGAIN表示需要继续调用，以进行数据删除
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
    EFDB_ErrorCode AddPuConfig(const UUIDString &devId, const TPuConfig &puconfig); //添加完整TPuconfig结构 仅给PUI用

	/** 
	*	追求性能优化的通道状态获取函数.
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
	void OnDisconnected();//断链
	bool StartHB();//开启心跳线程
	/*不解析虚拟或动态字段的函数*/
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
	//直接向redis索要每个告警类型包含通道的数量字段（这个字段是sdk私有使用的） type2chns保存着这个设备每种告警类型对应的通道数集合
	EFDB_ErrorCode GetAlarmTypeChnNum(const UUIDString &devId, std::map<std::string, std::set<int> > &type2chns);

	//额外设置 CHNS_WITH_ALARM_TYPE(type)的函数
	EFDB_ErrorCode SetAlarmNum(const UUIDString &devId, const std::map<std::string, CAlarmInputList> &setData);

	//将指定结构体中源字段srcField转成redis的字段，源字段srcField可以是任意字段包括虚拟字段和动态的字段，转换后的字段只是用来读的时候才使用这个函数
	//refData表示，内部用到的动态字段所需的一些信息从redis中获取后会自动存到这里来供本次使用，防止多次同一次转换内部多次向redis获取这些数据
	EFDB_ErrorCode FieldToRedisFieldForRead(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData);
	//给那些自身结构体，不会带上动态字段所需参数的结构体使用
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData);
	//结构体自身带了动态字段所需参数的，并且自身结构体有些动态字段又需要这些参数才能生成的时候用,这时候就需要额外再带上，这个结构体以及所有需要写入的字段的集合，供内部参考是从结构体中取参数还是到redis中去取参数
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const TPuConfig &struData);
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const CRedisPuData &struData);
	EFDB_ErrorCode FieldToRedisFieldForWrite(const std::string &devId, const std::string &srcField, std::vector<std::string> &redisFields, INOUT std::map<std::string, int> &refData, const TPuServiceStatus &struData);
private:
	std::string m_ip;
	int m_port;
	FDB_ConnInfo m_connInfo;
	void *m_redis;          //CRedisBase对象
	void *m_subpubThread;   // CSubPubThread对象
	void *m_hbThread;//心跳线程 CHBThread
	std::map<std::string, std::pair<SubscribeCallback, void *> > m_CBData; //订阅取消订阅相关的参数
	void *m_cbLocker;
};

//调试打印的回调函数，nLevel:ERROR_LEV表示错误级别，CRITICAL_LEV表示重要界别，EVENT_LEV表示普通级别，PROGRESS_LEV表示频繁级别。
typedef void(*LogCallBack)(int /*nLevel*/, const char * /*strLogMsg*/, void * /*userdata*/);
void SetLogCallBack(LogCallBack func, void *userdata);