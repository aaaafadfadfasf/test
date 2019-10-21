#ifndef _GBS_STRUCT_H_
#define _GBS_STRUCT_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include "cms/gb/gbspecial_struct.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
using std::map;

class TiXmlDocument;
class TiXmlNode;

typedef string TStreamType;
#define GBS_STREAM_TYPE_UDP "UDP"
#define GBS_STREAM_TYPE_TCP "TCP"

struct TGbsCivilCode
{
    string civilCode;  // �����������
    string civilName;  // ������������

    bool operator == (const TGbsCivilCode &rh) const
    {
        return civilCode == rh.civilCode && civilName == rh.civilName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsConf
{
    TGbsConf()
    {
        useOneGbDomainId = true;
        gb_port = 0;
        g800_port = 0;
    }

    string  gbDomainId;      // �ƴﱾ���Ӧ������������Ʒ��������� 20λ
    vector<TGbsCivilCode> civilCode; // ���������б�
    string  gbParentId;      // gbDomainId �ĸ�����
    // trueʱ���пƴ���ʹ��ͬһ���������ţ�falseʱ�����ӿƴ�����Ҫ�������
    bool useOneGbDomainId;   // �ƴ��¼���ȫ��ʹ��һ���������־

    string realm;            // GBS��������ķ�Χ��ʶ��Ĭ��ΪgbDomainId��ǰ8λ
    string uniformPassword;  // ͳһ�ĵ�¼����, Ϊ�ձ�ʾ��У��

    int gb_port;     // gbs�����Ĺ���˿� ��֧���޸�
    string g800_ip;  // g800 ip
    int g800_port;   // g800 �˿�

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsAdvanceConf
{
    TGbsAdvanceConf()
    {
        
    }
    enum EnRecordSource
    {
        enRecordSourcePU, // ǰ��¼��
        enRecordSourcePlatform, // ƽ̨¼��
		enRecordSourceAll, //ȫ��¼��
    };

    EnRecordSource recordSourceConf; // ¼����Դ���ã�����GBS�����ṩ¼����Դ

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsOptionConf
{
	TGbsOptionConf()
	{
		syncGbpuLongLati=false;
	}

	bool syncGbpuLongLati;  // ͬ������pu�ľ�γ��������ƽ̨
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsKdDomainConf
{
    string kdDomainId;       // �ƴ�����
    string gbDomainId;       // ��Ӧ�Ĺ������� ����������Ʒ��������� 20λ
    string gbParentDomainId; // ��Ӧ�Ĺ�����ĸ����� 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsGbPuConf
{
    TGbsGbPuConf()
    {
        videoSourceNum = 0;
        encodeChannelNum = 0;
        licenseNum = 0;
        useFlag = true;
        localStorageCap = false;
		streamType = GBS_STREAM_TYPE_UDP;
		parallelAlarmInputNum = 0;
    }

    string devName;          // �豸����
    string devModel;         // �豸�ͺ�
    string manufacturer;     // �豸����
    string userName;         // Ϊ��ʱʹ��PU���
    string userPassword;     // Ϊ��ʱʹ��ͳһ�ĵ�¼����
    string ip;               // �豸IP��ַ
    bool useFlag;            // ����ͣ��
	
    int  videoSourceNum;     // ��ƵԴ��Ŀ
    int  encodeChannelNum;   // ����ͨ����Ŀ
    int  licenseNum;         // license��ȡ��Ŀ
    bool localStorageCap;    // ���ش洢����
    // �豸������ǩ ֧�����±�ǩ
    // DEVICE_CAP_LABEL_ENCODE_NVR DEVICE_CAP_LABEL_ENCODE_IPC_BULLET DEVICE_CAP_LABEL_ENCODE_IPC_DOME
    vector<int> capLabel;    

    string devAddr;          // �豸��ַ                
    string devDesc;          // �豸����                

    string gbDevId;          // ����gbsʱ������豸������
    vector<string> gbChnIds; // ����gbsʱ�����ͨ���������б�
    string kdDevId;          // ����tasʱ����Ŀƴ��豸��
    string kdmDevId;         // ����tasʱָ����kdmno                 
    TStreamType streamType;                //�������ͣ�UDP--Ĭ�ϣ�TCP
    string gbNatId;                            //����ǰ��NAT��Ϣ
    int parallelAlarmInputNum;			   //���ڸ澯��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }

    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }

public:
    void SetKdmDevId(const string& strKdmDevId)
    {
        kdmDevId = strKdmDevId;
    }
    string& GetKdmDevId()
    {
        return kdmDevId;
    }
    const string& GetKdmDevId() const
    {
        return kdmDevId;
    }

public:
    void SetStreamType(const TStreamType& cStreamType)
    {
        streamType = cStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }
};
struct TGbsGbDecodeConf
{
	string devName;                                        // �豸����
	string devModelName;                                       // �豸�ͺ�
	string manufacturer;                                   // �豸����
	int    decodeChnNum;                                                  // ����ͨ����
	int    decodeOutputChnNum;                                            // �������ͨ����
	string userName;                                      // Ϊ��ʱʹ��PU���
	string userPassword;                                  // Ϊ��ʱʹ��ͳһ�ĵ�¼����
	string ip;                                             // �豸IP��ַ
	bool useFlag;                                          // ����ͣ��
	string devAddr;                                        // �豸��ַ                
	string devDesc;                                        // �豸����                
	string kdDevId;                                                       // �����Ƿ�����豸�ƴ���
	string gbDevId;                                        // ����gbsʱ������豸������
	vector<string> gbChnIds;                               // ����gbsʱ�����ͨ���������б�
	string gbNatId;                                                          //����ǰ��NAT��Ϣ
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
    void SetDevModelName(const string& strDevModelName)
    {
        devModelName = strDevModelName;
    }
    string& GetDevModelName()
    {
        return devModelName;
    }
    const string& GetDevModelName() const
    {
        return devModelName;
    }
    void SetManufacturer(const string& strManufacturer)
    {
        manufacturer = strManufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetDecodeChnNum(int nDecodeChnNum)
    {
        decodeChnNum = nDecodeChnNum;
    }
    int GetDecodeChnNum() const
    {
        return decodeChnNum;
    }
    void SetDecodeOutputChnNum(int nDecodeOutputChnNum)
    {
        decodeOutputChnNum = nDecodeOutputChnNum;
    }
    int GetDecodeOutputChnNum() const
    {
        return decodeOutputChnNum;
    }
    void SetUserName(const string& strUserName)
    {
        userName = strUserName;
    }
    string& GetUserName()
    {
        return userName;
    }
    const string& GetUserName() const
    {
        return userName;
    }
    void SetUserPassword(const string& strUserPassword)
    {
        userPassword = strUserPassword;
    }
    string& GetUserPassword()
    {
        return userPassword;
    }
    const string& GetUserPassword() const
    {
        return userPassword;
    }
    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
    string& GetIp()
    {
        return ip;
    }
    const string& GetIp() const
    {
        return ip;
    }
    void SetUseFlag(bool bUseFlag)
    {
        useFlag = bUseFlag;
    }
    bool GetUseFlag() const
    {
        return useFlag;
    }
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }
    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }
    void SetGbChnIds(const vector<string>& cGbChnIds)
    {
        gbChnIds = cGbChnIds;
    }
    vector<string>& GetGbChnIds()
    {
        return gbChnIds;
    }
    const vector<string>& GetGbChnIds() const
    {
        return gbChnIds;
    }
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    string& GetGbNatId()
    {
        return gbNatId;
    }
    const string& GetGbNatId() const
    {
        return gbNatId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TGbsGbPeripheralType;
#define GB_GBPERIPHERAL_TYPE_OTHER          0
#define GB_GBPERIPHERAL_TYPE_INNER_MSS      1

struct TGbsGbPeripheralConf
{
	TGbsGbPeripheralConf()
	{
		onlineStatus = "offline";
		type = GB_GBPERIPHERAL_TYPE_OTHER;
	}

	string name;       // ��������
	string gbDevId;    // ���������
	string userName;   // һ��Ϊ��Ӧ��������
	string password;   // Ϊ��ʱʹ��ͳһ�ĵ�¼����
	string ip;         // �豸IP
	string onlineStatus; // ����״̬��online, offline
	TGbsGbPeripheralType type;	// �������ͣ�Ŀǰ��Ҫ�У�����mss������


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//����Ŀ¼���ͷ�ʽ
enum EnumCatalogType
{
	GBS_CATALOG_TYPE_CIVILCODE,   //civilcode��ʽ����
	GBS_CATALOG_TYPE_BUSINNESS,   //216��ʽ����
};

struct TGbsGbDomainChildConf
{
    TGbsGbDomainChildConf()
    {
        onlineStatus = "offline";
		catalogtype = GBS_CATALOG_TYPE_CIVILCODE;
		streamType = GBS_STREAM_TYPE_UDP;
		qryCatalogOnReg = "TRUE";
        interConnect = false;
		reportCatalog2OtherUas = false;
    }
	
    string name;       // �¼�������
    string gbDevId;    // ������
    string userName;   // һ��Ϊ��Ӧ���¼����
    string password;   // Ϊ��ʱʹ��ͳһ�ĵ�¼����
    string ip;         // �豸IP
    string onlineStatus; // ����״̬��online, offline
	int   catalogtype;   //Ŀ¼���ͷ�ʽ
    string gbNatId;     //��Է�������NAT��ϵ
	TStreamType streamType;                //�������ͣ���ѡ��UDP��TCP
	string qryCatalogOnReg;                //ע��ɹ����Ƿ��Զ���ѯĿ¼��"TRUE"--�Զ���ѯ��"FALSE"--���Զ���ѯ
    bool interConnect;                               //true-�齨����ƽ̨
	bool reportCatalog2OtherUas;                     //true-�ɽ���uasĿ¼�ϱ�������uas��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetQryCatalogOnReg(const string& strQryCatalogOnReg)
    {
        qryCatalogOnReg = strQryCatalogOnReg;
    }
    string& GetQryCatalogOnReg()
    {
        return qryCatalogOnReg;
    }
    const string& GetQryCatalogOnReg() const
    {
        return qryCatalogOnReg;
    }
    
    void SetInterConnect(bool bInterConnect)
    {
        interConnect = bInterConnect;
    }
    bool GetInterConnect() const
    {
        return interConnect;
    }

    void SetReportCatalog2OtherUas(bool bReportCatalog2OtherUas)
    {
        reportCatalog2OtherUas = bReportCatalog2OtherUas;
    }
    bool GetReportCatalog2OtherUas() const
    {
        return reportCatalog2OtherUas;
    }

};

struct TGbsGbDomainParentConf
{
    TGbsGbDomainParentConf()
    {
        serverPort = 0;
        loginStatus = "unlogin";
		catalogtype = GBS_CATALOG_TYPE_CIVILCODE;
		streamType = GBS_STREAM_TYPE_UDP;
		fixGbsIp = "0.0.0.0";
		curGbsIp = "0.0.0.0";
		interConnect = false;
		reportCatalog2OtherUas  = false;
    }

    string name;      // �ϼ�������
    string gbDevId;   // ������
    string userName;  // һ��Ϊ������
    string password;  // ��¼����
    string serverIp;  // ƽ̨IP
    int    serverPort;// ƽ̨�˿�
    string loginStatus; // ��¼״̬��logined, unlogin
	int    catalogtype; //����Ŀ¼���ͷ�ʽ
    string gbNatId;     //��Է�������NAT��ϵ
	TStreamType streamType;                 //�������ͣ����ϼ����ԣ���������������Ч
	string fixGbsIp;                   //�û�ָ����GBSIP��"0.0.0.0"��ʾ֧�ֶ�̬���⣬�����ʾ��ָ��GBS�����������
	string curGbsIp;                   //��ǰ����ʹ�õ�GBSIP
	bool interConnect;                               //true-�齨����ƽ̨
	bool reportCatalog2OtherUas;                     //true-�ɽ���uasĿ¼�ϱ�������uas��
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetFixGbsIp(const string& strFixGbsIp)
    {
        fixGbsIp = strFixGbsIp;
    }
    string& GetFixGbsIp()
    {
        return fixGbsIp;
    }
    const string& GetFixGbsIp() const
    {
        return fixGbsIp;
    }

    void SetCurGbsIp(const string& strCurGbsIp)
    {
        curGbsIp = strCurGbsIp;
    }
    string& GetCurGbsIp()
    {
        return curGbsIp;
    }
    const string& GetCurGbsIp() const
    {
        return curGbsIp;
    }

public:
    void SetInterConnect(bool bInterConnect)
    {
        interConnect = bInterConnect;
    }
    bool GetInterConnect() const
    {
        return interConnect;
    }

    void SetReportCatalog2OtherUas(bool bReportCatalog2OtherUas)
    {
        reportCatalog2OtherUas = bReportCatalog2OtherUas;
    }
    bool GetReportCatalog2OtherUas() const
    {
        return reportCatalog2OtherUas;
    }

};

struct TGbsGbdevInfo
{
    TGbsGbdevInfo()
    {
        kdChannelIndex = -1;
        parental = 0;
        ipPort = 0;
        devStatus = "OFF";
    }

    string gbid;   // �豸������
    string kdid;   // �豸�ƴ���
    string kdDomainId;     // �豸�ƴ����
    int    kdChannelIndex; // �豸����ʱΪ-1
    string gbua;   // �豸ͨ���ĸ������豸���ߵ�
    string name;   // �豸����
    string manufacturer; // �豸����
    string model;        // �豸�ͺ�
    string address;      // �豸��ַ
    int    parental;     // �Ƿ�ӵ�����豸
    string parentID;     // ���豸���
    string ipAddress;    // �豸IP
    int    ipPort;       // �豸�˿�
    string devStatus;    // �豸����״̬ ON��OFF

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsBusinessGroup
{
    string id;   // ҵ�������
    string name; // ҵ���������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsVirtualOrganization
{
    string id;              // ������֯���
    string name;            // ������֯����
    string parentId;        // ��Ŀ¼��ţ�����ΪϵͳĿ¼ID��������֯���
    string businessGroupId; // ������ҵ�������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsOrganization2Dev
{
    string id;      // ������֯���
    string devId;   // �豸���
    string devName; // �豸���� ��������������ʱ��Ч
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsGbParentDomainInfo
{
	string gbid;                //�����ID
	string name;                //����
	string devTreeId;           //�豸��ID
	string devTreeVer;         //�豸���汾

public:
    void SetGbid(const string& strGbid)
    {
        gbid = strGbid;
    }
    string& GetGbid()
    {
        return gbid;
    }
    const string& GetGbid() const
    {
        return gbid;
    }

    void SetName(const string& strName)
    {
        name = strName;
    }
    string& GetName()
    {
        return name;
    }
    const string& GetName() const
    {
        return name;
    }

	void SetDevTreeId(const string& strDevTreeId)
	{
		devTreeId = strDevTreeId;
	}
	string& GetDevTreeId()
	{
		return devTreeId;
	}
	const string& GetDevTreeId() const
	{
		return devTreeId;
	}

	void SetDevTreeVer(const string& strDevTreeVer)
	{
		devTreeVer = strDevTreeVer;
	}
	string& GetDevTreeVer()
	{
		return devTreeVer;
	}
	const string& GetDevTreeVer() const
	{
		return devTreeVer;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

struct TGbDevGrpGbidBatchGetInfo
{
	string groupkdid;           //����kdid
	string gbid;               //����gbid
public:
    void SetGroupkdid(const string& strGroupkdid)
    {
        groupkdid = strGroupkdid;
    }
    string& GetGroupkdid()
    {
        return groupkdid;
    }
    const string& GetGroupkdid() const
    {
        return groupkdid;
    }

    void SetGbid(const string& strGbid)
    {
        gbid = strGbid;
    }
    string& GetGbid()
    {
        return gbid;
    }
    const string& GetGbid() const
    {
        return gbid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbDevGbidBatchGetCondition
{
	string  devGrpId;             //����id(��ѡ)
	string  kdid;               
	int     vidnum;               //��ƵԴ����
	vector<int> vidindexs;        //��ƵԴindex
	vector<int> prisec;           //0����������1��������2����ȫ�� ��vidindexsһһ��Ӧ
public:
    void SetKdid(const string& strKdid)
    {
        kdid = strKdid;
    }
    string& GetKdid()
    {
        return kdid;
    }
    const string& GetKdid() const
    {
        return kdid;
    }

    void SetVidnum(int nVidnum)
    {
        vidnum = nVidnum;
    }
    int GetVidnum() const
    {
        return vidnum;
    }

	void SetVidindexs(const vector<int>& cVidindexs)
	{
		vidindexs = cVidindexs;
	}
	vector<int>& GetVidindexs()
	{
		return vidindexs;
	}
	const vector<int>& GetVidindexs() const
	{
		return vidindexs;
	}

	void SetPrisec(const vector<int>& cPrisec)
	{
		prisec = cPrisec;
	}
	vector<int>& GetPrisec()
	{
		return prisec;
	}
	const vector<int>& GetPrisec() const
	{
		return prisec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevGrpId(const string& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    string& GetDevGrpId()
    {
        return devGrpId;
    }
    const string& GetDevGrpId() const
    {
        return devGrpId;
    }

};
struct TGbDevGbidBatchGetInfo
{
	string  devGrpId;             //����ID
	string  kdid;               
	int     vidindex;             //��ƵԴindex
	int     prisec;               //��������0-����  1-���� 2-ȫ��
	string  primary;               //����
	string  second;               //����
public:
    void SetKdid(const string& strKdid)
    {
        kdid = strKdid;
    }
    string& GetKdid()
    {
        return kdid;
    }
    const string& GetKdid() const
    {
        return kdid;
    }

    void SetVidindex(int nVidindex)
    {
        vidindex = nVidindex;
    }
    int GetVidindex() const
    {
        return vidindex;
    }
	void SetPrimary(const string& strPrimary)
	{
		primary = strPrimary;
	}
	string& GetPrimary()
	{
		return primary;
	}
	const string& GetPrimary() const
	{
		return primary;
	}

	void SetSecond(const string& strSecond)
	{
		second = strSecond;
	}
	string& GetSecond()
	{
		return second;
	}
	const string& GetSecond() const
	{
		return second;
	}
	void SetPrisec(int nPrisec)
	{
		prisec = nPrisec;
	}
	int GetPrisec() const
	{
		return prisec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevGrpId(const string& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    string& GetDevGrpId()
    {
        return devGrpId;
    }
    const string& GetDevGrpId() const
    {
        return devGrpId;
    }

};

struct TGbsNatData
{
    enum ProtoType
    {
        udp = 1,
        tcp = 2
    };
    TGbsNatData()
    {
        proto = udp;
        intraNetPortBegin = 0;
        intraNetPortEnd = 0;
        extraNetPortBegin = 0;
        extraNetPortEnd = 0;
    }

    ProtoType proto;
    string intraNetIp;
    u16 intraNetPortBegin;
    u16 intraNetPortEnd;
    string extraNetIp;
    u16 extraNetPortBegin;
    u16 extraNetPortEnd;
    string desc;

public:
    void SetProto(u16 wProto)
    {
        proto = (ProtoType)wProto;
    }
    u16 GetProto()
    {
        return (u32)proto;
    }

    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }
    const string& GetDesc() const
    {
        return desc;
    }

    void SetIntraNetIp(const string &strIntraNetIp)
    {
        intraNetIp = strIntraNetIp;
    }
    const string& GetIntraNetIp() const
    {
        return intraNetIp;
    }
    string& GetIntraNetIp()
    {
        return intraNetIp;
    }

    void SetIntraNetPortBegin(u16 wIntraNetPortBegin)
    {
        intraNetPortBegin = wIntraNetPortBegin;
    }
    int GetIntraNetPortBegin()
    {
        return intraNetPortBegin;
    }

    void SetIntraNetPortEnd(u16 wIntraNetPortEnd)
    {
        intraNetPortEnd= wIntraNetPortEnd;
    }
    int GetIntraNetPortEnd()
    {
        return intraNetPortEnd;
    }

    void SetExtraNetIp(const string &strExtraNetIp)
    {
        extraNetIp = strExtraNetIp;
    }
    const string& GetExtraNetIp() const
    {
        return extraNetIp;
    }
    string& GetExtraNetIp()
    {
        return extraNetIp;
    }


    void SetExtraNetPortBegin(u16 wExtraNetPortBegin)
    {
        extraNetPortBegin = wExtraNetPortBegin;
    }
    int GetExtraNetPortBegin()
    {
        return extraNetPortBegin;
    }

    void SetExtraNetPortEnd(u16 wExtraNetPortEnd)
    {
        extraNetPortEnd = wExtraNetPortEnd;
    }
    int GetExtraNetPortEnd()
    {
        return extraNetPortEnd;
    }

public:
    const std::string ToXml() const;
    void FromXml(const std::string &xml_string);
    void ToXml(TiXmlDocument &xmlDoc) const;
    void ToXmlWithoutClassNameNode(TiXmlNode &xmlNode) const;
    void FromXml(const TiXmlNode &xmlNode);
};

struct TGbsRegUasInfo
{
	string gbsIp;                           //GBSIP
	int    upCapacity;                      //��������
	bool   primarGbs;                                      //�Ƿ���GBS��true-�ǣ�false--��
	map<string, string> uasInfo;            //key:�����ϼ���ID,value:�����ϼ������

public:
    void SetGbsIp(const string& strGbsIp)
    {
        gbsIp = strGbsIp;
    }
    string& GetGbsIp()
    {
        return gbsIp;
    }
    const string& GetGbsIp() const
    {
        return gbsIp;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUpCapacity(int nUpCapacity)
    {
        upCapacity = nUpCapacity;
    }
    int GetUpCapacity() const
    {
        return upCapacity;
    }

public:
    void SetUasInfo(const map<string, string>& cUasInfo)
    {
        uasInfo = cUasInfo;
    }
    map<string, string>& GetUasInfo()
    {
        return uasInfo;
    }
    const map<string, string>& GetUasInfo() const
    {
        return uasInfo;
    }

public:
    void SetPrimarGbs(bool bPrimarGbs)
    {
        primarGbs = bPrimarGbs;
    }
    bool GetPrimarGbs() const
    {
        return primarGbs;
    }

};

struct TGbsInfo 
{
	int upCapacity;                               //��������
	string gbsIp;                                 //gbsIp
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUpCapacity(int nUpCapacity)
    {
        upCapacity = nUpCapacity;
    }
    int GetUpCapacity() const
    {
        return upCapacity;
    }

public:
    void SetGbsIp(const string& strGbsIp)
    {
        gbsIp = strGbsIp;
    }
    string& GetGbsIp()
    {
        return gbsIp;
    }
    const string& GetGbsIp() const
    {
        return gbsIp;
    }
};

struct TGbsDevInfo
{
	TGbsDevInfo()
	{
		gbLabel = 1;
		devStatus = "OFF";
		online = "OFFLINE";
		channel = 0;
		runStatus = "OK";

		certEndTime = "2037-01-01T01:00:00";
		longitude = INVALID_LONGITUDE;
		latitude = INVALID_LATITUDE;

		kdChannelIndex = -1;
		parental = 0;
		safetyWay = 0;
		registerWay = 0;
		certifiable = 0;
		certErrCode = 0;
		secrecy = 0;
		ipPort = 0;
		channel = 0;
	}

	string gbid;               
	string kdid;               
	string kdDomainId;               
	s32    kdChannelIndex;                // �豸����ʱΪ-1, �豸����ʱΪ-2
	s32 gbLabel;                   // �豸�Ĺ����ǩ, ���������豸�Ƿ�Ϊ�����豸
	string gbua;               
	string name;               
	string manufacturer;               
	string model;               
	string owner;               
	string civilCode;               
	string block;               
	string address;               
	s32    parental;               
private:
	string parentID;               
public:
	s32    safetyWay;               
	s32    registerWay;               
	string certNum;               
	s32    certifiable;               
	s32    certErrCode;               
	string certEndTime;               
	s32    secrecy;               
	string ipAddress;               
	s32    ipPort;               
	string password;               
	string devStatus;                                             
	double longitude;               
	double latitude;               
	string firmware;               
	s32    channel;               
	string online;                              
	string runStatus;               
	string abnormalReason;               
	string businessGroupID;               
	TGbDeviceCatalogInfo info;               
public:
	void SetGbid(const string& strGbid)
	{
		gbid = strGbid;
	}
	string& GetGbid()
	{
		return gbid;
	}
	const string& GetGbid() const
	{
		return gbid;
	}

	void SetKdid(const string& strKdid)
	{
		kdid = strKdid;
	}
	string& GetKdid()
	{
		return kdid;
	}
	const string& GetKdid() const
	{
		return kdid;
	}

	void SetKdDomainId(const string& strKdDomainId)
	{
		kdDomainId = strKdDomainId;
	}
	string& GetKdDomainId()
	{
		return kdDomainId;
	}
	const string& GetKdDomainId() const
	{
		return kdDomainId;
	}

	void SetKdChannelIndex(s32 nKdChannelIndex)
	{
		kdChannelIndex = nKdChannelIndex;
	}
	s32 GetKdChannelIndex() const
	{
		return kdChannelIndex;
	}

	void SetGbLabel(s32 cGbLabel)
	{
		gbLabel = cGbLabel;
	}
	s32 GetGbLabel()
	{
		return gbLabel;
	}
	const s32 GetGbLabel() const
	{
		return gbLabel;
	}

	void SetGbua(const string& strGbua)
	{
		gbua = strGbua;
	}
	string& GetGbua()
	{
		return gbua;
	}
	const string& GetGbua() const
	{
		return gbua;
	}

	void SetName(const string& strName)
	{
		name = strName;
	}
	string& GetName()
	{
		return name;
	}
	const string& GetName() const
	{
		return name;
	}

	void SetManufacturer(const string& strManufacturer)
	{
		manufacturer = strManufacturer;
	}
	string& GetManufacturer()
	{
		return manufacturer;
	}
	const string& GetManufacturer() const
	{
		return manufacturer;
	}

	void SetModel(const string& strModel)
	{
		model = strModel;
	}
	string& GetModel()
	{
		return model;
	}
	const string& GetModel() const
	{
		return model;
	}

	void SetOwner(const string& strOwner)
	{
		owner = strOwner;
	}
	string& GetOwner()
	{
		return owner;
	}
	const string& GetOwner() const
	{
		return owner;
	}

	void SetCivilCode(const string& strCivilCode)
	{
		civilCode = strCivilCode;
	}
	string& GetCivilCode()
	{
		return civilCode;
	}
	const string& GetCivilCode() const
	{
		return civilCode;
	}

	void SetBlock(const string& strBlock)
	{
		block = strBlock;
	}
	string& GetBlock()
	{
		return block;
	}
	const string& GetBlock() const
	{
		return block;
	}

	void SetAddress(const string& strAddress)
	{
		address = strAddress;
	}
	string& GetAddress()
	{
		return address;
	}
	const string& GetAddress() const
	{
		return address;
	}

	void SetParental(s32 nParental)
	{
		parental = nParental;
	}
	s32 GetParental() const
	{
		return parental;
	}

	void SetParentID(const string& strParentID)
	{
		parentID = strParentID;
	}
	string& GetParentID()
	{
		return parentID;
	}
	const string& GetParentID() const
	{
		return parentID;
	}

	void SetSafetyWay(s32 nSafetyWay)
	{
		safetyWay = nSafetyWay;
	}
	s32 GetSafetyWay() const
	{
		return safetyWay;
	}

	void SetRegisterWay(s32 nRegisterWay)
	{
		registerWay = nRegisterWay;
	}
	s32 GetRegisterWay() const
	{
		return registerWay;
	}

	void SetCertNum(const string& strCertNum)
	{
		certNum = strCertNum;
	}
	string& GetCertNum()
	{
		return certNum;
	}
	const string& GetCertNum() const
	{
		return certNum;
	}

	void SetCertifiable(s32 nCertifiable)
	{
		certifiable = nCertifiable;
	}
	s32 GetCertifiable() const
	{
		return certifiable;
	}

	void SetCertErrCode(s32 nCertErrCode)
	{
		certErrCode = nCertErrCode;
	}
	s32 GetCertErrCode() const
	{
		return certErrCode;
	}

	void SetCertEndTime(const string& strCertEndTime)
	{
		certEndTime = strCertEndTime;
	}
	string& GetCertEndTime()
	{
		return certEndTime;
	}
	const string& GetCertEndTime() const
	{
		return certEndTime;
	}

	void SetSecrecy(s32 nSecrecy)
	{
		secrecy = nSecrecy;
	}
	s32 GetSecrecy() const
	{
		return secrecy;
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

	void SetIpPort(s32 nIpPort)
	{
		ipPort = nIpPort;
	}
	s32 GetIpPort() const
	{
		return ipPort;
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

	void SetLongitude(double nLongitude)
	{
		longitude = nLongitude;
	}
	double GetLongitude() const
	{
		return longitude;
	}

	void SetLatitude(double nLatitude)
	{
		latitude = nLatitude;
	}
	double GetLatitude() const
	{
		return latitude;
	}

	void SetFirmware(const string& strFirmware)
	{
		firmware = strFirmware;
	}
	string& GetFirmware()
	{
		return firmware;
	}
	const string& GetFirmware() const
	{
		return firmware;
	}

	void SetChannel(s32 nChannel)
	{
		channel = nChannel;
	}
	s32 GetChannel() const
	{
		return channel;
	}

	void SetOnline(const string& cOnline)
	{
		online = cOnline;
	}
	string& GetOnline()
	{
		return online;
	}
	const string& GetOnline() const
	{
		return online;
	}

	void SetRunStatus(const string& strRunStatus)
	{
		runStatus = strRunStatus;
	}
	string& GetRunStatus()
	{
		return runStatus;
	}
	const string& GetRunStatus() const
	{
		return runStatus;
	}

	void SetAbnormalReason(const string& strAbnormalReason)
	{
		abnormalReason = strAbnormalReason;
	}
	string& GetAbnormalReason()
	{
		return abnormalReason;
	}
	const string& GetAbnormalReason() const
	{
		return abnormalReason;
	}

	void SetBusinessGroupID(const string& strBusinessGroupID)
	{
		businessGroupID = strBusinessGroupID;
	}
	string& GetBusinessGroupID()
	{
		return businessGroupID;
	}
	const string& GetBusinessGroupID() const
	{
		return businessGroupID;
	}

	void SetInfo(const TGbDeviceCatalogInfo& cInfo)
	{
		info = cInfo;
	}
	TGbDeviceCatalogInfo& GetInfo()
	{
		return info;
	}
	const TGbDeviceCatalogInfo& GetInfo() const
	{
		return info;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevStatus(const string& strDevStatus)
    {
        devStatus = strDevStatus;
    }
    string& GetDevStatus()
    {
        return devStatus;
    }
    const string& GetDevStatus() const
    {
        return devStatus;
    }

};

#endif // _GBS_STRUCT_H_
































