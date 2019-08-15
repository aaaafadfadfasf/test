#ifndef _TAS_STRUCT_H_
#define _TAS_STRUCT_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
using std::string;
using std::vector;
using std::set;
using std::map;
using std::istringstream;
using std::ostringstream;

class TiXmlDocument;
class TiXmlNode;

//enum TQueryOrderFieldId // ��ѯʱ�����ֶα�ʶ
//{
//    // hzytodo(��������ʱ����) ������ָ�����������, ʹ�÷�ʽ��������1.0�еġ�enum EmQryOrderOption��
//    //TUser..
//    em_TUserNOOrderTerm          = 1, // �����û����
//    em_TUserNameOrderTerm        = 2, // �����û���
//    em_TUserTypeOrderTerm        = 3, // �����û�����
//    em_TUserSexOrderTerm         = 4, // �����û��Ա�
//    em_TUserUseStateOrderTerm    = 5, // �����û���ͣ״̬
//    em_TUserDepartmentOrderTerm  = 6, // �����û�������λ
//    em_TUserStaffNumOrderTerm    = 7, // �����û�����
//    em_TUserTelephoneOrderTerm   = 8, // �����û���ϵ�绰
//    em_TUserEmailOrderTerm       = 9, // �����û��ʼ�
//    em_TUserDescriptionOrderTerm = 10,// �����û�������λ
//    em_TUser2DomainNOOrderTerm   = 11,// �����û�������ȫ�ֱ�� 
//    em_TUserExpireDateOrderTerm  = 12,// �����û�ʹ�ý�ֹ����
//    em_TUserCreateDateOrderTerm  = 13,// �����û���������
//    em_EndQryOrderTermForTUser   = 14,// ������..
//};

enum ENTasResourceTypes
{
    TasResourceGeneralDev = 1,      // ͨ���豸(���������⴦����豸���ͣ��������ǽ�豸)
    TasResourceTvwallDev  = 2,      // ����ǽ�豸
    TasResourceDevModel   = 3,      // �豸�ͺ�
    TasResourceDevCapIndexName = 4, // �豸������������
    TasResourceDevGrp = 5,          // �豸����
    TasResourceDevGrp2Dev = 6,      // �豸�����豸����
    TasResourceDomainRelation = 7,  // ���ϵ

    TasResourceCuser = 8,           // CU�û�
    TasResourceCuser2Dev = 9,       // CU�û��豸����
    TasResourceCuserGrp = 10,       // CU�û�����
    TasResourceCuserGrp2Dev = 11,   // CU�û������豸����

    TasResourceTuser = 12,               // tac�û�
    TasResourceSyncDstDomainCfg = 13,    // ͬ��Ŀ��������
    TasResourceTmpAuthorizationCfg = 14, // ������Ȩ����

	// ϸ��ͨ���豸֪ͨ [11/24/2015 pangubing]
	TasResourceGeneralDevAdd = 15,      // ͨ���豸����(���������⴦����豸���ͣ��������ǽ�豸)
	TasResourceGeneralDevDel = 16,      // ͨ���豸ɾ��(���������⴦����豸���ͣ��������ǽ�豸)
	TasResourceGeneralDevMod = 17,      // ͨ���豸�޸�(���������⴦����豸���ͣ��������ǽ�豸)
	TasResourcePtzLock  = 18,      // Ptz��

    TasLicenseUpdateNotify = 19,        // license����ʱ֪ͨ����������ʱʵʱ֪ͨ�µ�license��ֵ��

	TasResourceIgnoreGbChildDomainDevChg = 20, // ���Թ��������豸�����ֻҪ�ǹ��������豸��ر����ȫ���ԣ�

	TasResourceEnd,
};
// ����ģ���¼����չ��Ϣ
struct TPSLoginExternInfo
{
public:
	TPSLoginExternInfo()
	{
		needGrpGbid=false;
	}
public:
    set<int> resourceTypes; // ��ע����Դ���ͣ�TAS�ڲ���һЩ���ֻ�������עʱ��֪ͨ����ֵ����ΪENTasResourceTypes
	bool needGrpGbid;  //�Ƿ���Ҫ�������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// ���¼�ͬ����¼����չ��Ϣ
struct TParentSyncLoginExternInfo
{
public:
	string     platVersion;				// ƽ̨�汾��Ϣ

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ���¼�ͬ����¼���ص���չ��Ϣ
struct TParentSyncRspLoginExternInfo
{
public:
	string     platVersion;				// ƽ̨�汾��Ϣ

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CBitsContainer
{
public:
    CBitsContainer(){}
    void SetBit(u32 dwIndex, bool bOn = true)
    {
        CMS_ASSERT(dwIndex < 32);
        char ch = bOn? '1':'0';

        if (bits.size() <= dwIndex)
        {
            bits.resize(dwIndex+1, '0');
        }

        bits[dwIndex] = ch;
    }
    bool IsBitOn(u32 dwIndex) const
    {
        CMS_ASSERT(dwIndex < 32);
        if (bits.size() <= dwIndex)
        {
            return false;
        }
        else
        {
            return bits[dwIndex] == '1';
        }
    }
    u32 Convert2Int() const
    {
        CMS_ASSERT(bits.size() <= 32);
        u32 dwVal = 0;
        for (u32 i = 0; i < bits.size(); ++i)
        {
            if (bits[i] == '1') dwVal |= (0x1 << i);
        }
        return dwVal;
    }
    void ConvertFromInt(u32 dwVal)
    {
        bits.clear();

        u32 dwMask = 0x1;
        for (s32 i = 0; i < 32; ++i)
        {
            if (dwVal & (dwMask<<i))
            {
                SetBit(i);
            }
        }
    }
    void ConvertFromBitsContainer(const CBitsContainer &val)
    {
        bits = val.bits;
    }
    void Clear()
    {
        bits.clear();
    }
    bool IsSuperSet(const CBitsContainer &val) // �ж��Ƿ�Ϊval�ĳ���
    {
        size_t i;

        size_t size = bits.size()<val.bits.size()? bits.size():val.bits.size();
        for (i = 0; i < size; ++i)
        {
            if (bits[i] < val.bits[i]) return false;
        }
        for (; i < val.bits.size(); ++i)
        {
            if (val.bits[i] == '1') return false; 
        }

        return true;
    }
private:
    string bits; // λ��Чʱ��Ӧλ��ֵΪ1������Ϊ0. �˳�Ա��Ҫ�������ⲿ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserPrivilege: public CBitsContainer
{ // cu�û�Ȩ��λ
public:
    CUserPrivilege(){}
    enum TPrivilege
    {
        en_Vod_Rec              = 1, // ƽ̨¼�� ��һλ
        en_Vod_Play             = 2, // ƽ̨����
        en_Vod_Del              = 3, // ƽ̨¼��ɾ��
        en_Vod_Download         = 4, // ƽ̨¼������
        en_Vod_Local_Rec        = 5, // ����¼�񣬱���ץ��
        en_Rec_Query_Remote     = 6, // ����¼���ѯ

        en_Call_Ctrl            = 7,  // ���п���
        en_Alarm_Manage         = 8,  // �ֳ��澯����

        // ������Ȩ����ʱ�ò���, ��ע�͵�
        //en_Explore              = 9,  // ��Ƶ���Ȩ��
        //en_Cycle_Explore        = 10, // ��ѲȨ��
    };
    void SetPrivilege(CUserPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CUserPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserPrivilege: public CBitsContainer
{ // tac�û�Ȩ��λ
public:
    CTacUserPrivilege(){}
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CEncoderPrivilege: public CBitsContainer
{ // ������Ȩ��
public:
    CEncoderPrivilege(){}
    enum TPrivilege
    {
        en_Vod_Rec              = 1, // ǰ��¼��
        en_Vod_Play             = 2, // ǰ�˷���
        en_Vod_Del              = 3, // ǰ��¼��ɾ��
        en_Vod_Download         = 4, // ǰ��¼������

        en_Param_Config         = 5, // ǰ�˲�������
        en_IO_Ctrl              = 6, // ǰ�˿������������
        en_Defence_Ctrl         = 7, // ����/����
    };
    void SetPrivilege(CEncoderPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CEncoderPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePrivilege: public CBitsContainer
{ // ��ƵԴȨ��
public:
    CVideosourcePrivilege(){}
    enum TPrivilege
    {
        en_PTZ_Ctrl              = 1, // �ƾ�����(1-CMS_PTZ_MAX_PRI_LEV)�� ��ռ��CMS_PTZ_MAX_PRI_LEVλ(Ϊ��XML�����Ķ�)
        en_AUDIO_PLAY            = 11, // ��������
		en_PrePos_Set            = 12, // Ԥ��λ����   [12/10/2015 pangubing]
		en_PTZ_Lock              = 13  // PTZ����   [3/28/2016 pangubing]
    };
    void SetPrivilege(CVideosourcePrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CVideosourcePrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
    void SetPTZCtrl(int pri)
    {
        if (pri > CMS_PTZ_MAX_PRI_LEV)
        {
            CMS_ASSERT(false && "ptz privilege extend CMS_PTZ_MAX_PRI_LEV level");
            pri = CMS_PTZ_MAX_PRI_LEV;
        }

        s32 i;
        for (i = 0; i < CMS_PTZ_MAX_PRI_LEV; ++i)
        {
            SetBit(en_PTZ_Ctrl+i-1, false);
        }

        for (i = 1; i <= pri; ++i)
        {
            SetBit(en_PTZ_Ctrl+(i-1)-1);
        }
    }
    int GetPTZCtrl() const
    {
        s32 nPrivi = 0;
        for (s32 i = CMS_PTZ_MAX_PRI_LEV-1; i >= 0; --i)
        {
            if (IsBitOn(en_PTZ_Ctrl+i-1))
            {
                nPrivi = i+1;
                break;
            }
        }
        return nPrivi;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmInputPrivilege: public CBitsContainer
{ // �澯Ȩ��
public:
    CAlarmInputPrivilege(){}
    enum TPrivilege
    {
        em_Alarm_Manage       = 1,   // �澯����
    };
    void SetPrivilege(CAlarmInputPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CAlarmInputPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallPrivilege: public CBitsContainer
{ // ����ǽȨ��, ����ǽ�ľ�������Ȩ��ʹ��α������Ȩ��λȫ����λ
public:
    CTvwallPrivilege(){}
    enum TPrivilege
    {
        em_Tvwall_Manage       = 1,   // ����ǽ����
    };
    void SetPrivilege(CTvwallPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CTvwallPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// �豸Ȩ��λ
class CDevicePrivilege: public CBitsContainer
{
public:
    CDevicePrivilege(){}
	CEncoderPrivilege GetEncoderPrivilege() 
    {
		CEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
		return val;
    }
	CEncoderPrivilege GetEncoderPrivilege() const
    {
		CEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
		return val;
    }
    void SetEncoderPrivilege(const CEncoderPrivilege &val)
    {
        ConvertFromBitsContainer(val);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// �豸��������Ȩ��λ
struct TDeviceCapIndexs
{
    TDeviceCapIndexs()
    {
        id = DEVICE_CAP_INVALID;
    }
    int id;                 // ������ʶ
    vector<int> indexs;     // ��������ռ������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// �豸����Ȩ��λ
struct TDeviceCapPrivilege
{
    TDeviceCapPrivilege()
    {
        id = DEVICE_CAP_INVALID;
    }

    int id;  // ������ʶ
    map<int, CBitsContainer> privileges; // ��������Ȩ��, keyΪ��������, valueΪȨ����ֵ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapInfo
{
    TDeviceCapInfo()
    {
        type = DEVICE_CAP_INVALID;
        num = 0;
        license = 0;
    }
    int type;    // �豸��������
    int num;   // �豸������Ŀ(Ҳ�����ж��ٸ����������������ٸ�����ͨ��)
    int license; // ��ȡlicense��Ŀ
public:
    bool operator==(const TDeviceCapInfo& rh) const
    {
        return (type == rh.type && num == rh.num && license == rh.license);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TDeviceCapExtDataType;
#define DEVICE_CAP_EXT_DATA_TYPE_LON					"Lon"		//����
#define DEVICE_CAP_EXT_DATA_TYPE_LAT					"Lat"		//γ��

struct TDeviceCapExtData
{
public:
	TDeviceCapExtData()
	{

	}

	void Clear()
	{
		extData.clear();
	}

	const map<TDeviceCapExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TDeviceCapExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TDeviceCapExtDataType, string> &val)
	{
		extData = val;
	}

	bool GetLon(double &val) const
	{
		if (!GetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LON, val))
		{
			val = INVALID_LONGITUDE;
		}

		return true;
	}
	void SetLon(double val)
	{
		SetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LON, val);
	}

	bool GetLat(double &val) const
	{
		if (!GetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LAT, val))
		{
			val = INVALID_LATITUDE;
		}

		return true;
	}
	void SetLat(double val)
	{
		SetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LAT, val);
	}

private:
	bool GetDoubleData(const TDeviceCapExtDataType &tType, double &val) const
	{
		map<TDeviceCapExtDataType, string>::const_iterator it = extData.find(tType);
		if (extData.end() != it)
		{
			istringstream iss(it->second);
			iss>>val;
			return true;
		}

		return false;
	}
	void SetDoubleData(const TDeviceCapExtDataType &tType, double val)
	{
		ostringstream oss;
		oss.precision(18);
		oss<<val;
		extData[tType] = oss.str();
	}

private:
	map<TDeviceCapExtDataType, string> extData;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapIndexName
{
    TDeviceCapIndexName()
    {
        index = -1;
    }
    int index;   // �豸��������
    string name; // �豸����������Ӧ������
	TDeviceCapExtData extData;	//�豸������Ӧ����չ��Ϣ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapName
{
    UUIDString deviceId;  // �豸���
	UUIDString devDomainId; // �豸����
    int capId;            // ������ʶ, Ҳ����������
    vector<TDeviceCapIndexName> deviceCapIndexNames;

	void Clear()
	{
		deviceId.clear();
		devDomainId.clear();
		capId = DEVICE_CAP_VIDEOSOURCE;
		deviceCapIndexNames.clear();
	}

    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

	const UUIDString& GetDevDomainId() const
	{
		return devDomainId;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TTacUserType
{// hzytodo
    enTacUserType_SuperAdmin
};

enum TSyncDataType
{
    enDataType_PlatformRelation = 1, // ���¼���ϵ
    enDataType_Device = 2,           // �豸
    enDataType_DeviceGroup = 3,      // �豸����
    enDataType_DeviceGroupDevice = 4,// �豸���黮����Ϣ

    enDataType_Tmp_Device = 5,       // ��ʱ��Ȩ�豸��Ϣ
    enDataType_Tmp_UserDevice = 6,   // ��ʱ��Ȩ�豸����
};
enum TSyncOperateType
{
    enOperateType_Add = 1,  // ���
    enOperateType_Del = 2,  // ɾ��
    enOperateType_Mod = 3,  // �޸�
};

// �豸���Ա�־
class TDevicePropertyFlags: public CBitsContainer
{
public:
    TDevicePropertyFlags(){}
    enum TPropertyFlag
    {
        em_PropertyFlags_PrivateDev = 1,              // ˽���豸
        em_PropertyFlags_P2PDev_DISABLE_REASSIGN = 2, // ��Ȩ�豸�ҽ�ֹ���λ���
        em_PropertyFlags_P2PDev_ABLE_REASSIGN = 3,    // ��Ȩ�豸��������λ���
    };
    void SetPropertyFlag(TDevicePropertyFlags::TPropertyFlag flag)
    {
        SetBit(flag-1);
    }
    bool HasPropertyFlag(TDevicePropertyFlags::TPropertyFlag flag) const
    {
        return IsBitOn(flag-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef s32 TCreater;
#define CREATE_BY_PLAT                  0           // ��ƽ̨�ڲ�����
#define CREATE_BY_CMSSDK                1           // ��cmssdk����
#define CREATE_BY_GBS                   2           // ��gbs������ֻ�ޱ����豸
#define CREATE_BY_GBS_FOREIGN_DOMAIN    3           // ��gbs������ֻ�������豸

typedef string TDevExtDataType;
#define DEV_EXT_FIXADDR					"fixAddr"	// �豸��װλ��
#define DEV_EXT_KDMNO					"kdmNO"		// 1.0�豸���
#define DEV_EXT_GBID					"gbId"		// ������

struct TDevExtData
{
public:
	TDevExtData()
	{
	}

	void Clear()
	{
		extData.clear();
	}

	const map<TDevExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TDevExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TDevExtDataType, string> &val)
	{
		extData = val;
	}

private:
	map<TDevExtDataType, string> extData;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceInfo
{
public:
    CDeviceInfo()
    {
        type = DEVICE_TYPE_INVALID;
        usedFlag = true;
        netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
        creater = CREATE_BY_PLAT;
    }

	void Clear()
	{
		deviceId.clear();
		domainId.clear();
		name.clear();
		alias.clear();
		type = DEVICE_TYPE_INVALID;
		modelId.clear();
		propertyFlags.Clear();
		modelName.clear();
		manufacturer.clear();
		modelCapDetails.clear();
		usedFlag = true;
		netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
		netAddress.clear();
		description.clear();
		expireDate.clear();
		createDate.clear();
		creater = CREATE_BY_PLAT;
		devData.Clear();
		groupId.clear();
	}

    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    UUIDString& GetDeviceUUID() 
    {
        return deviceId;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
    }
    const UUIDString& GetDomainUUID() const
    {
        return domainId;
    }
    UUIDString& GetDomainUUID()
    {
        return domainId;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainId = val;
    }
    const string& GetDeviceName() const
    {
        return name;
    }
    string& GetDeviceName() 
    {
        return name;
    }
    void SetDeviceName(const string &val)
    {
        name = val;
    }
    const string& GetDeviceAlias() const
    {
        return alias;
    }
    string& GetDeviceAlias()
    {
        return alias;
    }
    void SetDeviceAlias(const string &val)
    {
        alias = val;
    }
	int GetDeviceType() 
    {
		return type;
    }
    int GetDeviceType() const
    {
        return type;
    }
    void SetDeviceType(int val)
    {
        type = val;
    }
    const UUIDString& GetDeviceModel() const
    {
        return modelId;
    }
    UUIDString& GetDeviceModel() 
    {
        return modelId;
    }
    void SetDeviceModel(const UUIDString &val)
    {
        modelId = val;
    }
    const TDevicePropertyFlags& GetDevicePropertyFlags() const
    {
        return propertyFlags;
    }
    TDevicePropertyFlags& GetDevicePropertyFlags()
    {
        return propertyFlags;
    }
    void SetPropertyFlags(const TDevicePropertyFlags& val)
    {
        propertyFlags = val;
    }
    vector<TDeviceCapInfo>& GetDeviceModelCapDetail()
    {
        return modelCapDetails;
    }
    const vector<TDeviceCapInfo>& GetDeviceModelCapDetail() const
    {
        return modelCapDetails;
    }
	void SetDeviceModelCapDetail(const vector<TDeviceCapInfo>& val)
	{
		modelCapDetails = val;
	}
	bool GetUsedFlag() 
    {
		return usedFlag;
    }
	const bool GetUsedFlag() const
    {
        return usedFlag;
    }
    void SetUsedFlag(bool val)
    {
        usedFlag = val;
    }
	int GetNetType() 
    {
		return netType;
    }
    int GetNetType() const
    {
        return netType;
    }
    void SetNetType(int val)
    {
        netType = val;
    }
    const string& GetNetAddress() const
    {
        return netAddress;
    }
    string& GetNetAddress()
    {
        return netAddress;
    }
    void SetNetAddress(const string &val)
    {
        netAddress = val;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    const string& GetModelName() const
    {
        return modelName;
    }
    string& GetModelName()
    {
        return modelName;
    }
    void SetModelName(const string &val)
    {
        modelName = val;
    }
    const string& GetDescription() const
    {
        return description;
    }
    string& GetDescription()
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    const string& GetExpireDate() const
    {
        return expireDate;
    }
    string& GetExpireDate()
    {
        return expireDate;
    }
    void SetExpireDate(const string &val)
    {
        expireDate = val;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    string& GetCreateDate()
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const TDevExtData &GetDevData() const
	{
		return devData;
	}
	TDevExtData &GetDevData()
	{
		return devData;
	}
	void SetDevData(const TDevExtData &val)
	{
		devData = val;
	}

	const UUIDString &GetOwner() const
	{
		return owner;
	}
	UUIDString &GetOwner()
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}

	void SetGroupId(const UUIDString& strGroupId)
	{
		groupId = strGroupId;
	}
	UUIDString& GetGroupId()
	{
		return groupId;
	}
	const UUIDString& GetGroupId() const
	{
		return groupId;
	}
private:
    UUIDString deviceId; // �豸UUIDString
    UUIDString domainId; // �豸��������
    string name;      // �豸��
    string alias;     // �豸����
    int type;         // �豸���ͣ��μ���������
    UUIDString modelId; // �豸�ͺ�
    TDevicePropertyFlags propertyFlags; // �豸���Ա�־
    string modelName;    // �豸�ͺ����ƣ����豸�ͺ�����������ڷ��㿼�ǣ�3AS�����豸�ͺŶ�̬ƴ�ӳ���
    string manufacturer; // �豸������, ���豸�ͺ�����������ڷ��㿼�ǣ�3AS�����豸�ͺŶ�̬ƴ�ӳ���
    vector<TDeviceCapInfo> modelCapDetails; // �豸�ͺž����������ݣ����ڷ���Ŀ��ǣ�3AS�����豸�ͺŶ�̬ƴ�ӳ���
    bool usedFlag; // ��ͣ״̬��־��0ͣ�ã���0����
    int netType;   // ������·��ʽ���μ���������
    string netAddress;   // �豸�����ַ��IP������
    string description;  // �豸����
    string expireDate;   // ʹ�ý�ֹ����
    string createDate;   // ��������
    TCreater creater;    // ������
	TDevExtData devData; // �豸��չ��Ϣ
	UUIDString owner;    // ������
	UUIDString groupId;  // �豸�������ţ�����ָ���豸��������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceInfoQueryCondition
{
public:
    CDeviceInfoQueryCondition()
    {
        usedFlag = true;
        netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceUUID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetDeviceUUIDBegin(UUIDString &val) const
    {
        if (!optionField.HasOption("idBegin"))
            return false;
        val = idBegin;
        return true;
    }
    void SetDeviceUUIDBegin(const UUIDString &val)
    {
        idBegin = val;
        optionField.SetOption("idBegin");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
    bool GetDeviceName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetDeviceName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    /*bool GetDeviceNameBegin(string &val) const
    {
        if (!optionField.HasOption("nameBegin"))
            return false;
        val = nameBegin;
        return true;
    }
    void SetDeviceNameBegin(const string &val)
    {
        nameBegin = val;
        optionField.SetOption("nameBegin");
    }*/
    bool GetDeviceAlias(string &val) const
    {
        if (!optionField.HasOption("alias"))
            return false;
        val = alias;
        return true;
    }
    void SetDeviceAlias(const string &val)
    {
        alias = val;
        optionField.SetOption("alias");
    }
    bool GetDeviceType(set<int> &val) const
    {
        if (!optionField.HasOption("types"))
            return false;
        val = types;
        return true;
    }
    void SetDeviceType(const set<int> &val)
    {
        types = val;
        optionField.SetOption("types");
    }
    bool GetDeviceModelId(UUIDString &val) const
    {
        if (!optionField.HasOption("modelId"))
            return false;
        val = modelId;
        return true;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        modelId = val;
        optionField.SetOption("modelId");
    }
    bool GetUsedFlag(bool &val) const
    {
        if (!optionField.HasOption("usedFlag"))
            return false;
        val = usedFlag;
        return true;
    }
    void SetUsedFlag(bool val)
    {
        usedFlag = val;
        optionField.SetOption("usedFlag");
    }
    bool GetNetType(int &val) const
    {
        if (!optionField.HasOption("netType"))
            return false;
        val = netType;
        return true;
    }
	void SetNetType(int val)
    {
        netType = val;
        optionField.SetOption("netType");
    }
    bool GetNetAddress(string &val) const
    {
        if (!optionField.HasOption("netAddress"))
            return false;
        val = netAddress;
        return true;
    }
    void SetNetAddress(const string &val)
    {
        netAddress = val;
        optionField.SetOption("netAddress");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("expireDate_From"))
            return false;
        val = expireDate_From;
        return true;
    }
    void SetExpireDate_From(const string &val)
    {
        expireDate_From = val;
        optionField.SetOption("expireDate_From");
    }
    bool GetExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("expireDate_To"))
            return false;
        val = expireDate_To;
        return true;
    }
    void SetExpireDate_To(const string &val)
    {
        expireDate_To = val;
        optionField.SetOption("expireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreaters(set<TCreater> &val) const
    {
        if (!optionField.HasOption("creaters"))
            return false;
        val = creaters;
        return true;
    }
    void SetCreaters(const set<TCreater> &val)
    {
        creaters = val;
        optionField.SetOption("creaters");
    }
	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
		optionField.SetOption("owner");
	}
	bool GetNeedSearchIp(string &val) const
	{
		if (!optionField.HasOption("needSearchIp"))
			return false;
		val = needSearchIp;
		return true;
	}
	void SetNeedSearchIp(const string &val)
	{
		needSearchIp = val;
		optionField.SetOption("needSearchIp");
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;          // �����ѯ��CU�û���ţ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵ�

    UUIDString id; // �豸Id
    UUIDString idBegin; // �豸Id����idBegin
    UUIDString domainId; // ��������
    string name;   // �豸��
    //string nameBegin; // �豸������nameBegin  ָ����ʱͬʱҪ��ָ��idBegin�ֶΣ���Ϊ�豸����Ψһ
    string alias;  // �豸����
    set<int> types;     // �豸���ͼ��ϣ�֧��һ��ָ��������ͣ�
    UUIDString modelId; // �豸�ͺ�ID
    bool usedFlag; // ��ͣ״̬��־��0ͣ�ã���0����
    int netType;   // ������·��ʽ���μ���������
    string netAddress; // �豸�����ַ��IP������
    string description;  // �豸����
    string expireDate_From; // ʹ�ý�ֹ����_��ѯ���
    string expireDate_To;   // ʹ�ý�ֹ����_��ѯ�յ�
    string createDate_From; // ��������_��ѯ���
    string createDate_To;   // ��������_��ѯ�յ�
    set<TCreater> creaters;       // �����߼��ϣ�֧��һ��ָ����������ߣ�
	UUIDString owner;         // ������
	string needSearchIp;                     // �Ƿ������������ַnetAddress�ֶΣ���ȡֵ��"true"/"false"
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameQryCondition
{
public:
    CDeviceCapIndexNameQryCondition(){}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceModelInfo
{
public:
    CDeviceModelInfo()
    {
        typeId = DEVICE_TYPE_INVALID;
        creater = CREATE_BY_PLAT;
    }

	void Clear()
	{
		id.clear();
		domainId.clear();
		typeId = DEVICE_TYPE_INVALID;
		name.clear();
		manufacturer.clear();
		capInfos.clear();
		creater = CREATE_BY_PLAT;
	}

    UUIDString& GetDeviceModelId() 
    {
        return id;
    }
    const UUIDString& GetDeviceModelId() const
    {
        return id;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        id = val;
    }
    int GetDeviceTypeId() 
    {
        return typeId;
    }
    int GetDeviceTypeId() const
    {
        return typeId;
    }
    void SetDeviceTypeId(int val)
    {
        typeId = val;
    }
    UUIDString& GetDomainId() 
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    } 
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
    }
    string& GetDeviceModelName() 
    {
        return name;
    }
    const string& GetDeviceModelName() const
    {
        return name;
    }
    void SetDeviceModelName(const string &val)
    {
        name = val;
    }
    const vector<TDeviceCapInfo>& GetDeviceCapInfo() const
    {
        return capInfos;
    }
    vector<TDeviceCapInfo>& GetDeviceCapInfo()
    {
        return capInfos;
    }
	void SetDeviceCapInfo(const vector<TDeviceCapInfo>& val)
	{
		capInfos = val;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const UUIDString& GetOwner() const
	{
		return owner;
	}
	UUIDString& GetOwner() 
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}
private:
    UUIDString id;      // �豸�ͺű��
    UUIDString domainId; // ��������
    int typeId;         // �����豸����
    string name;         // �豸�ͺ�����
    string manufacturer;  // �豸������Ϣ
    vector<TDeviceCapInfo> capInfos; // �豸������Ϣ
    TCreater creater;       // ������
	UUIDString owner;       // ������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelQueryCondition
{
public:
    CDeviceModelQueryCondition()
    {
        devTypeId = DEVICE_TYPE_INVALID;
    }
    bool GetModelId(UUIDString &val) const
    {
        if (!optionField.HasOption("modelId"))
            return false;
        val = modelId;
        return true;
    }
    void SetModelId(const UUIDString &val)
    {
        modelId = val;
        optionField.SetOption("modelId");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
    bool GetDevTypeId(int &val) const
    {
        if (!optionField.HasOption("devTypeId"))
            return false;
        val = devTypeId;
        return true;
    }
    void SetDevTypeId(int val)
    {
        devTypeId = val;
        optionField.SetOption("devTypeId");
    }
    bool GetCreater(TCreater &val) const
    {
        if (!optionField.HasOption("creater"))
            return false;
        val = creater;
        return true;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
        optionField.SetOption("creater");
    }

	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(UUIDString val)
	{
		owner = val;
		optionField.SetOption("owner");
	}

	void SetModelName(const string& strModelName)
	{
		modelName = strModelName;
		optionField.SetOption("modelName");
	}
	bool GetModelName(string &val) const
	{
		if (!optionField.HasOption("modelName"))
			return false;
		val = modelName;
		return true;
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString modelId;
    UUIDString domainId;    // ��������
    int devTypeId;          // �����豸����
    TCreater creater;       // ������
	UUIDString owner;       // ������
	string modelName;       //�豸�ͺţ�֧��ģ����ѯ             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceManufacturerInfo
{
public:
    CDeviceManufacturerInfo()
    {
        buildin = true;
    }

	void Clear()
	{
		manufacturer.clear();
		buildin = false;
	}

    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    bool GetBuildIn() 
    {
        return buildin;
    }
	bool GetBuildIn() const
    {
        return buildin;
    }
    void SetBuildIn(bool val)
    {
        buildin = val;
    }
private:
    string manufacturer;
    bool buildin; // �Ƿ�Ϊ���ó��̣����ó��̲���ɾ��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserInfo
{
public:
    CUserInfo()
    {
        type = CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
		pwdExpireEnabledFlag = false;
		pwdExpireTime = 90;
		ipListEnabledFlag = false;
		ipBindFlagForMobileClient = false;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
    }

	void Clear()
	{
		id.clear();
		grpID.clear();
		name.clear();
		password.clear();
		type = CUSER_TYPE_SUPERADMIN;
		mLoginFlag = true;
		enabledFlag = true;
		description.clear();
		nameExpireDate.clear();
		pwdExpireDate.clear();
		pwdExpireEnabledFlag = false;
		pwdExpireTime = 90;
		createDate.clear();
		creatorID.clear();
		privilege.Clear();
		deviceTreeVer.clear();
		loginIpList.clear();
		ipListEnabledFlag = false;
		ipBindFlagForMobileClient = false;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
	}

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
    UUIDString& GetUsergrpID() 
    {
        return grpID;
    }
    const UUIDString& GetUsergrpID() const
    {
        return grpID;
    }
    void SetUsergrpID(const UUIDString &val)
    {
        grpID = val;
    }
    string& GetUserName() 
    {
        return name;
    }
    const string& GetUserName() const
    {
        return name;
    }
    void SetUserName(const string &val)
    {
        name = val;
    }
    string& GetUserPwd() 
    {
        return password;
    }
    const string& GetUserPwd() const
    {
        return password;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
    }
    int GetUserType() 
    {
        return type;
    }
    int GetUserType() const
    {
        return type;
    }
    void SetUserType(int val)
    {
        type = val;
    }
    bool GetMLoginFlag() 
    {
        return mLoginFlag;
    }
	const bool GetMLoginFlag() const
    {
        return mLoginFlag;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
    }
    bool GetEnabledFlag() 
    {
        return enabledFlag;
    }
    bool GetEnabledFlag() const
    {
        return enabledFlag;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetNameExpireDate() 
    {
        return nameExpireDate;
    }
    const string& GetNameExpireDate() const
    {
        return nameExpireDate;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
    }
    string& GetPwdExpireDate() 
    {
        return pwdExpireDate;
    }
    const string& GetPwdExpireDate() const
    {
        return pwdExpireDate;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    UUIDString& GetCreatorID()
    {
        return creatorID;
    }
    const UUIDString& GetCreatorID() const
    {
        return creatorID;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
    }
    CUserPrivilege& GetUserPrivilege() 
    {
        return privilege;
    }
    const CUserPrivilege& GetUserPrivilege() const
    {
        return privilege;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        privilege = val;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }

	void SetPwdExpireEnabledFlag(bool bPwdExpireEnabledFlag)
	{
		pwdExpireEnabledFlag = bPwdExpireEnabledFlag;
	}
	bool GetPwdExpireEnabledFlag() const
	{
		return pwdExpireEnabledFlag;
	}

	void SetPwdExpireTime(int nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
	}
	int GetPwdExpireTime() const
	{
		return pwdExpireTime;
	}

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
	}
	string& GetLoginIpList()
	{
		return loginIpList;
	}
	const string& GetLoginIpList() const
	{
		return loginIpList;
	}
	void SetIpListEnabledFlag(bool bIpListEnabledFlag)
	{
		ipListEnabledFlag = bIpListEnabledFlag;
	}
	bool GetIpListEnabledFlag() const
	{
		return ipListEnabledFlag;
	}
	void SetIpBindFlagForMobileClient(bool bIpBindFlagForMobileClient)
	{
		ipBindFlagForMobileClient = bIpBindFlagForMobileClient;
	}
	bool GetIpBindFlagForMobileClient() const
	{
		return ipBindFlagForMobileClient;
	}
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
	}
	int GetSwitchNum() const
	{
		return switchNum;
	}

	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
	}
	bool GetWaterMarkEnabled() const
	{
		return waterMarkEnabled;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
	}
	string& GetWaterMarkFont()
	{
		return waterMarkFont;
	}
	const string& GetWaterMarkFont() const
	{
		return waterMarkFont;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
	}
	int GetWaterMarkFontSize() const
	{
		return waterMarkFontSize;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
	}
	int GetWaterMarkRotationAngle() const
	{
		return waterMarkRotationAngle;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
	}
	int GetWaterMarkColor() const
	{
		return waterMarkColor;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
	}
	int GetWaterMarkTransparent() const
	{
		return waterMarkTransparent;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
	}
	bool GetWaterMarkTiled() const
	{
		return waterMarkTiled;
	}
private:
    UUIDString id;    // �û����
    UUIDString grpID; // �û�������
    string name;     // �û���
    string password; // ��½����, ������ʽ
    int type;        // CU�û����ͣ��μ���������
    bool mLoginFlag; // ��ε�½��־, 1��ʾ������˵�¼
    bool enabledFlag;   // ��ͣ״̬��־
    string description; // ����
    string nameExpireDate; // �ʻ�ʹ�ý�ֹ����
    string pwdExpireDate;  // ����ʹ�ý�ֹ����
	bool   pwdExpireEnabledFlag; //������Ч���Ƿ�����
	int    pwdExpireTime;   //������Ч��ʱ��
	string loginIpList;    //�󶨵�IP��ַ
	bool   ipListEnabledFlag; //��IP�б��Ƿ�����              
    string createDate;    // ��������
    UUIDString creatorID; // �����߱��
    CUserPrivilege privilege;
    string deviceTreeVer; // �豸���汾��
	bool ipBindFlagForMobileClient;//�ƶ��ͻ���ip���Ƿ�����                
	int switchNum;               //����������
	//��Ƶˮӡ����
	bool waterMarkEnabled;                    //�Ƿ�����ˮӡ
	string waterMarkFont;                     //ˮӡ����
	int waterMarkFontSize;                    //�����С
	int waterMarkRotationAngle;               //��ת�Ƕ�
	int waterMarkColor;                       //��ɫ
	int waterMarkTransparent;                 //͸����
	bool waterMarkTiled;                      //�Ƿ�ƽ��

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CUserInfoQueryCondition
{
public:
    CUserInfoQueryCondition()
    {
        type = CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
		pwdExpireEnableFlag = false;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserPwd(string &val) const
    {
        if (!optionField.HasOption("password"))
            return false;
        val = password;
        return true;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
        optionField.SetOption("password");
    }
    bool GetUserType(int &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(int val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreatorNO(UUIDString &val) const
    {
        if (!optionField.HasOption("creatorNO"))
            return false;
        val = creatorNO;
        return true;
    }
    void SetCreatorNO(const UUIDString &val)
    {
        creatorNO = val;
        optionField.SetOption("creatorNO");
    }
    bool GetNameExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_From"))
            return false;
        val = nameExpireDate_From;
        return true;
    }
    void SetNameExpireDate_From(const string &val)
    {
        nameExpireDate_From = val;
        optionField.SetOption("nameExpireDate_From");
    }
    bool GetNameExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_To"))
            return false;
        val = nameExpireDate_To;
        return true;
    }
    void SetNameExpireDate_To(const string &val)
    {
        nameExpireDate_To = val;
        optionField.SetOption("nameExpireDate_To");
    }
    bool GetPwdExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_From"))
            return false;
        val = pwdExpireDate_From;
        return true;
    }
    void SetPwdExpireDate_From(const string &val)
    {
        pwdExpireDate_From = val;
        optionField.SetOption("pwdExpireDate_From");
    }
    bool GetPwdExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_To"))
            return false;
        val = pwdExpireDate_To;
        return true;
    }
    void SetPwdExpireDate_To(const string &val)
    {
        pwdExpireDate_To = val;
        optionField.SetOption("pwdExpireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
	void SetPwdExpireEnableFlag(bool bPwdExpireEnableFlag)
	{
		pwdExpireEnableFlag = bPwdExpireEnableFlag;
		optionField.SetOption("pwdExpireEnableFlag");
	}
	bool GetPwdExpireEnableFlag(bool &val) const
	{
		if (!optionField.HasOption("pwdExpireEnableFlag"))
			return false;
		val = pwdExpireEnableFlag;
		return true;
	}

	void SetPwdExpireTime(s32 nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
		optionField.SetOption("pwdExpireTime");
	}
	bool GetPwdExpireTime(s32 &val) const
	{
		if (!optionField.HasOption("pwdExpireTime"))
			return false;
		val = pwdExpireTime;
		return true;
	}

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
		optionField.SetOption("loginIpList");
	}

	bool GetLoginIpList(string &val) const
	{
		if (!optionField.HasOption("loginIpList"))
			return false;
		val = loginIpList;
		return true;
	}

private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;          // �����ѯ��CU�û���ţ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵ�(Ŀǰָ�����ֶ�ʱ��֧���������������!!!)

    UUIDString id; // �û����
    string name; // �û���
    string password; // ��½����, ������ʽ
    int type; // CU�û����ͣ��μ���������
    bool mLoginFlag; // ��ε�½��־
    bool enabledFlag; // ��ͣ״̬��־
	bool pwdExpireEnableFlag;  //������Ч����ͣ��־
	s32  pwdExpireTime;   //������Ч��ʱ�䣬��λ����
	string loginIpList; //�󶨵�IP                             
    string description; // ����
    UUIDString creatorNO; // �����߱��
    string nameExpireDate_From; //�ʻ�ʹ�ý�ֹ����_��ѯ���
    string nameExpireDate_To; //�ʻ�ʹ�ý�ֹ����_��ѯ�յ�
    string pwdExpireDate_From; //����ʹ�ý�ֹ����_��ѯ���
    string pwdExpireDate_To; //����ʹ�ý�ֹ����_��ѯ�յ�
    string createDate_From; //�ʻ���������_��ѯ���
    string createDate_To; //�ʻ���������_��ѯ�յ�
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CUserDeviceInfo
{
public:
    CUserDeviceInfo(){}

	void Clear()
	{
		userID.clear();
		deviceID.clear();
		devDomainID.clear();
		devicePrivilege.Clear();
		deviceCapPrivilege.clear();
	}

    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }
    void SetDevDomainID(const UUIDString& val)
    {
        devDomainID = val;
    }
    
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }
    void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
    {
        deviceCapPrivilege = val;
    }

private:
    UUIDString userID;
    UUIDString deviceID;
    UUIDString devDomainID; // �豸������Id, �����ͻ���ʱTASƴװ����
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserDeviceInfoKey
{
public:
    CUserDeviceInfoKey(){}
    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapset;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapset;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapset = val;
    }
private:
    UUIDString userID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapset;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserDeviceInfoQueryCondition
{
public:
    CUserDeviceInfoQueryCondition()
    {
        capID = DEVICE_CAP_INVALID;
        capIndex = -1;
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("userID"))
            return false;
        val = userID;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
        optionField.SetOption("userID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetCapID(int &val) const
    {
        if (!optionField.HasOption("capID"))
            return false;
        val = capID;
        return true;
    }
    void SetCapID(int val)
    {
        capID = val;
        optionField.SetOption("capID");
    }
    bool GetCapIndex(int &val) const
    {
        if (!optionField.HasOption("capIndex"))
            return false;
        val = capIndex;
        return true;
    }
    void SetCapIndex(int val)
    {
        capIndex = val;
        optionField.SetOption("capIndex");
    }

    bool GetDevGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("devGrpID"))
            return false;
        val = devGrpID;
        return true;
    }
    void SetDevGrpID(const UUIDString &val)
    {
        devGrpID = val;
        optionField.SetOption("devGrpID");
    }

	bool GetDevTreeID(UUIDString& val) const
	{
		if (!optionField.HasOption("devTreeID"))
			return false;
		val = devTreeID;
		return true;
	}

	void SetDevTreeID(const UUIDString& val)
	{
		devTreeID = val;
		optionField.SetOption("devTreeID");
	}

private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString userID;
    UUIDString deviceID;
    int capID;    // �豸����ID
    int capIndex; // �豸��������

    UUIDString devGrpID;	// �豸����������
	UUIDString devTreeID;	// �豸��ID 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TDevGroupType
{
	TDEV_GROUP_TYPE_UNKNOWN = 0,    //δ֪���ͣ������ϰ汾cu����̨ͳһ�������������ӷ��鴦��
	TDEV_GROUP_TYPE_CIVILCODE ,     //���������ӷ���
	TDEV_GROUP_TYPE_VIRTUALGRP,     //������֯�ӷ���
};

class CDeviceGroupInfo
{
public:
    CDeviceGroupInfo()
    {
        creater = CREATE_BY_PLAT;
		grpseq=0.0;
		Clear();
    }

	void Clear()
	{
		id.clear();
		parentId.clear();
		domainUUID.clear();
		parentDomainUUID.clear();
		name.clear();
		description.clear();
		createDate.clear();
		creater = CREATE_BY_PLAT;
		owner.clear();
		grpseq=0.0;
		fullpath.clear();
		devtreeId.clear();
		oldgbid.clear();
		newgbid.clear();
		grpType = TDEV_GROUP_TYPE_UNKNOWN;
	}

    bool IsAuthorizationDevGrp() const
    {
        return 0 == strcmp(description.c_str(), "domain_authorization_device_group");
    }

    UUIDString& GetDeviceGroupID() 
    {
        return id;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return id;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
    }
    UUIDString& GetDeviceParentGroupID() 
    {
        return parentId;
    }
    const UUIDString& GetDeviceParentGroupID() const
    {
        return parentId;
    }
    void SetDeviceParentGroupID(const UUIDString &val)
    {
        parentId = val;
    }
    UUIDString& GetDomainUUID()
    {
        return domainUUID;
    }
    const UUIDString& GetDomainUUID() const
    {
        return domainUUID;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainUUID = val;
    }
	UUIDString& GetParentDomainUUID()
	{
		return parentDomainUUID;
	}
	const UUIDString& GetParentDomainUUID() const
	{
		return parentDomainUUID;
	}
	void SetParentDomainUUID(const UUIDString &val)
	{
		parentDomainUUID = val;
	}
    string& GetDeviceGroupName()
    {
        return name;
    }
    const string& GetDeviceGroupName() const
    {
        return name;
    }
    void SetDeviceGroupName(const string &val)
    {
        name = val;
    }
    string& GetDeviceGroupDesc() 
    {
        return description;
    }
    const string& GetDeviceGroupDesc() const
    {
        return description;
    }
    void SetDeviceGroupDesc(const string &val)
    {
        description = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const UUIDString& GetOwner() const
	{
		return owner;
	}
	UUIDString& GetOwner() 
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}

	void SetGrpseq(double nGrpseq)
	{
		grpseq = nGrpseq;
	}
	double GetGrpseq() const
	{
		return grpseq;
	}

	void SetFullpath(const string& strFullpath)
	{
		fullpath = strFullpath;
	}
	string& GetFullpath()
	{
		return fullpath;
	}
	const string& GetFullpath() const
	{
		return fullpath;
	}

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString id;
    UUIDString parentId;   // ��Ϊ���ڸ���ʱ����ֵΪ��
    UUIDString domainUUID; // �豸����������
	UUIDString parentDomainUUID;	// ������������
    string name;
    string description;
    string createDate;
    TCreater creater;       // ������
	UUIDString owner;       // ������
	double     grpseq;       //�豸�������ڲ㼶�����
	string	fullpath;        //TAS�ڲ�ʹ�ã�����ģ�鲻Ҫʹ��
	UUIDString devtreeId;    //�豸�������豸��Id
	string  oldgbid;                                //����ѡ������ר�ã������޸ķ���ID
	string  newgbid;                
	TDevGroupType grpType;                  //�������ͣ�������ѡ����������Ч
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetOldgbid(const string& strOldgbid)
    {
        oldgbid = strOldgbid;
    }
    string& GetOldgbid()
    {
        return oldgbid;
    }
    const string& GetOldgbid() const
    {
        return oldgbid;
    }

    void SetNewgbid(const string& strNewgbid)
    {
        newgbid = strNewgbid;
    }
    string& GetNewgbid()
    {
        return newgbid;
    }
    const string& GetNewgbid() const
    {
        return newgbid;
    }
public:
    void SetGrpType(TDevGroupType eGrpType)
    {
        grpType = eGrpType;
    }
    TDevGroupType GetGrpType() const
    {
        return grpType;
    }

};

class CDeviceGroupInfoKey
{
public:
    CDeviceGroupInfoKey(){}

    UUIDString& GetDeviceGroupID() 
    {
        return id;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return id;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
    }

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString id;
	UUIDString devtreeId;               // �豸�������豸��ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceGroupInfoQueryCondition
{
public:
    CDeviceGroupInfoQueryCondition()
    {
    }

    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetDeviceParentGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("parentId"))
            return false;
        val = parentId;
        return true;
    }
    void SetDeviceParentGroupID(const UUIDString &val)
    {
        parentId = val;
        optionField.SetOption("parentId");
    }
    bool GetDomainUUID(UUIDString &val) const
    {
        if (!optionField.HasOption("domainUUID"))
            return false;
        val = domainUUID;
        return true;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainUUID = val;
        optionField.SetOption("domainUUID");
    }
    bool GetDeviceGroupName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetDeviceGroupName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetDeviceGroupDesc(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDeviceGroupDesc(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreaters(set<TCreater> &val) const
    {
        if (!optionField.HasOption("creaters"))
            return false;
        val = creaters;
        return true;
    }
    void SetCreaters(const set<TCreater> &val)
    {
        creaters = val;
        optionField.SetOption("creaters");
    }

	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
		optionField.SetOption("owner");
	}

	bool GetDevTreeId(string &val) const
	{
		if (!optionField.HasOption("devtreeId"))
			return false;
		val = devtreeId;
		return true;
	}
	void SetDevTreeId(const string &val)
	{
		devtreeId = val;
		optionField.SetOption("devtreeId");
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;  // ָ���˶���ʱ3as��������¸�CU�û��Ƿ�ӵ���豸��������������Ƿ�ӵ���豸

    UUIDString id;
    UUIDString parentId; // ���ô�ֵʱ��ʾֻ��ѯ���Ķ����飬����ֵΪ��ʱ��ʾ��ѯ����
    UUIDString domainUUID;
    string name;
    string description;
    string createDate_From;
    string createDate_To;
    set<TCreater> creaters;       // �����߼��ϣ�֧��һ��ָ����������ߣ�
	UUIDString owner;    // ������
	UUIDString devtreeId;   // �豸�������豸��ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



struct TDeviceGbid4Gb
{
	UUIDString oldprimarygbid;                              
	UUIDString oldsecondarygbid;               
	UUIDString newprimarygbid;               
	UUIDString newsecondarygbid;               
public:

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOldprimarygbid(const UUIDString& strOldprimarygbid)
    {
        oldprimarygbid = strOldprimarygbid;
    }
    UUIDString& GetOldprimarygbid()
    {
        return oldprimarygbid;
    }
    const UUIDString& GetOldprimarygbid() const
    {
        return oldprimarygbid;
    }

    void SetOldsecondarygbid(const UUIDString& strOldsecondarygbid)
    {
        oldsecondarygbid = strOldsecondarygbid;
    }
    UUIDString& GetOldsecondarygbid()
    {
        return oldsecondarygbid;
    }
    const UUIDString& GetOldsecondarygbid() const
    {
        return oldsecondarygbid;
    }

    void SetNewprimarygbid(const UUIDString& strNewprimarygbid)
    {
        newprimarygbid = strNewprimarygbid;
    }
    UUIDString& GetNewprimarygbid()
    {
        return newprimarygbid;
    }
    const UUIDString& GetNewprimarygbid() const
    {
        return newprimarygbid;
    }

    void SetNewsecondarygbid(const UUIDString& strNewsecondarygbid)
    {
        newsecondarygbid = strNewsecondarygbid;
    }
    UUIDString& GetNewsecondarygbid()
    {
        return newsecondarygbid;
    }
    const UUIDString& GetNewsecondarygbid() const
    {
        return newsecondarygbid;
    }

};

struct TDeviceInfo4Gb
{
	UUIDString vidSrcName;               
	int vidIndex;               
	TDeviceGbid4Gb gbid;  //�豸��gbid��������������old��newgbid                             
public:
    void SetVidSrcName(const UUIDString& strVidSrcName)
    {
        vidSrcName = strVidSrcName;
    }
    UUIDString& GetVidSrcName()
    {
        return vidSrcName;
    }
    const UUIDString& GetVidSrcName() const
    {
        return vidSrcName;
    }

    void SetVidIndex(int nVidIndex)
    {
        vidIndex = nVidIndex;
    }
    int GetVidIndex() const
    {
        return vidIndex;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbid(const TDeviceGbid4Gb& cGbid)
    {
        gbid = cGbid;
    }
    TDeviceGbid4Gb& GetGbid()
    {
        return gbid;
    }
    const TDeviceGbid4Gb& GetGbid() const
    {
        return gbid;
    }

};

class CDeviceGroupDeviceInfo4Gb
{
public:
	CDeviceGroupDeviceInfo4Gb(){}
	void Clear()
	{
		devGrpId.clear();
		devDevTreeId.clear();
		devName.clear();
		deviceId.clear();
		devinfo.clear();
	}
private:
	UUIDString devGrpId;                
	UUIDString devDevTreeId;               // �豸�����豸��ID
	UUIDString devDomainId;               
	UUIDString devName;               
	UUIDString deviceId;                
	vector<TDeviceInfo4Gb> devinfo;               
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevDevTreeId(const UUIDString& strDevDevTreeId)
    {
        devDevTreeId = strDevDevTreeId;
    }
    UUIDString& GetDevDevTreeId()
    {
        return devDevTreeId;
    }
    const UUIDString& GetDevDevTreeId() const
    {
        return devDevTreeId;
    }

    void SetDevName(const UUIDString& strDevName)
    {
        devName = strDevName;
    }
    UUIDString& GetDevName()
    {
        return devName;
    }
    const UUIDString& GetDevName() const
    {
        return devName;
    }

    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetDevinfo(const vector<TDeviceInfo4Gb>& cDevinfo)
    {
        devinfo = cDevinfo;
    }
    vector<TDeviceInfo4Gb>& GetDevinfo()
    {
        return devinfo;
    }
    const vector<TDeviceInfo4Gb>& GetDevinfo() const
    {
        return devinfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};
class CDeviceGroupDevSeqInfo
{               
	UUIDString devGrpId;               
	UUIDString devId;
	int        vidSrcIdx;               
	double     devSeq;                       
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidSrcIdx(int nVidSrcIdx)
    {
        vidSrcIdx = nVidSrcIdx;
    }
    int GetVidSrcIdx() const
    {
        return vidSrcIdx;
    }

    void SetDevSeq(double nDevSeq)
    {
        devSeq = nDevSeq;
    }
    double GetDevSeq() const
    {
        return devSeq;
    }

};
class CDeviceGroupDeviceInfo
{
public:
    CDeviceGroupDeviceInfo(){}

	void Clear()
	{
		groupId.clear();
		deviceId.clear();
		devGrpId.clear();
		domainId.clear();
		deviceCapIndexs.clear();
		grpDevTreeId.clear();         
		devDevTreeId.clear();
	}

    UUIDString& GetDeviceGroupID() 
    {
        return groupId;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return groupId;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        groupId = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceId;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceId = val;
    }

	void SetDevGrpId(const UUIDString& strDevGrpId)
	{
		devGrpId = strDevGrpId;
	}
	UUIDString& GetDevGrpId()
	{
		return devGrpId;
	}
	const UUIDString& GetDevGrpId() const
	{
		return devGrpId;
	}

    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs>& val)
    {
        deviceCapIndexs = val;
    }
    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetDomainId(const UUIDString& val)
    {
        domainId = val;
    }

	void SetGrpDevTreeId(const UUIDString& strGrpDevTreeId)
	{
		grpDevTreeId = strGrpDevTreeId;
	}
	UUIDString& GetGrpDevTreeId()
	{
		return grpDevTreeId;
	}
	const UUIDString& GetGrpDevTreeId() const
	{
		return grpDevTreeId;
	}

	void SetDevDevTreeId(const UUIDString& strDevDevTreeId)
	{
		devDevTreeId = strDevDevTreeId;
	}
	UUIDString& GetDevDevTreeId()
	{
		return devDevTreeId;
	}
	const UUIDString& GetDevDevTreeId() const
	{
		return devDevTreeId;
	}
private:
    UUIDString groupId;
    UUIDString deviceId;
	UUIDString devGrpId;               
    vector<TDeviceCapIndexs> deviceCapIndexs;

    UUIDString domainId; // ����豸��������Id(ҵ�����豸ֻ�ܱ����鵽ͬ����豸����)�������ͻ���ʱTASƴװ����
	UUIDString grpDevTreeId;                 // �豸�������豸��ID
	UUIDString devDevTreeId;                 // �豸�����豸��ID
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceGroupDeviceInfoKey
{
public:
    CDeviceGroupDeviceInfoKey(){}

    UUIDString& GetDeviceGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return groupID;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
	void SetDeviceCapIndexs(const vector<TDeviceCapIndexs>& val)
	{
		deviceCapIndexs = val;
    }

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString groupID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
	UUIDString devtreeId;  // �豸�������豸��ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceInfoQuery4GbCondition
{
public:
	CDeviceGroupDeviceInfoQuery4GbCondition()
	{
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
	UUIDString devicegroupId;                               
	UUIDString devTreeId;                 
public:
    void SetDevicegroupId(const UUIDString& strDevicegroupId)
    {
        devicegroupId = strDevicegroupId;
		optionField.SetOption("devicegroupId");
    }
	bool GetDevicegroupId(UUIDString& val) const 
	{
		if (!optionField.HasOption("devicegroupId"))
			return false;
		val = devicegroupId;
		return true;
	}
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
		optionField.SetOption("devTreeId");
    }
	bool GetDevTreeId(UUIDString& val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeId;
		return true;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDeviceGroupDeviceInfoQueryCondition
{
public:
    CDeviceGroupDeviceInfoQueryCondition()
    {
        deviceCapID = DEVICE_CAP_INVALID;
        deviceCapIndex = -1;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceGroupID"))
            return false;
        val = deviceGroupID;
        return true;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        deviceGroupID = val;
        optionField.SetOption("deviceGroupID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapID(int &val) const
    {
        if (!optionField.HasOption("deviceCapID"))
            return false;
        val = deviceCapID;
        return true;
    }
    void SetDeviceCapID(int val)
    {
        deviceCapID = val;
        optionField.SetOption("deviceCapID");
    }
    bool GetDeviceCapIndex(int &val) const
    {
        if (!optionField.HasOption("deviceCapIndex"))
            return false;
        val = deviceCapIndex;
        return true;
    }
    void SetDeviceCapIndex(int val)
    {
        deviceCapIndex = val;
        optionField.SetOption("deviceCapIndex");
    }
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;          // �����ѯ��CU�û���ţ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵ�

    UUIDString deviceGroupID;
    UUIDString deviceID;
    int deviceCapID;  
    int deviceCapIndex;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfo
{
public:
    CUserGroupInfo()
    {
        usertype = CUSER_TYPE_SUPERADMIN;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
    }

	void Clear()
	{
		id.clear();
		name.clear();
		description.clear();
		createDate.clear();
		usertype = CUSER_TYPE_SUPERADMIN;
		userPrivilege.Clear();
		createUser.clear();
		deviceTreeVer.clear();
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 72;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
	}

    UUIDString& GetGroupNO() 
    {
        return id;
    }
    const UUIDString& GetGroupNO() const
    {
        return id;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
    }
    string& GetGroupName() 
    {
        return name;
    }
    const string& GetGroupName() const
    {
        return name;
    }
    void SetGroupName(const string &val)
    {
        name = val;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
	int GetUserType() 
	{
		return usertype;
	}
    int GetUserType() const
    {
        return usertype;
    }
    void SetUserType(int val)
    {
        usertype = val;
    }
    CUserPrivilege& GetUserPrivilege() 
    {
        return userPrivilege;
    }
    const CUserPrivilege& GetUserPrivilege() const
    {
        return userPrivilege;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        userPrivilege = val;
    }
    UUIDString& GetCreateUser()
    {
        return createUser;
    }
    const UUIDString& GetCreateUser() const
    {
        return createUser;
    }
    void SetCreateUser(const UUIDString &val)
    {
        createUser = val;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
	}
	int GetSwitchNum() const
	{
		return switchNum;
	}
	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
	}
	bool GetWaterMarkEnabled() const
	{
		return waterMarkEnabled;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
	}
	string& GetWaterMarkFont()
	{
		return waterMarkFont;
	}
	const string& GetWaterMarkFont() const
	{
		return waterMarkFont;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
	}
	int GetWaterMarkFontSize() const
	{
		return waterMarkFontSize;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
	}
	int GetWaterMarkRotationAngle() const
	{
		return waterMarkRotationAngle;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
	}
	int GetWaterMarkColor() const
	{
		return waterMarkColor;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
	}
	int GetWaterMarkTransparent() const
	{
		return waterMarkTransparent;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
	}
	bool GetWaterMarkTiled() const
	{
		return waterMarkTiled;
	}
private:
    UUIDString id;
    string name;
    string description;  // ���� 
    string createDate;   // ��������
    int usertype;        // CU�û����ͣ��μ���������
    CUserPrivilege userPrivilege; // �û�Ȩ��
    UUIDString createUser;        // ������ID
    string deviceTreeVer;         // �豸���汾��
	int switchNum;               //����������
	//��Ƶˮӡ����
	bool waterMarkEnabled;                                   //�Ƿ�����ˮӡ
	string waterMarkFont;                                    //ˮӡ����
	int waterMarkFontSize;                                   //�����С
	int waterMarkRotationAngle;                              //��ת�Ƕ�
	int waterMarkColor;                                      //��ɫ
	int waterMarkTransparent;                                //͸����
	bool waterMarkTiled;                                     //�Ƿ�ƽ��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfoKey
{
public:
    CUserGroupInfoKey(){}

    UUIDString& GetGroupNO() 
    {
        return id;
    }
    const UUIDString& GetGroupNO() const
    {
        return id;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
    }
private:
    UUIDString id;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfoQueryCondition
{
public:
    CUserGroupInfoQueryCondition(){}
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetGroupNO(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetGroupName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetGroupName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;          // �����ѯ��CU�û���ţ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵ�
                                 // �û���û�����ṹ��ָ����IDʱ�����û���������

    UUIDString id;
    string name;
    string description; //����
    string createDate_From; //����������ʼ����
    string createDate_To; //����������ʼ����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfo
{
public:
    CUserGroupDeviceInfo(){}

	void Clear()
	{
		groupID.clear();
		deviceID.clear();
		devDomainID.clear();
		devicePrivilege.Clear();
		capPrivileges.clear();
	}

    UUIDString& GetGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetGroupID() const
    {
        return groupID;
    }
    void SetGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }
    void SetDevDomainID(const UUIDString& val)
    {
        devDomainID = val;
    }
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return capPrivileges;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return capPrivileges;
    }
	void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
	{
		capPrivileges = val;
	}
private:
    UUIDString groupID;
    UUIDString deviceID;
    UUIDString devDomainID; // �豸������ID, �����ͻ���ʱTASƴװ����
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> capPrivileges;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfoKey
{
public:
    CUserGroupDeviceInfoKey(){}

    UUIDString& GetGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetGroupID() const
    {
        return groupID;
    }
    void SetGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs() 
    {
        return deviceCapset;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapset;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapset = val;
    }
private:
    UUIDString groupID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapset;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfoQueryCondition
{
public:
    CUserGroupDeviceInfoQueryCondition()
    {
        capID = DEVICE_CAP_INVALID;
        capIndex = -1;
    }
    bool GetCuserGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserGrpID"))
            return false;
        val = cuserGrpID;
        return true;
    }
    void SetCuserGrpID(const UUIDString &val)
    {
        cuserGrpID = val;
        optionField.SetOption("cuserGrpID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapID(int &val) const
    {
        if (!optionField.HasOption("capID"))
            return false;
        val = capID;
        return true;
    }
    void SetDeviceCapID(int val)
    {
        capID = val;
        optionField.SetOption("capID");
    }
    bool GetDeviceCapIndex(int &val) const
    {
        if (!optionField.HasOption("capIndex"))
            return false;
        val = capIndex;
        return true;
    }
    void SetDeviceCapIndex(int val)
    {
        capIndex = val;
        optionField.SetOption("capIndex");
    }

    bool GetDevGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("devGrpID"))
            return false;
        val = devGrpID;
        return true;
    }
    void SetDevGrpID(const UUIDString &val)
    {
        devGrpID = val;
        optionField.SetOption("devGrpID");
    }

	bool GetDevTreeId(UUIDString &val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeID;
		return true;
	}
	void SetDevTreeId(const UUIDString &val)
	{
		devTreeID = val;
		optionField.SetOption("devTreeId");
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ

    UUIDString cuserGrpID;
    UUIDString deviceID;
    int capID;    // �豸����ID
    int capIndex; // �豸��������

    UUIDString devGrpID; // �豸�����豸������
	UUIDString devTreeID; // �豸�����豸��ID

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceFullGbInfo
{
	UUIDString devId;               
	string     devName;
	string     devDomainId;               
	string     vidSrcName;               
	int        vidIdx;                     //��ƵԴId
	string     oldPriGbid;               
	string     newPriGbid;               
	string     oldSecGbid;               
	string     newSecGbid;               

	vector<CDeviceGroupInfo> devGrpInfos;                //�豸-->������·����ÿ��������ϸ��Ϣ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOldPriGbid(const string& strOldPriGbid)
    {
        oldPriGbid = strOldPriGbid;
    }
    string& GetOldPriGbid()
    {
        return oldPriGbid;
    }
    const string& GetOldPriGbid() const
    {
        return oldPriGbid;
    }

    void SetNewPriGbid(const string& strNewPriGbid)
    {
        newPriGbid = strNewPriGbid;
    }
    string& GetNewPriGbid()
    {
        return newPriGbid;
    }
    const string& GetNewPriGbid() const
    {
        return newPriGbid;
    }

    void SetOldSecGbid(const string& strOldSecGbid)
    {
        oldSecGbid = strOldSecGbid;
    }
    string& GetOldSecGbid()
    {
        return oldSecGbid;
    }
    const string& GetOldSecGbid() const
    {
        return oldSecGbid;
    }

    void SetNewSecGbid(const string& strNewSecGbid)
    {
        newSecGbid = strNewSecGbid;
    }
    string& GetNewSecGbid()
    {
        return newSecGbid;
    }
    const string& GetNewSecGbid() const
    {
        return newSecGbid;
    }

public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

    void SetVidSrcName(const string& strVidSrcName)
    {
        vidSrcName = strVidSrcName;
    }
    string& GetVidSrcName()
    {
        return vidSrcName;
    }
    const string& GetVidSrcName() const
    {
        return vidSrcName;
    }

    void SetVidIdx(int nVidIdx)
    {
        vidIdx = nVidIdx;
    }
    int GetVidIdx() const
    {
        return vidIdx;
    }

public:
    void SetDevGrpInfos(const vector<CDeviceGroupInfo>& cDevGrpInfos)
    {
        devGrpInfos = cDevGrpInfos;
    }
    vector<CDeviceGroupInfo>& GetDevGrpInfos()
    {
        return devGrpInfos;
    }
    const vector<CDeviceGroupInfo>& GetDevGrpInfos() const
    {
        return devGrpInfos;
    }

public:
    void SetDevDomainId(const string& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    string& GetDevDomainId()
    {
        return devDomainId;
    }
    const string& GetDevDomainId() const
    {
        return devDomainId;
    }

};

typedef vector<CDeviceGroupInfo> TDevGrpFullGbInfos ;

class CDomainRelationQueryCondition
{
public:
    CDomainRelationQueryCondition(){}

    bool GetDomainID(UUIDString &val) const
    {
        if (!optionField.HasOption("domainID"))
            return false;
        val = domainID;
        return true;
    }
    void SetDomainID(const UUIDString &val)
    {
        domainID = val;
        optionField.SetOption("domainID");
    }
    bool GetParentDomainID(UUIDString &val) const
    {
        if (!optionField.HasOption("parentDomainID"))
            return false;
        val = parentDomainID;
        return true;
    }
    void SetParentDomainID(const UUIDString &val)
    {
        parentDomainID = val;
        optionField.SetOption("parentDomainID");
    }
    bool GetDomainName(UUIDString &val) const
    {
        if (!optionField.HasOption("domainName"))
            return false;
        val = domainName;
        return true;
    }
    void SetDomainName(const UUIDString &val)
    {
        domainName = val;
        optionField.SetOption("domainName");
    }
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ

    UUIDString domainID;       // ����
    UUIDString parentDomainID; // ������
    string domainName;         // ������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserInfo
{
public:
    CTacUserInfo()
    {
        type = enTacUserType_SuperAdmin;
        mLoginFlag = true;
        enabledFlag = true;
    }

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
    string& GetUserName()
    {
        return name;
    }
    const string& GetUserName() const
    {
        return name;
    }
    void SetUserName(const string &val)
    {
        name = val;
    }
    string& GetUserPwd() 
    {
        return password;
    }
    const string& GetUserPwd() const
    {
        return password;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
    }
	TTacUserType GetUserType() 
    {
		return type;
    }
	TTacUserType GetUserType() const
    {
        return type;
    }
    void SetUserType(TTacUserType val)
    {
        type = val;
    }
    bool GetMLoginFlag() 
    {
        return mLoginFlag;
    }
	bool GetMLoginFlag() const
    {
        return mLoginFlag;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
    }
    bool GetEnabledFlag() 
    {
        return enabledFlag;
    }
    bool GetEnabledFlag() const
    {
        return enabledFlag;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
    }
    string& GetDescription() 
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetNameExpireDate()
    {
        return nameExpireDate;
    }
    const string& GetNameExpireDate() const
    {
        return nameExpireDate;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
    }
    string& GetPwdExpireDate() 
    {
        return pwdExpireDate;
    }
    const string& GetPwdExpireDate() const
    {
        return pwdExpireDate;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    UUIDString& GetCreatorID() 
    {
        return creatorID;
    }
    const UUIDString& GetCreatorID() const
    {
        return creatorID;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
    }
    CTacUserPrivilege& GetTasUserPrivilege()
    {
        return tasUserPrivilege;
    }
    const CTacUserPrivilege& GetTasUserPrivilege() const
    {
        return tasUserPrivilege;
    }
    void SetTasUserPrivilege(const CTacUserPrivilege &val)
    {
        tasUserPrivilege = val;
    }
private:
    UUIDString id; // �û����
    string name; // �û���
    string password; // ��½����, ������ʽ
    TTacUserType type; // �û�����
    bool mLoginFlag; // ��ε�½��־
    bool enabledFlag; // ��ͣ״̬��־
    string description; // ����
    string nameExpireDate; // �ʻ�ʹ�ý�ֹ����
    string pwdExpireDate; // ����ʹ�ý�ֹ����
    string createDate; // ��������
    UUIDString creatorID; // �����߱��
    CTacUserPrivilege tasUserPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTacUserInfoKey
{
public:
    CTacUserInfoKey(){}

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
private:
    UUIDString id; // �û����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTacUserInfoQueryCondition
{
public:
    CTacUserInfoQueryCondition()
    {
        type = enTacUserType_SuperAdmin;
        mLoginFlag = true;
        enabledFlag = true;
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserType(TTacUserType &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(TTacUserType val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetNameExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_From"))
            return false;
        val = nameExpireDate_From;
        return true;
    }
    void SetNameExpireDate_From(const string &val)
    {
        nameExpireDate_From = val;
        optionField.SetOption("nameExpireDate_From");
    }
    bool GetNameExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_To"))
            return false;
        val = nameExpireDate_To;
        return true;
    }
    void SetNameExpireDate_To(const string &val)
    {
        nameExpireDate_To = val;
        optionField.SetOption("nameExpireDate_To");
    }
    bool GetPwdExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_From"))
            return false;
        val = pwdExpireDate_From;
        return true;
    }
    void SetPwdExpireDate_From(const string &val)
    {
        pwdExpireDate_From = val;
        optionField.SetOption("pwdExpireDate_From");
    }
    bool GetPwdExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_To"))
            return false;
        val = pwdExpireDate_To;
        return true;
    }
    void SetPwdExpireDate_To(const string &val)
    {
        pwdExpireDate_To = val;
        optionField.SetOption("pwdExpireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreatorID(UUIDString &val) const
    {
        if (!optionField.HasOption("creatorID"))
            return false;
        val = creatorID;
        return true;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
        optionField.SetOption("creatorID");
    }
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString id; // �û����
    string name; // �û���
    TTacUserType type; // �û����ͣ��μ���������
    bool mLoginFlag;   // ��ε�½��־
    bool enabledFlag;  // ��ͣ״̬��־
    string description; // ����
    string nameExpireDate_From; // �ʻ�ʹ�ý�ֹ������ʼʱ��
    string nameExpireDate_To; // �ʻ�ʹ�ý�ֹ���ڽ���ʱ��
    string pwdExpireDate_From; // ����ʹ�ý�ֹ������ʼʱ��
    string pwdExpireDate_To;   // ����ʹ�ý�ֹ���ڽ���ʱ��
    string createDate_From; // ����������ʼʱ��
    string createDate_To;   // �������ڽ���ʱ��
    UUIDString creatorID;   // �����߱��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTempAuthorizationInfo
{
public:
    CTempAuthorizationInfo(){}

    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }
	void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
	{
		deviceCapPrivilege = val;
	}
    string& GetDesc() 
    {
        return desc;
    }
    const string& GetDesc() const
    {
        return desc;
    }
    void SetDesc(const string &val)
    {
        desc = val;
    }
    string& GetDestinationDomain() 
    {
        return destinationDomain;
    }
    const string& GetDestinationDomain() const
    {
        return destinationDomain;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
    }
    string& GetUsername()
    {
        return username;
    }
    const string& GetUsername() const
    {
        return username;
    }
    void SetUsername(const string &val)
    {
        username = val;
    }
    string& GetCreateTime()
    {
        return createTime;
    }
    const string& GetCreateTime() const
    {
        return createTime;
    }
    void SetCreateTime(const string &val)
    {
        createTime = val;
    }

private:
    UUIDString deviceID;
    UUIDString devDomainId;    // �����ͻ���ʱTASƴװ���� hzytodo δʵ��
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
    string desc;
    string destinationDomain;  // ����Ȩ�û���������
    string username;           // ����Ȩ�û����û���
    string createTime;         // ��TAS����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};
class CTempAuthorizationInfoKey
{
public:
    CTempAuthorizationInfoKey(){}

    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs() 
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapIndexs = val;
    }
    string& GetDestinationDomain()
    {
        return destinationDomain;
    }
    const string& GetDestinationDomain() const
    {
        return destinationDomain;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
    }
    string& GetUserName() 
    {
        return username;
    }
    const string& GetUserName() const
    {
        return username;
    }
    void SetUserName(const string &val)
    {
        username = val;
    }
private:
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
    string destinationDomain;  // ����Ȩ�û���������
    string username;           // ����Ȩ�û����û���
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTempAuthorizationInfoQueryCondition
{
public:
    CTempAuthorizationInfoQueryCondition(){}
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapIndexs(vector<TDeviceCapIndexs> &val) const
    {
        if (!optionField.HasOption("deviceCapIndexs"))
            return false;
        val = deviceCapIndexs;
        return true;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapIndexs = val;
        optionField.SetOption("deviceCapIndexs");
    }
    bool GetDesc(string &val) const
    {
        if (!optionField.HasOption("desc"))
            return false;
        val = desc;
        return true;
    }
    void SetDesc(const string &val)
    {
        desc = val;
        optionField.SetOption("desc");
    }
    bool GetDestinationDomain(string &val) const
    {
        if (!optionField.HasOption("destinationDomain"))
            return false;
        val = destinationDomain;
        return true;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
        optionField.SetOption("destinationDomain");
    }
    bool GetUsername(string &val) const
    {
        if (!optionField.HasOption("username"))
            return false;
        val = username;
        return true;
    }
    void SetUsername(const string &val)
    {
        username = val;
        optionField.SetOption("username");
    }
    bool GetCreateTime_From(string &val) const
    {
        if (!optionField.HasOption("createTime_From"))
            return false;
        val = createTime_From;
        return true;
    }
    void SetCreateTime_From(const string &val)
    {
        createTime_From = val;
        optionField.SetOption("createTime_From");
    }
    bool GetCreateTime_To(string &val) const
    {
        if (!optionField.HasOption("createTime_To"))
            return false;
        val = createTime_To;
        return true;
    }
    void SetCreateTime_To(const string &val)
    {
        createTime_To = val;
        optionField.SetOption("createTime_To");
    }
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString cuserID;          // �����ѯ��CU�û���ţ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵ�

    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
    string desc;
    string destinationDomain;
    string username;
    string createTime_From;
    string createTime_To;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TDomainExtDataType;
#define DOMAIN_EXT_KDMNO        "kdmNO"     // 1.0����
#define DOMAIN_EXT_FIXADDR      "fixAddr"   // ��İ�װλ��

struct TDomainExtData
{
public:
    TDomainExtData()
    {
    }

    void Clear()
    {
        extData.clear();
    }

    bool IsEmpty() const
    {
        return extData.empty();
    }

private:
    map<TDomainExtDataType, string> extData;                   
public:
    void SetExtData(const map<TDomainExtDataType, string>& cExtData)
    {
        extData = cExtData;
    }
    map<TDomainExtDataType, string>& GetExtData()
    {
        return extData;
    }
    const map<TDomainExtDataType, string>& GetExtData() const
    {
        return extData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationInfo
{
public:
    CDomainRelationInfo(){}

	void Clear()
	{
		domainID.clear();
		parentDomainID.clear();
		domainName.clear();
		domainAlias.Clear();
	}

    UUIDString& GetDomainID() 
    {
        return domainID;
    }
    const UUIDString& GetDomainID() const
    {
        return domainID;
    }
    void SetDomainID(const UUIDString &val)
    {
        domainID = val;
    }
    string& GetDomainName() 
    {
        return domainName;
    }
    const string& GetDomainName() const
    {
        return domainName;
    }
    void SetDomainName(const string &val)
    {
        domainName = val;
    }
    UUIDString& GetParentDomainID() 
    {
        return parentDomainID;
    }
    const UUIDString& GetParentDomainID() const
    {
        return parentDomainID;
    }
    void SetParentDomainID(const UUIDString &val)
    {
        parentDomainID = val;
    }
    void SetDomainAlias(const TDomainExtData& cDomainAlias)
    {
        domainAlias = cDomainAlias;
    }
    TDomainExtData& GetDomainAlias()
    {
        return domainAlias;
    }
    const TDomainExtData& GetDomainAlias() const
    {
        return domainAlias;
    }
private:
    UUIDString domainID;            // ����
    UUIDString parentDomainID;      // ������
    string domainName;              // ������
	TDomainExtData domainAlias;		// ����� mod by liangli 2014.3.21 ��domainAlias�ֶ���չ��һ��map�����ݿ���Ķ�

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuserDevicefullinfoAndPrivilegeQueryCondition
{
public:
    CCuserDevicefullinfoAndPrivilegeQueryCondition(){}
    bool GetDevicegroupId(UUIDString &val) const
    {
        if (!optionField.HasOption("devicegroupId"))
            return false;
        val = devicegroupId;
        return true;
    }
    void SetDevicegroupId(const UUIDString &val)
    {
        devicegroupId = val;
        optionField.SetOption("devicegroupId");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
	bool GetDevTreeId(UUIDString &val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeId;
		return true;
	}
	void SetDevTreeId(const UUIDString &val)
	{
		devTreeId = val;
		optionField.SetOption("devTreeId");
	}
private:
    CQryOptionField optionField; // ��ѯ�ֶα�ʶ
    UUIDString devicegroupId; 
    UUIDString domainId;
	UUIDString devTreeId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TUserVideoSourceCapInfo
{
    TUserVideoSourceCapInfo(){}
    string name;
	string extData;
    CVideosourcePrivilege privilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TUserAlarmInputCapInfo
{
    TUserAlarmInputCapInfo(){}
    string name;
    CAlarmInputPrivilege privilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TDevicefullinfoAndPrivilege
{
    TDevicefullinfoAndPrivilege(){}
    CDeviceInfo deviceInfo;
    CDevicePrivilege devicePrivilege;
	vector<int> vidSrcIndex;                //���Ⱥ�˳������ƵԴ����ֵ
    map<int, TUserVideoSourceCapInfo> videosourceinfos; // keyΪvideosource����
    map<int, TUserAlarmInputCapInfo> alarminputinfos;   // keyΪalarminput����
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidSrcIndex(const vector<int>& cVidSrcIndex)
    {
        vidSrcIndex = cVidSrcIndex;
    }
    vector<int>& GetVidSrcIndex()
    {
        return vidSrcIndex;
    }
    const vector<int>& GetVidSrcIndex() const
    {
        return vidSrcIndex;
    }

};

struct TSyncDataUserDeviceInfo
{
    string userName;
    UUIDString deviceID;
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENSyncDataType
{
    enSyncDataDomainInfo = 1,         // ����Ϣ, ��������ΪCDomainRelationInfo���л�����
    enSyncDataDeviceModel = 2,        // �豸�ͺ���Ϣ, ��������ΪCDeviceModelInfo���л�����
    enSyncDataDeviceInfo = 3,         // �豸��Ϣ, ��������ΪCDeviceInfo���л�����
    enSyncDataDeviceCapIndexName = 4, // �豸������������, ��������ΪTDeviceCapName���л�
    enSyncDataDeviceGroup = 5,        // �豸������Ϣ, ��������ΪCDeviceGroupInfo���л�����
    enSyncDataDeviceGroup2Dev = 6,    // �豸���黮��, ��������ΪCDeviceGroupDeviceInfo���л�����
    enSyncDataUserDevice = 7,          // �豸������Ȩ, ��������ΪTSyncDataUserDeviceInfo���л�����
	enSyncDataDevGrpDevSeq = 8,       // �豸���豸����ֵ�� ��������ΪCDeviceGroupDevSeqInfo���л�����
};
enum ENSyncOperateType
{
    enSyncOperateAdd = 1,  // ���ͬ��
    enSyncOperateDel = 2,  // ɾ��ͬ��
    enSyncOperateMod = 3   // �޸�ͬ��
};

// ͬ����������
struct TSyncDataInfo
{
    TSyncDataInfo():sn(0)
    {
    }
    ENSyncDataType dataType;       // ͬ�����ݵ�����
    int sn;                        // ͬ�����ݵ����к�
    ENSyncOperateType operateType; // ��������
    string dataDomainId;           // ͬ������������Ҳ����ͬ�����ݵ�ֱ��������
    CTransparentData dataContent;  // ͬ����������, ��ͬ���������;�������ʱ��ʾ���账��

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// ͬ�����ݽ��
struct TSyncDataResult
{
    TSyncDataResult():sn(0), result(0)
    {
    }
    int sn;
    int result; // ���ܵ�ֵ��CMS_SUCCESS, ERR_TAS_INVALID_SYNCDATA

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// ����ǽ�ĵ�����������󶨹�ϵ
struct TvDecoderBinder
{
    TvDecoderBinder():tvId(0), decoderOutputId(0)
    {
    }
	int tvId;             // ���ӱ��
	UUIDString decoderId; // ���������
    int decoderOutputId;  // ��������Ƶ���ͨ��Id
    
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
// ����ǽ��Ϣ
class CTvwallInfo: public CDeviceInfo
{
public:
    CTvwallInfo():CDeviceInfo(), tvNum(0),tvwallStyle(0)
    {
    }

	void Clear()
	{
		tvNum = 0;
		tvwallStyle = 0;
		tvwallCustomStyleData.clear();
		tvDecoderBinders.clear();
	}
private:
	int tvNum;            // ���ӻ���Ŀ             
	int tvwallStyle;      // ����ǽ���, ��ʹ��ö�٣���ΪTAS�����ľ�������ݣ�����ʱ����������ʱTASҲҪ����             
	string tvwallCustomStyleData; // ����ǽ�Զ���������             

	vector<TvDecoderBinder> tvDecoderBinders; // ������������󶨹�ϵ             

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvNum(int nTvNum)
    {
        tvNum = nTvNum;
    }
    int GetTvNum() const
    {
        return tvNum;
    }

    void SetTvwallStyle(int nTvwallStyle)
    {
        tvwallStyle = nTvwallStyle;
    }
    int GetTvwallStyle() const
    {
        return tvwallStyle;
    }

    void SetTvwallCustomStyleData(const string& strTvwallCustomStyleData)
    {
        tvwallCustomStyleData = strTvwallCustomStyleData;
    }
    string& GetTvwallCustomStyleData()
    {
        return tvwallCustomStyleData;
    }
    const string& GetTvwallCustomStyleData() const
    {
        return tvwallCustomStyleData;
    }

    void SetTvDecoderBinders(const vector<TvDecoderBinder>& cTvDecoderBinders)
    {
        tvDecoderBinders = cTvDecoderBinders;
    }
    vector<TvDecoderBinder>& GetTvDecoderBinders()
    {
        return tvDecoderBinders;
    }
    const vector<TvDecoderBinder>& GetTvDecoderBinders() const
    {
        return tvDecoderBinders;
    }

};

class CTvwallInfoQueryCondition
{
public:
	CTvwallInfoQueryCondition()
	{
	}
    bool GetCuserId(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserId"))
            return false;
        val = cuserId;
        return true;
    }
    void SetCuserId(const UUIDString &val)
    {
        cuserId = val;
        optionField.SetOption("cuserId");
    }
    bool GetCuserGrpId(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserGrpId"))
            return false;
        val = cuserGrpId;
        return true;
    }
    void SetCuserGrpId(const UUIDString &val)
    {
        cuserGrpId = val;
        optionField.SetOption("cuserGrpId");
    }
    bool GetTvwallId(UUIDString &val) const
    {
        if (!optionField.HasOption("tvwallId"))
            return false;
        val = tvwallId;
        return true;
    }
    void SetTvwallId(const UUIDString &val)
    {
        tvwallId = val;
        optionField.SetOption("tvwallId");
    }

private:
	CQryOptionField optionField; // ��ѯ�ֶα�ʶ

    UUIDString cuserId;    // �����û�����������ˣ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û�Ȩ�޷�Χ�ڵģ����������������������ͬʱָ��
    UUIDString cuserGrpId; // �����û������������ˣ�ָ��ʱ�ܹ����Ʒ��ص�����Ϊ���û���Ȩ�޷�Χ�ڵģ����������������������ͬʱָ��
	UUIDString tvwallId;   // ����ǽ���

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENSyncType
{
    enSyncTypeParent     = 1, // ���ϼ�ͬ�����Զ��ݹ����ϼ�ͬ������
    enSyncTypeP2PLimit   = 2, // ��Ե�ͬ����Ȩ����Ȩ�����ڿ�����Ȩ
    enSyncTypeP2PNoLimit = 3, // ��Ե�ͬ����Ȩ�޲��ޣ���������ȫ��ͨ����
};

struct TSyncDstDomainInfo
{
    UUIDString domainId;
    string domainName;
    ENSyncType syncType;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TTransProgress
{
	int startTime;		// ����Ŀ�ʼʱ��
	int endTime;		// ��������ʱ��
	int totalNum;		// ��Ҫ���������
	int finishNum;		// ������ɵ�����

public:
	TTransProgress()
	{
		startTime = 0;
		endTime   = 0;
		totalNum  = 0;
		finishNum = 0;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignInfo
{
public:
	CDeviceAutoAssignInfo()
	{
		ownerType = 0;
	}

	void Clear()
	{
		devGrpId.clear();
		domainId.clear();
		ownerId.clear();
		ownerType = 0;
		operTime.clear();
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

	void SetDomainId(const UUIDString &val)
	{
		domainId = val;
	}
	const UUIDString &GetDomainId() const
	{
		return domainId;
	}
	UUIDString &GetDomainId()
	{
		return domainId;
	}

	void SetOwnerId(const UUIDString &val)
	{
		ownerId = val;
	}
	const UUIDString &GetOwnerId() const
	{
		return ownerId;
	}
	UUIDString &GetOwnerId()
	{
		return ownerId;
	}

	void SetOwnerType(int val)
	{
		ownerType = val;
	}
	int GetOwnerType() const
	{
		return ownerType;
	}
	int &GetOwnerType()
	{
		return ownerType;
	}

	void SetOperTime(const string &val)
	{
		operTime = val;
	}
	const string &GetOperTime() const
	{
		return operTime;
	}
	string &GetOperTime()
	{
		return operTime;
	}

private:
	UUIDString devGrpId;
	UUIDString domainId;
	UUIDString ownerId;
	int        ownerType;
	string     operTime;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDevTreeInfo
{
public:
	CDevTreeInfo()
	{
		Clear();
	}

	void Clear()
	{
		devtreeId.clear();
		devtreeName.clear();
		canBeUsed4Gb = false;
        type=SystemCustomTree;
	}

    enum TreeType
    {
        SystemCustomTree=0, // ϵͳ�Զ����豸��
        UserFavorites=1,    // �û��ղؼ�
        AllTree=255         // ���е���
    };

private:
	UUIDString devtreeId;               
	string     devtreeName;  
	string     devtreeVer;
	bool       canBeUsed4Gb;
    TreeType   type;                
public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

    void SetDevtreeName(const string& strDevtreeName)
    {
        devtreeName = strDevtreeName;
    }
    string& GetDevtreeName()
    {
        return devtreeName;
    }
    const string& GetDevtreeName() const
    {
        return devtreeName;
    }

	void SetDevtreeVer(const string& strDevtreeVer)
	{
		devtreeVer = strDevtreeVer;
	}
	string& GetDevtreeVer()
	{
		return devtreeVer;
	}
	const string& GetDevtreeVer() const
	{
		return devtreeVer;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCanBeUsed4Gb(bool bCanBeUsed4Gb)
    {
        canBeUsed4Gb = bCanBeUsed4Gb;
    }
    bool GetCanBeUsed4Gb() const
    {
        return canBeUsed4Gb;
    }

public:
    void SetType(TreeType eType)
    {
        type = eType;
    }
    TreeType GetType() const
    {
        return type;
    }

};

class CPtzLockInfo
{
public:
	CPtzLockInfo()
	{

	}

	void Clear()
	{
		deviceId.clear();
		videoSrcId=0;
		beginTime.clear();
		endTime.clear();	
		userId.clear();
		password.clear();
	}

private:
	string deviceId;               	// �豸id
	int videoSrcId;               		// ��ƵԴid
	string beginTime;               	// ������ʼʱ��.
	string endTime;               	    // �Զ�����ʱ��.
	string userId;               		// �����û�
	string password;               	// �ֶ���������  
public:
    void SetDeviceId(const string& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    string& GetDeviceId()
    {
        return deviceId;
    }
    const string& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

    void SetBeginTime(const string& strBeginTime)
    {
        beginTime = strBeginTime;
    }
    string& GetBeginTime()
    {
        return beginTime;
    }
    const string& GetBeginTime() const
    {
        return beginTime;
    }

    void SetEndTime(const string& strEndTime)
    {
        endTime = strEndTime;
    }
    string& GetEndTime()
    {
        return endTime;
    }
    const string& GetEndTime() const
    {
        return endTime;
    }

    void SetUserId(const string& strUserId)
    {
        userId = strUserId;
    }
    string& GetUserId()
    {
        return userId;
    }
    const string& GetUserId() const
    {
        return userId;
    }

    void SetPassword(const string& strPassword)
    {
        password = strPassword;
    }
    string& GetPassword()
    {
        return password;
    }
    const string& GetPassword() const
    {
        return password;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPtzLockQueryCondition
{
public:
	CPtzLockQueryCondition(){}

	bool GetDeviceID(UUIDString &val) const
	{
		if (!optionField.HasOption("deviceId"))
			return false;
		val = deviceId;
		return true;
	}
	void SetDeviceID(const UUIDString &val)
	{
		deviceId = val;
		optionField.SetOption("deviceId");
	}

	bool GetVideoSrcId(int &val) const
	{
		if (!optionField.HasOption("videoSrcId"))
			return false;
		val = videoSrcId;
		return true;
	}
	void SetVideoSrcId(const int &val)
	{
		videoSrcId = val;
		optionField.SetOption("videoSrcId");
	}
private:
	CQryOptionField optionField; // ��ѯ�ֶα�ʶ

	string deviceId;       // �豸id
	int videoSrcId;        // ��ƵԴid

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDevGrpVidSrcInfo
{
	UUIDString devId;                                   //�豸ID
	UUIDString devDomainID;                             //�豸������ID, �����ͻ���ʱTASƴװ����
	CDevicePrivilege devPrivilege;               
	TDeviceCapPrivilege devCapPri;                                                         //���豸������ָ���û�ָ���豸���µ���ƵԴindex
	vector<int> vidSrcIndex;                               //���Ⱥ�˳������ƵԴ����ֵ

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetDevDomainID(const UUIDString& strDevDomainID)
    {
        devDomainID = strDevDomainID;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }

    void SetDevPrivilege(const CDevicePrivilege& cDevPrivilege)
    {
        devPrivilege = cDevPrivilege;
    }
    CDevicePrivilege& GetDevPrivilege()
    {
        return devPrivilege;
    }
    const CDevicePrivilege& GetDevPrivilege() const
    {
        return devPrivilege;
    }

public:
    void SetDevCapPri(const TDeviceCapPrivilege& cDevCapPri)
    {
        devCapPri = cDevCapPri;
    }
    TDeviceCapPrivilege& GetDevCapPri()
    {
        return devCapPri;
    }
    const TDeviceCapPrivilege& GetDevCapPri() const
    {
        return devCapPri;
    }

public:
    void SetVidSrcIndex(const vector<int>& cVidSrcIndex)
    {
        vidSrcIndex = cVidSrcIndex;
    }
    vector<int>& GetVidSrcIndex()
    {
        return vidSrcIndex;
    }
    const vector<int>& GetVidSrcIndex() const
    {
        return vidSrcIndex;
    }

};

class CDevGrpVidSrcNum
{
private:
	UUIDString devGrpId;               
	int vidSrcNum;               
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetVidSrcNum(int nVidSrcNum)
    {
        vidSrcNum = nVidSrcNum;
    }
    int GetVidSrcNum() const
    {
        return vidSrcNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevVidsrc
{
private:
	UUIDString devId;                       //�豸ID
	int vidSrcIdx;                          //��ƵԴ����
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetVidSrcIdx(int nVidSrcIdx)
    {
        vidSrcIdx = nVidSrcIdx;
    }
    int GetVidSrcIdx() const
    {
        return vidSrcIdx;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevVidSrcInfo
{
private:
	UUIDString devId;                //�豸ID
	bool exist;                      //Ӧ���жϴ��ֶΣ�Ϊtrueʱ�������ֶβ���Ч��false��ʾ��ƵԴ������
	int vidsrcIdx;                   //��ƵԴ����             
	UUIDString devGrpId;             //��ƵԴ��������ID
	string devGrpName;                              //��ƵԴ����������
	UUIDString devGrpdomainId;       //�豸��������ID
	string devName;                  //�豸��
	string vidAlias;                 //��ƵԴ����
	string ipAddress;                
	TDeviceCapExtData extData;       //��ƵԴ��γ����Ϣ
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetExist(bool bExist)
    {
        exist = bExist;
    }
    bool GetExist() const
    {
        return exist;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetVidAlias(const string& strVidAlias)
    {
        vidAlias = strVidAlias;
    }
    string& GetVidAlias()
    {
        return vidAlias;
    }
    const string& GetVidAlias() const
    {
        return vidAlias;
    }

    void SetIpAddress(const string& strIpAddress)
    {
        ipAddress = strIpAddress;
    }
    string& GetIpAddress()
    {
        return ipAddress;
    }
    const string& GetIpAddress() const
    {
        return ipAddress;
    }

    void SetExtData(const TDeviceCapExtData& cExtData)
    {
        extData = cExtData;
    }
    TDeviceCapExtData& GetExtData()
    {
        return extData;
    }
    const TDeviceCapExtData& GetExtData() const
    {
        return extData;
    }

public:
	void Clear()
	{
		devId.clear();
		exist = false;
		vidsrcIdx = -1;
		devGrpId.clear();
		devGrpdomainId.clear();
		devName.clear();
		vidAlias.clear();
		ipAddress.clear();
		extData.Clear();
	}
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidsrcIdx(int nVidsrcIdx)
    {
        vidsrcIdx = nVidsrcIdx;
    }
    int GetVidsrcIdx() const
    {
        return vidsrcIdx;
	}
public:
    void SetDevGrpdomainId(const UUIDString& strDevGrpdomainId)
    {
        devGrpdomainId = strDevGrpdomainId;
    }
    UUIDString& GetDevGrpdomainId()
    {
        return devGrpdomainId;
    }
    const UUIDString& GetDevGrpdomainId() const
    {
        return devGrpdomainId;
    }

public:
    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

public:
    void SetDevGrpName(const string& strDevGrpName)
    {
        devGrpName = strDevGrpName;
    }
    string& GetDevGrpName()
    {
        return devGrpName;
    }
    const string& GetDevGrpName() const
    {
        return devGrpName;
    }

};

enum TDevGbidErrType
{
	GBID_ERRTYPE_NOERROR = 0,
	GBID_ERRTYPE_PRIREPEAT = 1, //����gbid�ظ�
	GBID_ERRTYPE_SECREPEAT = 2, //����gbid�ظ�
};

struct TDevGbidErrItem
{
	TDevGbidErrType gbidErrType; //��������
	int  errIndex;//���δ����豸�����

	//�豸��Ϣ
	UUIDString domainId;
	UUIDString devId;
	string     devName;
	string     vidSrcName;
	int        vidSrcId;                   //��ƵԴId
	string     primaryOldGbid;
	string     primaryNewGbid;
	string     secondaryOldGbid;
	string     secondaryNewGbid;

	//������Ϣ
	UUIDString devGrpId;
	int        vidSrcGrpIdx;               //��ƵԴ�ڷ����ڵ����

	vector<CDeviceGroupInfo> devGrpFullGbinfos; //�豸---->������·����ÿ���������ϸ��Ϣ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENOrderType
{
	ENORDER_TYPE_INNER_DEVGRP_MOVE=1,
	ENORDER_TYPE_INNER_DEVGRP_TOP=2,
	ENORDER_TYPE_INNER_DEVGRP_BOTTOM=3,
	ENORDER_TYPE_INNER_DEVICE_MOVE=4,
	ENORDER_TYPE_INNER_DEVICE_TOP=5,
	ENORDER_TYPE_INNER_DEVICE_BOTTOM=6,
};

struct TVidSrcOrderInfo
{
	TVidSrcOrderInfo()
	{
		vidsrcIndexMoved = -2;
		vidsrcIndexRefer = -2;
	}
private:
	UUIDString devGrpId;               
	UUIDString devMovedId;                //���ƶ����豸Id
	int        vidsrcIndexMoved;                //���ƶ����豸��ƵԴID��-1��ʶ�÷����ڸ��豸������ƵԴ���ƶ�
	UUIDString devReferId;                //�ƶ������豸����ʶ�����ƶ��豸���ƶ������豸����ƵԴ��֮��
	int        vidsrcIndexRefer;          //�ƶ������豸��ƵԴID��-1��ʾ�ƶ������豸��ĩ��ƵԴ֮��
	ENOrderType orderType;                         //�����������
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevMovedId(const UUIDString& strDevMovedId)
    {
        devMovedId = strDevMovedId;
    }
    UUIDString& GetDevMovedId()
    {
        return devMovedId;
    }
    const UUIDString& GetDevMovedId() const
    {
        return devMovedId;
    }

    void SetVidsrcIndexMoved(int nVidsrcIndexMoved)
    {
        vidsrcIndexMoved = nVidsrcIndexMoved;
    }
    int GetVidsrcIndexMoved() const
    {
        return vidsrcIndexMoved;
    }

    void SetDevReferId(const UUIDString& strDevReferId)
    {
        devReferId = strDevReferId;
    }
    UUIDString& GetDevReferId()
    {
        return devReferId;
    }
    const UUIDString& GetDevReferId() const
    {
        return devReferId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOrderType(ENOrderType eOrderType)
    {
        orderType = eOrderType;
    }
    ENOrderType GetOrderType() const
    {
        return orderType;
    }

public:
    void SetVidsrcIndexRefer(int nVidsrcIndexRefer)
    {
        vidsrcIndexRefer = nVidsrcIndexRefer;
    }
    int GetVidsrcIndexRefer() const
    {
        return vidsrcIndexRefer;
    }

};

#endif // _TAS_STRUCT_H_























































