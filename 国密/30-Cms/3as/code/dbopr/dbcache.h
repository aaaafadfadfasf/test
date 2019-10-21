/*
���ܣ�Ϊdbopr�ṩ�������滺��һЩ���ݿ�����, ���ݲ�����ʱ����dboprʵʱ��ȡ
*/
#ifndef _DBCACHE_H_
#define _DBCACHE_H_
#include "inner_common.h"

class CDbOpr;

class CDbCache
{
public:
    bool Init();
    void Quit();
    bool IsConnectedDb();
public:
    CDeviceModelInfo GetDeviceModelInfo(const UUIDString &strModelId, CDbOpr *pcDbOpr);
    void DeleteDeviceModelInfoByModelId(const UUIDString &strModelId);
    void DeleteDeviceModelInfoByDomainId(const UUIDString &strDomainId); // hzytodo2 domain del
    
private:
    typedef map<UUIDString, CDeviceModelInfo> DevModelMap;
    DevModelMap m_cDeviceModels;
    SEMHANDLE m_hDeviceModelLock;

    SEMHANDLE m_hDbOprLock;
    CDbOpr *m_pcDbOpr;
};

extern CDbCache g_cDbCache;

#endif // _DBCACHE_H_
