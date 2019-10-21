/*
这里保存了国标协议中的协议类用到的特殊struct
该类struct不适合cpp自动生成，需要人工定义
*/

#pragma once
#include "gbbase_proto.h"
#include "cms/utility/strtok.h"
#include "cms/utility/digital_string.h"


struct TGBAreaType
{
public:
    TGBAreaType()
    {
        XPos = 0;
        YPos = 0;
        Height = 0;
        Width = 0;
    }
private:
    int XPos;
    int YPos;
    int Height;
    int Width;

public:
    virtual bool empty() const
    {
        return XPos == 0 && YPos == 0 && Height == 0 && Width == 0;
    }
    virtual void Clear()
    {
        XPos = 0;
        YPos = 0;
        Height = 0;
        Width = 0;
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode, "XPos", "int", XPos);

        ConvertHelper::ToXml::AddChildNode(xmlNode, "YPos", "int", YPos);

        ConvertHelper::ToXml::AddChildNode(xmlNode, "Height", "int", Height);

        ConvertHelper::ToXml::AddChildNode(xmlNode, "Width", "int", Width);

    }
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "XPos", XPos);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "YPos", YPos);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Height", Height);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Width", Width);

        return bRet;

    }
public:
    void SetXPos(const int nXPos)
    {
        XPos = nXPos;
    }
    const int GetXPos() const
    {
        return XPos;
    }
    void SetYPos(const int nYPos)
    {
        YPos = nYPos;
    }
    const int GetYPos() const
    {
        return YPos;
    }
    void SetHeight(const int nHeight)
    {
        Height = nHeight;
    }
    const int GetHeight() const
    {
        return Height;
    }
    void SetWidth(const int nWidth)
    {
        Width = nWidth;
    }
    const int GetWidth() const
    {
        return Width;
    }
};


/*
<KdmVideoCoveredConfig>
<Enable> ON </ Enable>
<!—图像遮挡 -->
<DetectAreaList Num="1"/>
<Item>
<XPos>0</XPos>
<YPos>0</YPos>
<Height>768</Height>
<Width>1024</Width>
<Item>
</DetectAreaList>
<!—如果有多个图像遮挡区域，可以有多个Item-->
<Sensibility>Low</Sensibility>
<!—敏感度，Low, MediumLow, Middle, MediumHigh, High-->
</KdmVideoCoveredConfig>
*/
struct TKDMGBVideoCoveredConfigType : public TGBStruct
{
public:
    TKDMGBVideoCoveredConfigType()
    {
        Clear();
    }
private:
    string Enable;                       //必选，值为ON,OFF
    vector<TGBAreaType> DetectAreaList;  //可选

    /*可选，5档敏感度，从低到高
    Low, MediumLow, Middle, MediumHigh, High*/
    string Sensibility;                  
 
 public:
    void Clear()
    {  
        Enable.clear();
        Sensibility.clear();
        DetectAreaList.clear();
        TGBStruct::Clear();      
    }
    virtual bool empty() const
    {
        return Enable.empty() && Sensibility.empty() && DetectAreaList.empty();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        if (!Enable.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "Enable", "string", Enable);
        }        
        if ( !DetectAreaList.empty() )
        {
            ConvertHelper::ToXml::AddContainerGBStyle(xmlNode, DetectAreaList, "DetectAreaList", "Item");
        }

        if (!Sensibility.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "Sensibility", "string", Sensibility);
        }
    }

    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Enable", Enable);
        ConvertHelper::ToStruct::ParseContainerGBStyle(*pXmlNode, DetectAreaList, "DetectAreaList", "Item");
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Sensibility", Sensibility);

        return bRet;
    }

public:
    void SetDetectAreaList(const vector<TGBAreaType>& vctAreaList)
    {
        DetectAreaList = vctAreaList;
    }
    const vector<TGBAreaType>& GetDetectAreaList() const
    {
        return DetectAreaList;
    }
    vector<TGBAreaType>& GetDetectAreaList()
    {
        return DetectAreaList;
    }

    void SetEnable(const string& strEnable)
    {
        Enable = strEnable;
    }
    const string& GetEnable() const
    {
        return Enable;
    }
    string &GetEnable()
    {
        return Enable;
    }

    void SetSensibility(const string& strSensibility)
    {
        Sensibility = strSensibility;
    }
    const string& GetSensibility() const
    {
        return Sensibility;
    }
    string& GetSensibility()
    {
        return Sensibility;
    }
};



/*
<KdmMotionDectionConfig>
<Enable> ON </ Enable>
<!—移动侦测使能 -->
<DetectAreaList Num="1"/>
<Item>
<XPos>0</XPos>
<YPos>0</YPos>
<Height>768</Height>
<Width>1024</Width>
<Item>
</DetectAreaList>
<!—如果有多个侦测区域，可以有多个Item-->
<Sensibility>Low</Sensibility>
<!—敏感度，Low, MediumLow, Middle, MediumHigh, High-->
</KdmMotionDectionConfig>
*/
struct TKDMGBMotionDectionConfigType : public TGBStruct
{
public:
    TKDMGBMotionDectionConfigType()
    {
        Clear();
    }
private:
    string Enable;                       //必选，值为ON,OFF
    vector<TGBAreaType> DetectAreaList;  //可选

    /*可选，5档敏感度，从低到高
    Low, MediumLow, Middle, MediumHigh, High*/
    string Sensibility;                  
 
 public:
    void Clear()
    {  
        Enable.clear();
        Sensibility.clear();
        DetectAreaList.clear();
        TGBStruct::Clear();      
    }
    virtual bool empty() const
    {
        return Enable.empty() && Sensibility.empty() && DetectAreaList.empty();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        if (!Enable.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "Enable", "string", Enable);
        }        
        if ( !DetectAreaList.empty() )
        {
            ConvertHelper::ToXml::AddContainerGBStyle(xmlNode, DetectAreaList, "DetectAreaList", "Item");
        }

        if (!Sensibility.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "Sensibility", "string", Sensibility);
        }
    }

    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Enable", Enable);
        ConvertHelper::ToStruct::ParseContainerGBStyle(*pXmlNode, DetectAreaList, "DetectAreaList", "Item");
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Sensibility", Sensibility);

        return bRet;
    }

public:
    void SetDetectAreaList(const vector<TGBAreaType>& vctAreaList)
    {
        DetectAreaList = vctAreaList;
    }
    const vector<TGBAreaType>& GetDetectAreaList() const
    {
        return DetectAreaList;
    }
    vector<TGBAreaType>& GetDetectAreaList()
    {
        return DetectAreaList;
    }

    void SetEnable(const string& strEnable)
    {
        Enable = strEnable;
    }
    const string& GetEnable() const
    {
        return Enable;
    }
    string &GetEnable()
    {
        return Enable;
    }

    void SetSensibility(const string& strSensibility)
    {
        Sensibility = strSensibility;
    }
    const string& GetSensibility() const
    {
        return Sensibility;
    }
    string& GetSensibility()
    {
        return Sensibility;
    }
};

//视频输入接口配置
//允许的取值为0-100，-1为无效
struct TGBVideoInInfConfigType : public TGBStruct
{
	TGBVideoInInfConfigType()
	{
		Clear();
	}
private:
	int Hue;
	int Contrast;
	int Brightness;
	int Saturation;

public:
	void Clear()
	{
		Hue = -1;
		Contrast = -1;
		Brightness = -1;
		Saturation = -1;
	}
	virtual bool empty() const
	{
		return Hue == -1 && Contrast == -1 && Brightness == -1 && Saturation == -1;
	}
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		ConvertHelper::ToXml::AddChildNode(xmlNode, "Hue", "int", Hue);
		ConvertHelper::ToXml::AddChildNode(xmlNode, "Contrast", "int", Contrast);
		ConvertHelper::ToXml::AddChildNode(xmlNode, "Brightness", "int", Brightness);
		ConvertHelper::ToXml::AddChildNode(xmlNode, "Saturation", "int", Saturation);
	
	}

	virtual bool FromXml(const TiXmlNode& xmlNode)
	{
		bool bRet = true;
		Clear();

		const TiXmlNode* pXmlNode = xmlNode.ToElement();

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Hue", Hue);
		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Contrast", Contrast);
		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Brightness", Brightness);
		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Saturation", Saturation);

		return bRet;
	}

public:
	void SetHue(int dwHue)
	{
		Hue = dwHue;
	}
	const int GetHue() const
	{
		return Hue;
	}
	int GetHue()
	{
		return Hue;
	}
	void SetContrast(int dwContrast)
	{
		Contrast = dwContrast;
	}
	const int GetContrast() const
	{
		return Contrast;
	}
	int GetContrast()
	{
		return Contrast;
	}
	void SetBrightness(int dwBrightness)
	{
		Brightness = dwBrightness;
	}
	const int GetBrightness() const
	{
		return Brightness;
	}
	int GetBrightness()
	{
		return Brightness;
	}
	void SetSaturation(int dwSaturation)
	{
		Saturation = dwSaturation;
	}
	const int GetSaturation() const
	{
		return Saturation;
	}
	int GetSaturation()
	{
		return Saturation;
	}
};





//视频输出接口配置和查询
struct TGBVideoOutInfConfigType : public TGBStruct
{
    TGBVideoOutInfConfigType()
    {
        Clear();
    }
private:
    string Resolution;      //分辨率
    int RefreshRate;
    int Hue;
    int Contrast;
    int Brightness;
    int Saturation;
    vector<TGBAreaType> PaneLayoutList;

public:
    void Clear()
    {
        Resolution.clear();
        RefreshRate = 0;
        Hue = 0;
        Contrast = 0;
        Brightness = 0;
        Saturation = 0;
        PaneLayoutList.clear();
    }
    virtual bool empty() const
    {
        return Resolution.empty();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        if (!Resolution.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "Resolution", "string", Resolution);
        }
     
        ConvertHelper::ToXml::AddChildNode(xmlNode, "RefreshRate", "int", RefreshRate);
        ConvertHelper::ToXml::AddChildNode(xmlNode, "Hue", "int", Hue);
        ConvertHelper::ToXml::AddChildNode(xmlNode, "Contrast", "int", Contrast);
        ConvertHelper::ToXml::AddChildNode(xmlNode, "Brightness", "int", Brightness);
        ConvertHelper::ToXml::AddChildNode(xmlNode, "Saturation", "int", Saturation);

        if (!PaneLayoutList.empty())
        {
            ConvertHelper::ToXml::AddContainerGBStyle(xmlNode, PaneLayoutList, "PaneLayoutList", "Item");
        }
    }

    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Resolution", Resolution);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "RefreshRate", RefreshRate);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Hue", Hue);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Contrast", Contrast);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Brightness", Brightness);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Saturation", Saturation);

        ConvertHelper::ToStruct::ParseContainerGBStyle(*pXmlNode, PaneLayoutList, "PaneLayoutList", "Item");

        return bRet;
    }

public:
    void SetResolution(const string& strResolution)
    {
        Resolution = strResolution;
    }
    const string& GetResolution() const
    {
        return Resolution;
    }
    string& GetResolution()
    {
        return Resolution;
    }

    void SetRefreshRate(int dwRefreshRate)
    {
        RefreshRate = dwRefreshRate;
    }    
    const int GetRefreshRate() const
    {
        return RefreshRate;
    }
    int GetRefreshRate()
    {
        return RefreshRate;
    }
    void SetHue(int dwHue)
    {
        Hue = dwHue;
    }
    const int GetHue() const
    {
        return Hue;
    }
    int GetHue()
    {
        return Hue;
    }
    void SetContrast(int dwContrast)
    {
        Contrast = dwContrast;
    }
    const int GetContrast() const
    {
        return Contrast;
    }
    int GetContrast()
    {
        return Contrast;
    }
    void SetBrightness(int dwBrightness)
    {
        Brightness = dwBrightness;
    }
    const int GetBrightness() const
    {
        return Brightness;
    }
    int GetBrightness()
    {
        return Brightness;
    }
    void SetSaturation(int dwSaturation)
    {
        Saturation = dwSaturation;
    }
    const int GetSaturation() const
    {
        return Saturation;
    }
    int GetSaturation()
    {
        return Saturation;
    }
    void SetPaneLayoutList(const vector<TGBAreaType>& vctPaneLayoutList)
    {
        PaneLayoutList = vctPaneLayoutList;
    }
    const vector<TGBAreaType>& GetPaneLayoutList() const
    {
        return PaneLayoutList;
    }
    vector<TGBAreaType>& GetPaneLayoutList()
    {
        return PaneLayoutList;
    }
};


/*
<KDM>
<EventType>OnOffLine</ EventType >
状态订阅不需要订阅告警信息。对外设来说，告警不需要订阅，有告警信息直接通知即可。（参见国标定义）
删除///< EventType >Alarm/Intelligence/AlarmIn</ EventType ><!--采用/方式进行扩展--> 
< EventType >RecordStatus</ EventType >
< EventType >GPS</ EventType >
</KDM>
*/
struct TKDMGBDevStatusEventTypes : public TGBStruct
{
public:
    TKDMGBDevStatusEventTypes()
    {
    }
private:
    vector<string> EventTypes;
public:
    void Clear()
    {
        EventTypes.clear();
        TGBStruct::Clear();
    }
    virtual bool empty() const
    {
        return EventTypes.empty();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        vector<string>::const_iterator pItem = EventTypes.begin();
        while ( pItem != EventTypes.end() )
        {
            if ( ! pItem->empty() )
            {
                ConvertHelper::ToXml::AddChildNode(xmlNode,"EventType","string",*pItem);
            }

            pItem++;
        }
    }

    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pParentXmlNode = xmlNode.ToElement();

        const TiXmlElement* pChildXmlNode = pParentXmlNode->FirstChildElement("EventType");
        while ( NULL != pChildXmlNode )
        {
            const std::string szValue = ConvertHelper::ToStruct::Private::DoParseValue(*pChildXmlNode);
            EventTypes.push_back(szValue);

            pChildXmlNode=pChildXmlNode->NextSiblingElement("EventType");
        }

        return bRet;
    }
public:
    void SetEventTypes(const vector<string>& vctEventTypes)
    {
        EventTypes = vctEventTypes;
    }
    const vector<string>& GetEventTypes() const
    {
        return EventTypes;
    }
    vector<string>& GetEventTypes()
    {
        return EventTypes;
    }
};


/*
<KDM>
<AlarmType> Intelligence </ AlarmType>
<!—智能算法，取值0为全部，1为警戒线，2为剧烈运动，3为视频诊断，4为起身检测，5为区域看防； -->
<AlarmParam Name=” Algorithm” Value=”1”/>
<AlarmParam Name=” Algorithm” Value=”2”/>
<!—如果有多个告警类型，可以有多个AlarmParam-->
</KDM>
*/
struct TKDMGBAlarmParam
{
    TKDMGBAlarmParam()
    {
        Clear();
    }
    void Clear()
    {
        Value = 0;
    }
    string Name;
    int Value;
};

struct TKDMGBAlarmType : public TGBStruct
{
public:
    TKDMGBAlarmType()
    {
    }
private:
    string AlarmType;
    vector<TKDMGBAlarmParam> AlarmParams;
public:
    void Clear()
    {
        AlarmType.clear();
        AlarmParams.clear();
        TGBStruct::Clear();
    }
    virtual bool empty() const
    {
        return AlarmType.empty() && AlarmParams.empty();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        //如果没有告警类型，则告警参数也无意义
        if ( AlarmType.empty() )
        {
            return;
        }
        ConvertHelper::ToXml::AddChildNode(xmlNode,"AlarmType","string",AlarmType);

        vector<TKDMGBAlarmParam>::const_iterator pItem = AlarmParams.begin();
        while ( pItem != AlarmParams.end() )
        {
            TiXmlElement* pAlarmParamXmlNode = new TiXmlElement("AlarmParam");
            if ( ! pItem->Name.empty() )
            {
                pAlarmParamXmlNode->SetAttribute("Name", pItem->Name);
            }
            pAlarmParamXmlNode->SetAttribute("Value", pItem->Value);
            xmlNode.LinkEndChild(pAlarmParamXmlNode);

            pItem++;
        }
    }

    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pParentXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pParentXmlNode,"AlarmType",AlarmType);

        const TiXmlElement* pChildXmlNode = pParentXmlNode->FirstChildElement("AlarmParam");
        while ( NULL != pChildXmlNode )
        {
            TKDMGBAlarmParam tParam;
            const char *s = pChildXmlNode->Attribute("Name");
            if ( s )
            {
                tParam.Name = s;
            }
            pChildXmlNode->Attribute("Value", &tParam.Value);
            AlarmParams.push_back(tParam);

            pChildXmlNode=pChildXmlNode->NextSiblingElement("AlarmParam");
        }

        return bRet;
    }
public:
    void SetAlarmType(const string& strAlarmType)
    {
        AlarmType = strAlarmType;
    }
    const string& GetAlarmType() const
    {
        return AlarmType;
    }
    string& GetAlarmType()
    {
        return AlarmType;
    }
    void SetAlarmParams(const vector<TKDMGBAlarmParam>& vctAlarmParams)
    {
        AlarmParams = vctAlarmParams;
    }
    const vector<TKDMGBAlarmParam>& GetAlarmParams() const
    {
        return AlarmParams;
    }
    vector<TKDMGBAlarmParam>& GetAlarmParams()
    {
        return AlarmParams;
    }
};

struct TFaultChnSet:public TGBStruct
{
public:
	TFaultChnSet()
	{

	}
private:
	set<string> FaultChnIds;
public:
	virtual bool empty() const
	{
		return FaultChnIds.empty();
	}
	virtual void Clear()
	{
		FaultChnIds.clear();
		TGBStruct::Clear();
	}
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		if(! FaultChnIds.empty() )
		{
			//ConvertHelper::ToXml::AddChildNode(xmlNode,"FaultChnIds","set<string>",FaultChnIds);
			string strHead = "<DeviceID>";
			string strTail = "</DeviceID>";

			string strValue;
			string strOneNode;
			string strGbid;
			set<string> ::iterator it = FaultChnIds.begin();
			while(it != FaultChnIds.end())
			{
				strOneNode = strHead;
				strGbid = *it;
				strOneNode += strGbid;
				strOneNode += strTail;
				strValue += strOneNode;
				it ++;
			}
			ConvertHelper::ToXml::AddChildNode(xmlNode,"Info","string",strValue);
		}
	}
	virtual bool FromXml(const TiXmlNode& xmlNode)
	{
		bool bRet = true;
		Clear();

		const TiXmlNode* pParentXmlNode = xmlNode.ToElement();

		const TiXmlElement* pChildXmlNode = pParentXmlNode->FirstChildElement("DeviceID");
		while ( NULL != pChildXmlNode )
		{
            if (NULL != pChildXmlNode->FirstChild())
            {
			const std::string &strDevId = pChildXmlNode->FirstChild()->ValueStr();
			FaultChnIds.insert(strDevId);
			pChildXmlNode=pChildXmlNode->NextSiblingElement("DeviceID");
            }
            else
            {
                pChildXmlNode=pChildXmlNode->NextSiblingElement("DeviceID");
            }
		}

		return bRet;
	}

public:
	void SetFaultChnIds(const set<string>& tFaultChnIds)
	{
		FaultChnIds = tFaultChnIds;
	}
	const set<string>& GetFaultChnIds() const
	{
		return FaultChnIds;
	}
	set<string>& GetFaultChnIds()
	{
		return FaultChnIds;
	}
};

struct TKDMGBDeviceCatalogExt : public TGBStruct
{
public:
    TKDMGBDeviceCatalogExt()
    {
		Clear();
    }
private:
    string GroupID;
	int gpsCap;
	int lonlatPrecision; //经纬度字段精确度，此字段不会序列化并上报（仅用于业务层将配置传入gbproto库）
	bool blockSerialize; //block字段是否无条件序列化，默认false
    bool isParentIdFilt216Only;  //parentId是只上报216， 此字段不会序列化
    bool reserveEventOnly;   //是否只保留deviceID和Event字段

public:
    virtual void Clear()
    {
        GroupID.clear();
		gpsCap = 0;
		lonlatPrecision = 6;
        isParentIdFilt216Only = false;
		blockSerialize = false;
        reserveEventOnly = false;
        TGBStruct::Clear();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        if(! GroupID.empty() )
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode,"GroupID","string",GroupID);
        }

		if (gpsCap != 0)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"gpsCap","int",gpsCap);
		}
    }
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"GroupID",GroupID);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "gpsCap", gpsCap);

        return bRet;
    }
    virtual bool empty() const
    {
        return GroupID.empty()&&gpsCap == 0;
    }
public:
    void SetReserveEventOnly(bool bReserveEventOnly)
    {
       reserveEventOnly =  bReserveEventOnly;
    }

    bool GetReserveEventOnly()
    {
       return  reserveEventOnly;
    }
    
    const bool GetReserveEventOnly() const
    {
       return  reserveEventOnly;
    }

    void SetGroupID(const string& strGroupID)
    {
        GroupID = strGroupID;
    }
    const string& GetGroupID() const
    {
        return GroupID;
    }
    string& GetGroupID()
    {
        return GroupID;
    }
	void SetGpsCap(int nGpsCap)
	{
		gpsCap = nGpsCap;
	}
	const int GetExtCapSet() const
	{
		return gpsCap;
	}
	int GetGpsCap()
	{
		return gpsCap;
	}
	void SetLonLatPrecision(int nLonLatBits)
	{
		lonlatPrecision = nLonLatBits;
	}
	const int GetLonLatPrecision() const
	{
		return lonlatPrecision;
	}
	int GetLonLatPrecision()
	{
		return lonlatPrecision;
	}
    void SetIsParentFilt216Only(bool bIsParentFilt216Only)
    {
        isParentIdFilt216Only = bIsParentFilt216Only;
    }
    const bool GetIsParentIdFilt216Only() const 
    {
        return isParentIdFilt216Only;
    }
    bool GetIsParentIdFilt216Only()
    {
        return isParentIdFilt216Only;
	}

	void SetBlockSerialize(bool bBlockSerialize)
	{
		blockSerialize = bBlockSerialize;
	}
	const bool GetBlockSerialize() const
	{
		return blockSerialize;
	}
	bool GetBlockSerialize()
	{
		return blockSerialize;
	}
};

struct TGbDeviceCatalogInfo :public TGBStruct
{
public:
	TGbDeviceCatalogInfo()
	{
		PTZType = 0;
		PositionType = 0;
		RoomType = 0;
		UseType = 0;
		SupplyLightType = 0;
		DirectionType = 0;
		Resolution.clear();
		BusinessGroupID.clear();
        DownloadSpeed.clear();
        SVCSpaceSupportMode = 0;
        SVCTimeSupportMode = 0;
	}
private:
	
    int PTZType; //1-球机，2-半球，3-固定枪机，4-遥控枪机。 

	/*1-省际检查站，2-党政机关，3-车站码头，4-中心广场，5-体育场馆
	  6-商业中心，7-宗教场所，8-校园周边，9-治安复杂区域，10-交通干线
	*/
    int PositionType; 
    int RoomType; //1-室外，2-室内
    int UseType; //1-治安，2-交通，3-重点
    int SupplyLightType;  //1-无补光，2-红外补光，3-白光补光
	
	/*1-东，2-西，3-南，4-北
	  5-东南，6-东北，7-西南，8-西北
	*/
    int DirectionType;

	/*摄像机支持的分辨率：可有多个分辨率值，各个取值间以“/”
	分隔，分辨率取值参见SDP中f字段规定:QCIF、CIF、4CIF、D1、720P、1080P*/
    string Resolution;
    string BusinessGroupID;
    string DownloadSpeed;
    int SVCSpaceSupportMode;
    int SVCTimeSupportMode;
	int    SourceID; //取值最小为0，若为视频编码通道，则为视频源编号；若为音频编码通道，则为音频源编号
public:
    virtual void Clear()
	{
		PTZType = 0;
		PositionType = 0;
		RoomType = 0;
		UseType = 0;
		SupplyLightType = 0;
		DirectionType = 0;
		SourceID=-1;
		Resolution.clear();
		BusinessGroupID.clear();
        DownloadSpeed.clear();
        SVCSpaceSupportMode = 0;
        SVCTimeSupportMode = 0;
		TGBStruct::Clear();
	}
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		if (0 != PTZType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"PTZType","int",PTZType);
		}

		if (0 != PositionType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"PositionType","int",PositionType);
		}

		if (0 != RoomType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"RoomType","int",RoomType);
		}

		if (0 != UseType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"UseType","int",UseType);
		}

		if (0 != SupplyLightType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"SupplyLightType","int",SupplyLightType);
		}

		if (0 != DirectionType)
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"DirectionType","int",DirectionType);
		}

		if(! Resolution.empty() )
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"Resolution","string",Resolution);
		}

		if(! BusinessGroupID.empty() )
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"BusinessGroupID","string",BusinessGroupID);
		}

        if (!DownloadSpeed.empty())
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "DownloadSpeed", "string", DownloadSpeed);
        }
        if (0 != SVCSpaceSupportMode)
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "SVCSpaceSupportMode", "int", SVCSpaceSupportMode);
        }
        if (0 != SVCTimeSupportMode)
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode, "SVCTimeSupportMode", "int", SVCTimeSupportMode);
        }
	}
    virtual bool FromXml(const TiXmlNode& xmlNode)
	{
		bool bRet = true;
		Clear();

		const TiXmlNode* pXmlNode = xmlNode.ToElement();

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"PTZType",PTZType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"PositionType",PositionType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"RoomType",RoomType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"UseType",UseType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"SourceID",SourceID);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"SupplyLightType",SupplyLightType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"DirectionType",DirectionType);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Resolution",Resolution);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"BusinessGroupID",BusinessGroupID);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "DownloadSpeed", DownloadSpeed);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "SVCSpaceSupportMode", SVCSpaceSupportMode);
        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "SVCTimeSupportMode", SVCTimeSupportMode);
		return bRet;

	}
	virtual bool empty() const
	{
		return PTZType == 0 && PositionType == 0 && RoomType == 0 && UseType == 0
               && SupplyLightType == 0 && DirectionType == 0 && Resolution.empty() && BusinessGroupID.empty() && DownloadSpeed.empty() && SVCSpaceSupportMode == 0 && SVCTimeSupportMode == 0&& SourceID==-1;
	}
	const string ToString() const
	{
		string strInfo;
		strInfo = digital_string(PTZType) + "|";
		strInfo += digital_string(PositionType) + "|";
		strInfo += digital_string(RoomType) + "|";
		strInfo += digital_string(UseType) + "|";
		strInfo += digital_string(SupplyLightType) + "|";
		strInfo += digital_string(DirectionType) + "|";
		strInfo += (Resolution.empty()?"NA":Resolution) + "|";
        strInfo += (BusinessGroupID.empty() ? "NA" : BusinessGroupID) + "|";
        strInfo += (DownloadSpeed.empty() ? "NA" : DownloadSpeed) + "|";
        strInfo += digital_string(SVCSpaceSupportMode) + "|";
        strInfo += digital_string(SVCTimeSupportMode);
		return strInfo;

	}
	void FromString(const string& strInfo)
	{
		vector<string> vecInfo;
		strtok(strInfo.c_str(), "|", vecInfo);
		if (vecInfo.size() < 6) //兼容老版本(使用的是不同分隔符)，如果该字段为空，则不解析，直接使用默认值
		{
			return;
		}
		PTZType = atoi(vecInfo[0].c_str());
		PositionType = atoi(vecInfo[1].c_str());
		RoomType = atoi(vecInfo[2].c_str());
		UseType = atoi(vecInfo[3].c_str());
		SupplyLightType = atoi(vecInfo[4].c_str());
		DirectionType = atoi(vecInfo[5].c_str());
		if (vecInfo.size() > 6)
		{
			Resolution = vecInfo[6];
			if (Resolution == "NA")
			{
				Resolution.clear();
			}
		}
		if (vecInfo.size() > 7)
		{
			BusinessGroupID = vecInfo[7];
			if (BusinessGroupID == "NA")
			{
				BusinessGroupID.clear();
			}
		}
        if (vecInfo.size() > 8)
        {
            DownloadSpeed = vecInfo[8];
            if (DownloadSpeed == "NA")
            {
                DownloadSpeed.clear();
            }
        }
        if (vecInfo.size() > 9)
        {
            SVCSpaceSupportMode = atoi(vecInfo[9].c_str());
        }
        if (vecInfo.size() > 10)
        {
            SVCTimeSupportMode = atoi(vecInfo[10].c_str());
        }
	}
public:
    void SetPTZType(const int nPTZType)
    {
        PTZType = nPTZType;
    }
    const int GetPTZType() const
    {
        return PTZType;
    }
    void SetPositionType(const int nPositionType)
    {
        PositionType = nPositionType;
    }
    const int GetPositionType() const
    {
        return PositionType;
    }
    void SetRoomType(const int nRoomType)
    {
        RoomType = nRoomType;
    }
    const int GetRoomType() const
    {
        return RoomType;
    }
    void SetUseType(const int nUseType)
    {
        UseType = nUseType;
    }
    const int GetUseType() const
    {
        return UseType;
    }
	
	void SetSourceID(const int nSourceID)
	{
		SourceID = nSourceID;
	}
	const int GetSourceID() const
	{
		return SourceID;
	}
    void SetSupplyLightType(const int nSupplyLightType)
    {
        SupplyLightType = nSupplyLightType;
    }
    const int GetSupplyLightType() const
    {
        return SupplyLightType;
    }
    void SetDirectionType(const int nDirectionType)
    {
        DirectionType = nDirectionType;
    }
    const int GetDirectionType() const
    {
        return DirectionType;
    }
    void SetResolution(const string& strResolution)
    {
        Resolution = strResolution;
    }
    const string& GetResolution() const
    {
        return Resolution;
    }
    string& GetResolution()
    {
        return Resolution;
    }
    void SetBusinessGroupID(const string& strBusinessGroupID)
    {
        BusinessGroupID = strBusinessGroupID;
    }
    const string& GetBusinessGroupID() const
    {
        return BusinessGroupID;
    }
    string& GetBusinessGroupID()
    {
        return BusinessGroupID;
    }

    void SetDownloadSpeed(const string &strDownloadSpeed)
    {
        DownloadSpeed = strDownloadSpeed;
    }
    const string& GetDownloadSpeed() const
    {
        return DownloadSpeed;
    }
    string& GetDownloadSpeed()
    {
        return DownloadSpeed;
    }
    void SetSVCSpaceSupportMode(const int nSVCSpaceSupportMode)
    {
        SVCSpaceSupportMode = nSVCSpaceSupportMode;
    }
    const int GetSVCSpaceSupportMode() const
    {
        return SVCSpaceSupportMode;
    }
    void SetSVCTimeSupportMode(const int nSVCTimeSupportMode)
    {
        SVCTimeSupportMode = nSVCTimeSupportMode;
    }
    const int GetSVCTimeSupportMode() const
    {
        return SVCTimeSupportMode;
    }
	bool operator==(const TGbDeviceCatalogInfo& tInfo)
	{
		return (PTZType == tInfo.PTZType && PositionType == tInfo.PositionType && RoomType == tInfo.RoomType 
			&& UseType == tInfo.UseType  && SupplyLightType == tInfo.SupplyLightType && DirectionType == tInfo.DirectionType 
                && Resolution == tInfo.Resolution && BusinessGroupID == tInfo.BusinessGroupID && DownloadSpeed == tInfo.DownloadSpeed
                && SVCSpaceSupportMode == tInfo.SVCSpaceSupportMode && SVCTimeSupportMode == tInfo.SVCTimeSupportMode);
	}
};

struct TGBDeviceCataLog : public TGBStruct
{
public:
    TGBDeviceCataLog()
    {
        Longitude = INVALID_LONGITUDE;
        Latitude = INVALID_LATITUDE;
        CatalogType = 0;        //国标外设
        RecLocation = 0;        //国标外设
        Parental = 0;       //国标外设中含义不一样
        SafetyWay = 0;
        RegisterWay = 0;
        Certifiable = 0;
        ErrCode = 0;
        Secrecy = 0;
        Port = 0;
    }
private:
    string DeviceID;
    TKDMGBDeviceCatalogExt KDM;
    string Name;
    string Manufacturer;
    int CatalogType;        //国标外设
    int RecLocation;        //国标外设
    string OperateType;     //国标外设
    string Model;
    string Owner;
    string CivilCode;
    string Block;
    string Address;
    int Parental;       //国标外设中含义不一样
    string ParentID;
    int SafetyWay;
    int RegisterWay;
    string CertNum;
    int Certifiable;
    int ErrCode;
    string EndTime;
    int Secrecy;
    string IPAddress;
    int Port;
    string Password;
    string Status;
    double Longitude;
    double Latitude;
    string BusinessGroupID; //虚拟组织所属的业务分组
	string Event; 
	TGbDeviceCatalogInfo Info;
public:
    virtual bool empty() const
    {
        return false;
    }
    virtual void Clear()
    {
        DeviceID.clear();
        KDM.Clear();
        Name.clear();
        Manufacturer.clear();
        CatalogType = 0;
        RecLocation = 0;
        OperateType.clear();
        Model.clear();
        Owner.clear();
        CivilCode.clear();
        Block.clear();
        Address.clear();
        Parental = 0;
        ParentID.clear();
        SafetyWay = 0;
        RegisterWay = 0;
        CertNum.clear();
        Certifiable = 0;
        ErrCode = 0;
        EndTime.clear();
        Secrecy = 0;
        IPAddress.clear();
        Port = 0;
        Password.clear();
        Status.clear();
        Longitude = INVALID_LONGITUDE;
        Latitude = INVALID_LATITUDE;
        BusinessGroupID.clear();
		Event.clear();
        Info.Clear();
        TGBStruct::Clear();
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        if (KDM.GetReserveEventOnly() == true) 
        {
                if(! DeviceID.empty() )
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                }
                if (!Event.empty())
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode, "Event", "string", Event);
                }
        }
        else
        {
            if (DeviceID.size() == 20)
            {
                if (DeviceID.compare(10, 3, "200", 3) == 0)
                {
                    if(! DeviceID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                    }
                    if(! Name.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Name","string",Name);
                    }
                    if(! Manufacturer.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Manufacturer","string",Manufacturer);
                    }
                    if(! Model.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Model","string",Model);
                    }

                    if(! Owner.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Owner","string",Owner);
                    }

                    if(! CivilCode.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"CivilCode","string",CivilCode);
                    }

                    if(! Address.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Address","string",Address);
                    }

                    if(! IPAddress.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"IPAddress","string",IPAddress);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Port","int",Port);

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"RegisterWay","int",RegisterWay);
                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Secrecy","int",Secrecy);

                    if(! ParentID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"ParentID","string",ParentID);
                    }

                    if(! Status.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Status","string",Status);
                    }

                    if (!OperateType.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "OperateType", "string", OperateType);
                    }

                    if (!Event.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Event", "string", Event);
                    }
                }
                else if (DeviceID.compare(10, 3, "215", 3) == 0)
                {
                    if(! DeviceID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                    }
                    if(! Name.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Name","string",Name);
                    }
                    if(! ParentID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"ParentID","string",ParentID);
                    }

                    if (!OperateType.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "OperateType", "string", OperateType);
                    }

                    if (!Event.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Event", "string", Event);
                    }
                }
                else if (DeviceID.compare(10, 3, "216", 3) == 0)
                {
                    if(! DeviceID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                    }
                    if(! Name.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Name","string",Name);
                    }
                    if(! ParentID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"ParentID","string",ParentID);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"BusinessGroupID","string",BusinessGroupID);

                    if (!OperateType.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "OperateType", "string", OperateType);
                    }

                    if (!Event.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Event", "string", Event);
                    }
                }
                else
                {
                    if(! DeviceID.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                    }

                    if(! KDM.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"KDM","TKDMGBDeviceCatalogExt",KDM);
                    }

                    if(! Name.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Name","string",Name);
                    }

                    if(! Manufacturer.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Manufacturer","string",Manufacturer);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"CatalogType","int",CatalogType);

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"RecLocation","int",RecLocation);

                    if(! OperateType.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"OperateType","string",OperateType);
                    }

                    if(! Model.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Model","string",Model);
                    }

                    if(! Owner.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Owner","string",Owner);
                    }
                    else
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Owner","string",string("1"));
                    }

                    if(! CivilCode.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"CivilCode","string",CivilCode);
                    }

                    if(! Block.empty() || KDM.GetBlockSerialize() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Block","string",Block);
                    }

                    if(! Address.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Address","string",Address);
                    }
                    else
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Address","string",string("1"));
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Parental","int",Parental);

                    if(! ParentID.empty() )
                    {
                    string strTmpParentId;
                    vector<string> vctParentId;
                    if (KDM.GetIsParentIdFilt216Only())
                    {
                        bool bHas200 = false;
                        bool bHas216 = false;
                        vector<string> strTmp216IdList;
                        strtok(ParentID.c_str(), "/", vctParentId);
                        for (vector<string>::iterator iter = vctParentId.begin(); iter != vctParentId.end(); iter++)
                        {
                            if (iter->compare(10, 3, "200", 3) == 0)
                            {
                                bHas200 = true;
                            }
                            else if (iter->compare(10, 3, "216", 3) == 0)
                            {
                                bHas216 = true;
                                strTmp216IdList.push_back(*iter);
                            }
                        }
                        if (bHas200 && bHas216)
                        {
                            for (vector<string>::iterator iter216 = strTmp216IdList.begin(); iter216 != strTmp216IdList.end(); iter216++)
                            {
                                if (!strTmpParentId.empty())
                                {
                                    strTmpParentId = strTmpParentId + string("/");
                                }
                                strTmpParentId = strTmpParentId + *iter216;
                            }
                        }
                        else
                        {
                            strTmpParentId = ParentID;
                        }
                    }
                    else
                    {
                        strTmpParentId = ParentID;
                    }
                    if (!strTmpParentId.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "ParentID", "string", strTmpParentId);
                    }
                }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"SafetyWay","int",SafetyWay);

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"RegisterWay","int",RegisterWay);

                    if(! CertNum.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"CertNum","string",CertNum);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Certifiable","int",Certifiable);

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"ErrCode","int",ErrCode);

                    if(! EndTime.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"EndTime","string",EndTime);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Secrecy","int",Secrecy);

                    if(! IPAddress.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"IPAddress","string",IPAddress);
                    }

                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Port","int",Port);

                    if(! Password.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Password","string",Password);
                    }

                    if(! Status.empty() )
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Status","string",Status);
                    }

                    if ((Longitude - 180 <= 1e-5) && (180-Longitude <= 1e-5)
                        ||Longitude <180.0&&Longitude>-180.0)
                    {
                        s8 value[32]={0};
                        sprintf(value, "%.*f",KDM.GetLonLatPrecision(), Longitude);
                        string strLon = string(value);

                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Longitude", "string", strLon);
                    }

                    if ((Latitude - 90 <= 1e-5) && (90-Latitude <= 1e-5)
                        ||Latitude <90.0 && Latitude >-90.0)
                    {
                        s8 value[32]={0};
                        sprintf(value, "%.*f",KDM.GetLonLatPrecision(), Latitude);
                        string strLat = string(value);

                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Latitude", "string", strLat);
                    }

                    if (! Event.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode,"Event","string",Event);
                    }


                    if (! Info.empty())
                    {
                        ConvertHelper::ToXml::AddChildNode(xmlNode, "Info", "TGbDeviceCatalogInfo", Info);
                    }
                }
            }
            else
            {
                // civilcode
                if(! DeviceID.empty() )
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
                }
                if(! Name.empty() )
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode,"Name","string",Name);
                }

                if (!OperateType.empty())
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode, "OperateType", "string", OperateType);
                }

                if (!Event.empty())
                {
                    ConvertHelper::ToXml::AddChildNode(xmlNode, "Event", "string", Event);
                }
            }
        }
    }
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode* pXmlNode = xmlNode.ToElement();

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"DeviceID",DeviceID);

        ConvertHelper::ToStruct::ParseChildNode(xmlNode,"KDM",KDM);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Name",Name);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Manufacturer",Manufacturer);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"CatalogType",CatalogType);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"RecLocation",RecLocation);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"OperateType",OperateType);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Model",Model);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Owner",Owner);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"CivilCode",CivilCode);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Block",Block);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Address",Address);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Parental",Parental);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"ParentID",ParentID);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"SafetyWay",SafetyWay);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"RegisterWay",RegisterWay);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"CertNum",CertNum);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Certifiable",Certifiable);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"ErrCode",ErrCode);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"EndTime",EndTime);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Secrecy",Secrecy);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"IPAddress",IPAddress);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Port",Port);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Password",Password);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Status",Status);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Longitude",Longitude);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"Latitude",Latitude);

        ConvertHelper::ToStruct::ParseChildNode(*pXmlNode,"BusinessGroupID",BusinessGroupID);

		ConvertHelper::ToStruct::ParseChildNode(*pXmlNode, "Event", Event);

		ConvertHelper::ToStruct::ParseChildNode(xmlNode,"Info",Info);
        return bRet;
    }
public:
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
    }
    void SetKDM(const TKDMGBDeviceCatalogExt& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBDeviceCatalogExt& GetKDM() const
    {
        return KDM;
    }
    TKDMGBDeviceCatalogExt& GetKDM()
    {
        return KDM;
    }
    void SetName(const string& strName)
    {
        Name = strName;
    }
    const string& GetName() const
    {
        return Name;
    }
    string& GetName()
    {
        return Name;
    }
    void SetManufacturer(const string& strManufacturer)
    {
        Manufacturer = strManufacturer;
    }
    const string& GetManufacturer() const
    {
        return Manufacturer;
    }
    string& GetManufacturer()
    {
        return Manufacturer;
    }
    void SetCatalogType(const int nCatalogType)
    {
        CatalogType = nCatalogType;
    }
    const int GetCatalogType() const
    {
        return CatalogType;
    }
    void SetRecLocation(const int nRecLocation)
    {
        RecLocation = nRecLocation;
    }
    const int GetRecLocation() const
    {
        return RecLocation;
    }
    void SetOperateType(const string& strOperateType)
    {
        OperateType = strOperateType;
    }
    const string& GetOperateType() const
    {
        return OperateType;
    }
    string& GetOperateType()
    {
        return OperateType;
    }
    void SetModel(const string& strModel)
    {
        Model = strModel;
    }
    const string& GetModel() const
    {
        return Model;
    }
    string& GetModel()
    {
        return Model;
    }
    void SetOwner(const string& strOwner)
    {
        Owner = strOwner;
    }
    const string& GetOwner() const
    {
        return Owner;
    }
    string& GetOwner()
    {
        return Owner;
    }
    void SetCivilCode(const string& strCivilCode)
    {
        CivilCode = strCivilCode;
    }
    const string& GetCivilCode() const
    {
        return CivilCode;
    }
    string& GetCivilCode()
    {
        return CivilCode;
    }
    void SetBlock(const string& strBlock)
    {
        Block = strBlock;
    }
    const string& GetBlock() const
    {
        return Block;
    }
    string& GetBlock()
    {
        return Block;
    }
    void SetAddress(const string& strAddress)
    {
        Address = strAddress;
    }
    const string& GetAddress() const
    {
        return Address;
    }
    string& GetAddress()
    {
        return Address;
    }
    void SetParental(const int nParental)
    {
        Parental = nParental;
    }
    const int GetParental() const
    {
        return Parental;
    }
    void SetParentID(const string& strParentID)
    {
        ParentID = strParentID;
    }
    const string& GetParentID() const
    {
        return ParentID;
    }
    string& GetParentID()
    {
        return ParentID;
    }
    void SetSafetyWay(const int nSafetyWay)
    {
        SafetyWay = nSafetyWay;
    }
    const int GetSafetyWay() const
    {
        return SafetyWay;
    }
    void SetRegisterWay(const int nRegisterWay)
    {
        RegisterWay = nRegisterWay;
    }
    const int GetRegisterWay() const
    {
        return RegisterWay;
    }
    void SetCertNum(const string& strCertNum)
    {
        CertNum = strCertNum;
    }
    const string& GetCertNum() const
    {
        return CertNum;
    }
    string& GetCertNum()
    {
        return CertNum;
    }
    void SetCertifiable(const int nCertifiable)
    {
        Certifiable = nCertifiable;
    }
    const int GetCertifiable() const
    {
        return Certifiable;
    }
    void SetErrCode(const int nErrCode)
    {
        ErrCode = nErrCode;
    }
    const int GetErrCode() const
    {
        return ErrCode;
    }
    void SetEndTime(const string& strEndTime)
    {
        EndTime = strEndTime;
    }
    const string& GetEndTime() const
    {
        return EndTime;
    }
    string& GetEndTime()
    {
        return EndTime;
    }
    void SetSecrecy(const int nSecrecy)
    {
        Secrecy = nSecrecy;
    }
    const int GetSecrecy() const
    {
        return Secrecy;
    }
    void SetIPAddress(const string& strIPAddress)
    {
        IPAddress = strIPAddress;
    }
    const string& GetIPAddress() const
    {
        return IPAddress;
    }
    string& GetIPAddress()
    {
        return IPAddress;
    }
    void SetPort(const int nPort)
    {
        Port = nPort;
    }
    const int GetPort() const
    {
        return Port;
    }
    void SetPassword(const string& strPassword)
    {
        Password = strPassword;
    }
    const string& GetPassword() const
    {
        return Password;
    }
    string& GetPassword()
    {
        return Password;
    }
    void SetStatus(const string& strStatus)
    {
        Status = strStatus;
    }
    const string& GetStatus() const
    {
        return Status;
    }
    string& GetStatus()
    {
        return Status;
    }
    void SetLongitude(const double fLongitude)
    {
        Longitude = fLongitude;
    }
    const double GetLongitude() const
    {
        return Longitude;
    }
    void SetLatitude(const double fLatitude)
    {
        Latitude = fLatitude;
    }
    const double GetLatitude() const
    {
        return Latitude;
    }
public:
    void SetBusinessGroupID(const string& strBusinessGroupID)
    {
        BusinessGroupID = strBusinessGroupID;
    }
    const string& GetBusinessGroupID() const
    {
        return BusinessGroupID;
    }
    string& GetBusinessGroupID()
    {
        return BusinessGroupID;
    }

	void SetEvent(const string& strEvent)
	{
		Event = strEvent;
	}
	const string& GetEvent() const
	{
		return Event;
	}
	string& GetEvent()
	{
		return Event;
	}

	void SetInfo(const TGbDeviceCatalogInfo& tInfo)
	{
		Info = tInfo;
	}
	const TGbDeviceCatalogInfo& GetInfo() const
	{
		return Info;
	}
	TGbDeviceCatalogInfo& GetInfo()
	{
		return Info;
	}
};


//默认时，必须设置element和content才能序列化
//设置oldType为true时，只需要设置content
struct TGBTransparentData
{
public:
    TGBTransparentData()
    {
       element.clear();
       content.clear();
       oldType = false;
    }
private:
    string element;
    string content;
    bool oldType;

public:
    virtual bool empty() const
    {
        return content.empty();
    }
    virtual void Clear()
    {
       element.clear();
       content.clear();
       oldType = false;
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode &xmlNode) const
    {
        if (oldType == true)
        {
              TiXmlText * pMember = new TiXmlText (content);
              xmlNode.LinkEndChild(pMember);
        }
        else
        {
            if (element.empty())
            {
                return;
            }
            ConvertHelper::ToXml::AddChildNode(xmlNode, element, "string", content);
        }
    }

    virtual bool FromXml(const TiXmlNode &xmlNode)
    {
        bool bRet = true;
        Clear();

        const TiXmlNode *pXmlNode = xmlNode.ToElement();
        //element名称不确定，且只存在第一个元素，则直接解析第一个元素

        const TiXmlNode* pChildNode = pXmlNode->FirstChild();
        if (!pChildNode)
        {
            return bRet;
        }

        //没有子的情况下，就认为是旧的方法
        if (!pChildNode->FirstChild())
        {
            oldType = true;
            content = pChildNode->Value();
        }
        else
        {
            oldType = false;
            const TiXmlNode* pElem = pChildNode->ToElement();
            if (!pElem)
            {
                return bRet;
            }
            element = pElem->Value();

            const TiXmlNode *pContent = pChildNode->FirstChild();
            if (!pContent)
            {
                return bRet;
            }
            content = pContent->Value();
        }

        return bRet;
    }
public:
    void SetElement(const string &strElement)
    {
        element = strElement;
    }
    const string& GetElement() const
    {
        return element;
    }
    string& GetElement()
    {
        return element;
    }
    void SetContent(const string &strContent)
    {
        content = strContent;
    }
    const string& GetContent() const
    {
        return content;
    }
    string& GetContent()
    {
        return content;
    }
    void SetIsOldType(const bool bIsOldType)
    {
        oldType = bIsOldType;
    }
    const bool GetIsOldType() const
    {
        return oldType;
    }
};

struct TKdmCapability:public TGBStruct
{
public:
	TKdmCapability()
	{
		this->Clear();
	}
private:
	string MRTC;
public:
	virtual void Clear()
	{
		MRTC.clear();
		TGBStruct::Clear();
	}
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		if(! MRTC.empty() )
		{
			TiXmlElement* pMember = new TiXmlElement("StringItem");
			pMember->SetAttribute("Name", "MRTC");
			pMember->SetAttribute("Value", MRTC);
			xmlNode.LinkEndChild(pMember);
		}
	}
	virtual bool FromXml(const TiXmlNode& xmlNode)
	{
		bool bRet = true;
		Clear();

		string strTmp;
		for( const TiXmlNode* child = xmlNode.FirstChild(); child; child = child->NextSibling())
		{
			const TiXmlElement *pXmlElement = child->ToElement();

			if(TIXML_SUCCESS==pXmlElement->QueryValueAttribute(string("Name"), &strTmp) && strTmp == "MRTC")
			{
				if(TIXML_SUCCESS==pXmlElement->QueryValueAttribute(string("Value"), &strTmp))
					MRTC=strTmp;
			}
		}

		return bRet;
	}
	virtual bool empty() const
	{
		return MRTC.empty();
	}

public:
	void SetMRTC(bool val)
	{
		MRTC=val?"true":"false";
	}
	bool GetMRTC() const
	{
		return MRTC=="true";
	}
};

//End of file

