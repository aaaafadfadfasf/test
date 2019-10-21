#ifndef _INCLUDE_ALARM_TYPE_COMMON_
#define _INCLUDE_ALARM_TYPE_COMMON_


#include <string>

//#ifndef CMS_ALARM_TYPE_TAG
//#define CMS_ALARM_TYPE_TAG
typedef std::string CMS_ALARM_TYPE ;
//#endif //CMS_ALARM_TYPE

//#ifndef CU_ALARM_TYPE_TAG
//#define CU_ALARM_TYPE_TAG
typedef int CU_ALARM_TYPE;
//#endif //CU_ALARM_TYPE

struct TAlarmTypeCommonImpl;

struct TAlarmData
{
    std::string strDescKey;
};

class IAlarmDataWriter
{
public:
    IAlarmDataWriter(){}
    virtual ~IAlarmDataWriter(){}
public:
    virtual void Write( const CMS_ALARM_TYPE &cmsType, const TAlarmData &tData ) = 0;
};

class CAlarmTypeCommon
{
public:
    CAlarmTypeCommon(void);
    ~CAlarmTypeCommon(void);

public:
    CMS_ALARM_TYPE ConvertFromCuToCms( CU_ALARM_TYPE cuType  ) const;
    CU_ALARM_TYPE ConvertFromCmsToCu(const CMS_ALARM_TYPE &cmsType) const;
    bool HasCuAlarmType(CU_ALARM_TYPE &cuType) const;
    bool HasCmsAlarmType( const CMS_ALARM_TYPE &cmsType) const;
    bool IsInCuV2Only( CU_ALARM_TYPE cuType ) const;
	std::string GetAlmDesc( const CMS_ALARM_TYPE &cmsType) const;

public:
    bool WriteAllAlarmDesc(IAlarmDataWriter *pcWriter) const;

private:
    void InitCuiCuAlmMap();
    void InitCuiDescKeyMap();
	void InitAlmDescMap();
    //void AddAlarmType( const CMS_ALARM_TYPE &cmsType, CU_ALARM_TYPE cuType );

private:
    TAlarmTypeCommonImpl *m_ptImpl; //使用结构指针装载成员
};

const CAlarmTypeCommon* GetCommonType();

#endif //_INCLUDE_ALARM_TYPE_COMMON_