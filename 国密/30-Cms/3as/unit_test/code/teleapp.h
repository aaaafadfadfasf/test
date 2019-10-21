#ifndef _TELEAPP_H_
#define _TELEAPP_H_
#include "inner_common.h"

class CTeleInst: public CInstExt
{
    void DaemonEntry(CMessage* const pcMsg);
    void NormalEntry(CMessage* const pcMsg){};
    virtual LPCSTR GetStrState() const{return "NA";}
    void DefaultMsgHandler(CMessage* const pcMsg);
    // 从文件中读取请求自动发送给TAS
    void AutoTest(const s8* cstrfile); 
    // 自动处理TAS发送过来的消息，能够处理时返回true
    bool AutoTest(CMessage* const pcMsg);

    void OnPowerOn();
    void TasLogin(s32 logintype, const s8* pszName, const s8* pszPwd, const s8* pszExtra);
    void TasLoginRsp(CMessage* const pcMsg);
    void TasLogout();
    void TasOnline(const TOnlineInfo &tInfo);
    void TasOffline(const s8* cstrCuSessid);

    void DevModelAddReq(const TDevModelAdd &tInfo);
    void DevModelAddRsp(CMessage* const pcMsg);
    void DevModelDelReq(const string &strModelId);
    void DevModelDelRsp(CMessage* const pcMsg);
    void DevModelQryReq();
    void DevModelQryRsp(CMessage* const pcMsg);
    void DevModelQryNtfReq(CMessage* const pcMsg);

    void DevAddReq(const TDevAddInfo &tAddInfo);
    void DevAddRsp(CMessage* const pcMsg);
    void DevDelReq(const string &strDevId);
    void DevDelRsp(CMessage* const pcMsg);
    void DevModReq(const TDevModInfo &tModInfo);
    void DevModRsp(CMessage* const pcMsg);
    void DevQryReq(const TDevQryInfo &tQryInfo);
    void DevQryRsp(CMessage* const pcMsg);
    void DevQryNtfReq(CMessage* const pcMsg);
private:
    TSipURI m_tTasSipUri;
    TOspTransation m_tSrcOspTrans;
    string m_strSessId; // 会话Id
};

typedef COspApp<CTeleInst, 1> CTeleApp;

#endif // _TELEAPP_H_
