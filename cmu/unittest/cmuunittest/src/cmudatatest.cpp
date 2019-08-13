#include "cppunitlite/cppunitlite.h"
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"
#include "cms/tas/tas_proto.h"

#include "cmudata.h"
#include "cmucoreapp.h"
#include "puregtask.h"

TEST_GROUP( CPuData )
{

};

/*
bool IsUsed() const;
bool IsExpire() const;
s32 GetPuLicenseNum() const;
*/

TEST( CPuData, DevInfo )
{
    CPuData cPuData;

    CDeviceInfo tDevInfo;
    tDevInfo.SetDeviceUUID("55000000000000000011200019800000");
    CCMSTime cTime;
    cTime.SetLocalTime(2013, 1, 1, 1, 1, 1, 1);
    string strExpireTime = cTime.ToString();
    tDevInfo.SetExpireDate(strExpireTime);
    tDevInfo.SetUsedFlag(true);

    vector<TDeviceCapInfo>& tCapInfoList = tDevInfo.GetDeviceModelCapDetail();
    TDeviceCapInfo tCapInfo;
    tCapInfo.license = 1;
    tCapInfo.type = DEVICE_TYPE_ENCODER;
    tCapInfoList.push_back(tCapInfo);

    cPuData.SetDevInfo(tDevInfo);
    bool bIsDevUsed = cPuData.IsUsed();
	CK_EQ( true, bIsDevUsed );

    bool bIsDevExpire = cPuData.IsExpire();
    CK_EQ( false, bIsDevExpire );

    int nLicenseNum = cPuData.GetPuLicenseNum();
    CK_EQ( 1, nLicenseNum );

}

/*
void SetRegTask(CPuRegTask* ptTask);
CPuRegTask* GetRegTask() const;
*/

TEST( CPuData, RegTask )
{
    CPuData cPuData;

    CPuRegTask* pRegTask = (CPuRegTask*)0x12345678;
    cPuData.SetRegTask(pRegTask);
    CK_EQ( pRegTask, cPuData.GetRegTask() );
}

/*
void AddSsTask(CPuSubscribeTask* ptTask);
void DelSsTask(CPuSubscribeTask* ptTask);
void DelSsTask(const string& strCuSession);
*/
TEST( CPuData, SsTask )
{
    CPuData cPuData;

    CPuSubscribeTask* pSsTask = (CPuSubscribeTask*)0x12345678;
    cPuData.AddSsTask(pSsTask);
}


TEST_GROUP( CPuDataList )
{

};


/*
public:
void AddPuData(CPuData* pPuData);
void DelPuData(CPuData* pPuData);
void DelPuData(const string& strUri);
CPuData* FindPuData(const string& strUri);

CPuRegTask* FindRegTask(const string& strUri);

public:
void PrintData(u8 byFlag = 0);
u32 GetOnlinePuNum();
u32 GetSize();
*/

TEST( CPuDataList, DataMgr )
{
    CPuDataList cPuList;
    string strDevUUID1 = "55000000000000000011200019800000";
    CDeviceInfo tDevInfo1;
    tDevInfo1.SetDeviceUUID(strDevUUID1);
    CPuData cPuData1;

    CApp* ptApp = (CApp*)(&g_cCmuCoreApp);
    CInstance* pInst = ptApp->GetInstance(CInstance::DAEMON);
    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);

    CPuRegTask* pRegTask = new CPuRegTask(pInstExt);
    cPuData1.SetRegTask(pRegTask);
    cPuData1.SetDevInfo(tDevInfo1);
    cPuList.AddPuData(&cPuData1);

    string strDevUUID2 = "55000000000000000011200019900000";
    CDeviceInfo tDevInfo2;
    tDevInfo2.SetDeviceUUID(strDevUUID2);
    CPuData cPuData2;
    cPuData2.SetDevInfo(tDevInfo2);
    cPuList.AddPuData(&cPuData2);

    CK_EQ( 2, cPuList.GetSize() );
    CK_EQ( 1, cPuList.GetOnlinePuNum() );

    CPuData* pcPuData1 = cPuList.FindPuData(GetUriFromDevId(strDevUUID1));
    CK_EQ( pcPuData1, &cPuData1 );

    CPuData* pcPuData2 = cPuList.FindPuData(GetUriFromDevId(strDevUUID2));
    CK_EQ( pcPuData2, &cPuData2 );

    cPuList.DelPuData(pcPuData2);
    CK_EQ( 1, cPuList.GetSize() );
    CK_EQ( 1, cPuList.GetOnlinePuNum() );

    cPuList.DelPuData(GetUriFromDevId(strDevUUID1));
    CK_EQ( 0, cPuList.GetSize() );
    CK_EQ( 0, cPuList.GetOnlinePuNum() );

    delete pRegTask;

}


