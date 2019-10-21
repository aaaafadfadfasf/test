/*****************************************************************************
   ģ����      : mps_struct
   �ļ���      : mps_struct.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: mps_struct.h ������ӵ�ͼ��������ӿ��õ��Ľṹ��
   ����        : zhouchanghong
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/7/02   1.0         �ܲ���        ����
******************************************************************************/

#ifndef __MPS_STRUCT_H__
#define __MPS_STRUCT_H__

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

using std::string;
using std::vector;
using std::map;
using std::list;

class TiXmlDocument;
class TiXmlNode;

typedef string EmMapType;
#define MAP_GOOGLE "google"
#define MAP_JPEG	"jpeg"

#define MPS_DEF_FLOAT_ZERO	(0.0f)

typedef std::map<string,string> MODULE_ATTRI_MAP;

class MapTypeInfo
{
private:
	EmMapType name;	             	//��ͼ��������
	string version;	             	//��ͼ����汾��
	string accessUrl;	            //���ʷ����ַ
	               							
	MODULE_ATTRI_MAP moduleAttrs;		   //����֧�ֵ�ģ�飬��gps��ʷ�켣ģ���������С�gps���ַ���,�������������google��ͼ���������tiles��
										   //keyΪģ������valueΪģ���Ӧ������ֵ����keyΪtilesʱ��value������ߵ�ͼ�ķ��ʵ�ַ��
										   //��Ӧģ��������ֶκ����
										   //    key  |  value    |   ����
										   //    gps  |  local/IP |   local��ʾ�������ݿ⣬IP��ַ��ΪԶ�����ݿ��ַ
										   //    tiles | url      |   url���ʵ�ַ������Ϊ���Ŀ¼�;��Ե�ַ
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
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

    void SetAccessUrl(const string& strAccessUrl)
    {
        accessUrl = strAccessUrl;
    }
    string& GetAccessUrl()
    {
        return accessUrl;
    }
    const string& GetAccessUrl() const
    {
        return accessUrl;
    }
    void SetVersion(const string& strVarsion)
    {
        version = strVarsion;
    }
    string& GetVersion()
    {
        return version;
    }
    const string& GetVersion() const
    {
        return version;
    }

    void SetModuleAttrs(const MODULE_ATTRI_MAP& cModuleAttrs)
    {
        moduleAttrs = cModuleAttrs;
    }
    MODULE_ATTRI_MAP& GetModuleAttrs()
    {
        return moduleAttrs;
    }
    const MODULE_ATTRI_MAP& GetModuleAttrs() const
    {
        return moduleAttrs;
    }

};


class MapPosition		//��ͼ����㣬��google��ͼ�б���Ϊ��γ�ȣ���Jpeg��ͼ�б���Ϊ���ص�
{
public:
	MapPosition()
	{
		longitude = 0.0;
		latitude = 0.0;
	}
	MapPosition(double lng,double lat)
	{
		longitude = lng;
		latitude = lat;
	}
private:
	double	longitude;	             	// ����.
	double	latitude;	               // γ��.

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetLongitude(const double cLongitude)
    {
        longitude = cLongitude;
    }
    double GetLongitude()
    {
        return longitude;
    }
    const double GetLongitude() const
    {
        return longitude;
    }

    void SetLatitude(const double cLatitude)
    {
        latitude = cLatitude;
    }
    double GetLatitude()
    {
        return latitude;
    }
    const double GetLatitude() const
    {
        return latitude;
    }

};

class Bounds		//�߽�
{
private:
	MapPosition leftTop;		                        
	MapPosition rightBottom;	                             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
   

public:
	Bounds()
	{
		
	}
	Bounds(MapPosition leftTop,MapPosition rightBottom)
	{
		this->leftTop = leftTop;
		this->rightBottom = rightBottom;
	}
    void SetLeftTop(const MapPosition& cLeftTop)
    {
        leftTop = cLeftTop;
    }
    MapPosition& GetLeftTop()
    {
        return leftTop;
    }
    const MapPosition& GetLeftTop() const
    {
        return leftTop;
    }

    void SetRightBottom(const MapPosition& cRightBottom)
    {
        rightBottom = cRightBottom;
    }
    MapPosition& GetRightBottom()
    {
        return rightBottom;
    }
    const MapPosition& GetRightBottom() const
    {
        return rightBottom;
    }

};

class MapInfo
{
private:
	string mapId;	             	// ��ͼId
	string mapName;	             	// ��ͼ����.

	EmMapType mapType;              // ��ͼ����.

	string mapUrl;	             	// ��ͼͼƬ�ļ�url. ( �ɵ�ͼ��������ʽ ) jpeg��ͼҪ���ϴ�ͼƬ.

	string parentMapId;             // ����ͼId.

	string description;	            // ����

	// ��ͼ�߽�.
	Bounds bounds;	             

	// ���ŷ�Χ.
	int minScaleLevel;	             
	int maxScaleLevel;	             
	int defaultScaleLevel;	        // Ĭ�ϵ����ż���

	MapPosition centerPos;	             
	bool earthCoord;              	//���õ�����ϵ
	string version;	             	// ��ͼ�汾.?
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
	MapInfo()
	{
		mapType = MAP_JPEG;
		minScaleLevel = 0;
		maxScaleLevel = 17;
		defaultScaleLevel = 10;
		version = "1.0";
		earthCoord = true;
		mapId = CUUID();
	}
    void SetMapId(const string& strMapId)
    {
        mapId = strMapId;
    }
    string& GetMapId()
    {
        return mapId;
    }
    const string& GetMapId() const
    {
        return mapId;
    }

    void SetMapName(const string& strMapName)
    {
        mapName = strMapName;
    }
    string& GetMapName()
    {
        return mapName;
    }
    const string& GetMapName() const
    {
        return mapName;
    }

    void SetMapType(EmMapType eMapType)
    {
        mapType = eMapType;
    }
    EmMapType GetMapType() const
    {
        return mapType;
    }

    void SetMapUrl(const string& strMapUrl)
    {
        mapUrl = strMapUrl;
    }
    string& GetMapUrl()
    {
        return mapUrl;
    }
    const string& GetMapUrl() const
    {
        return mapUrl;
    }

    void SetParentMapId(const string& strParentMapId)
    {
        parentMapId = strParentMapId;
    }
    string& GetParentMapId()
    {
        return parentMapId;
    }
    const string& GetParentMapId() const
    {
        return parentMapId;
    }

    void SetDescription(const string& strDescription)
    {
        description = strDescription;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }

    void SetBounds(const Bounds& cBounds)
    {
        bounds = cBounds;
    }
    Bounds& GetBounds()
    {
        return bounds;
    }
    const Bounds& GetBounds() const
    {
        return bounds;
    }

    void SetMinScaleLevel(int nMinScaleLevel)
    {
        minScaleLevel = nMinScaleLevel;
    }
    int GetMinScaleLevel() const
    {
        return minScaleLevel;
    }

    void SetMaxScaleLevel(int nMaxScaleLevel)
    {
        maxScaleLevel = nMaxScaleLevel;
    }
    int GetMaxScaleLevel() const
    {
        return maxScaleLevel;
    }

    void SetDefaultScaleLevel(int nDefaultScaleLevel)
    {
        defaultScaleLevel = nDefaultScaleLevel;
    }
    int GetDefaultScaleLevel() const
    {
        return defaultScaleLevel;
    }

    void SetCenterPos(const MapPosition& cCenterPos)
    {
        centerPos = cCenterPos;
    }
    MapPosition& GetCenterPos()
    {
        return centerPos;
    }
    const MapPosition& GetCenterPos() const
    {
        return centerPos;
    }

    void SetVersion(const string& strVersion)
    {
        version = strVersion;
    }
    string& GetVersion()
    {
        return version;
    }
    const string& GetVersion() const
    {
        return version;
    }

    void SetEarthCoord(bool bEarthCoord)
    {
        earthCoord = bEarthCoord;
    }
    bool GetEarthCoord() const
    {
        return earthCoord;
    }

};

typedef vector<MapInfo> MapInfoList;

class MapCondition
{
private:
	string mapId;	             	// ��ͼId
	string mapName;              	// ��ͼ����.

	EmMapType mapType;              	// ��ͼ����.
	bool earthCoord;	             	// ָʾ��ѯ���ؽ���Ƿ���õ�������ϵ
	string parentMapId;              	// ����ͼId. ��մ���ʾû�и���ͼ����ǰ��ͼΪ����ͼ
	CQryOptionField optionField;        // ��ѯ�ֶα�ʶ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
	MapCondition()
	{
		mapType = MAP_JPEG;
		earthCoord = false;
	}
    void SetMapId(const string& strMapId)
    {
        mapId = strMapId;
		optionField.SetOption("mapId");
    }
    bool GetMapId(string& val) const
    {
		if (!optionField.HasOption("mapId"))
			return false;
		val = mapId;
		return true;
		
    }
    
    void SetMapName(const string& strMapName)
    {
        mapName = strMapName;
		optionField.SetOption("mapName");
    }
    bool GetMapName(string& val) const
    {
		if (!optionField.HasOption("mapName"))
			return false;
		val = mapName;
		return true;
    }
    
    void SetMapType(EmMapType eMapType)
    {
        mapType = eMapType;
		optionField.SetOption("mapType");
    }
    bool GetMapType(EmMapType& val) const
    {
		if (!optionField.HasOption("mapType"))
			return false;
		val = mapType;
		return true;
    }

    void SetParentMapId(const string& strParentMapId)
    {
        parentMapId = strParentMapId;
		optionField.SetOption("parentMapId");
    }
    bool GetParentMapId(string& val) const
    {
		if (!optionField.HasOption("parentMapId"))
			return false;
		val = parentMapId;
		return true;
    }
    void SetEarthCoord(bool bEarthCoord)
    {
        earthCoord = bEarthCoord;
    }
    bool GetEarthCoord() const
    {
        return earthCoord;
    }
    void SetOptionField(const CQryOptionField& cOptionField)
    {
        optionField = cOptionField;
    }
    CQryOptionField& GetOptionField()
    {
        return optionField;
    }
    const CQryOptionField& GetOptionField() const
    {
        return optionField;
    }

};


class MapLayer
{
public:
	MapLayer()
	{
		zIndex = 0;
		uuid = CUUID();
	}
private:
	string uuid;	             
	string mapId;	             
	string name;	             	//ͼ������
	int zIndex;		             	//���Ŵ���
	string description;	            //����
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUuid(const string& strUuid)
    {
        uuid = strUuid;
    }
    string& GetUuid()
    {
        return uuid;
    }
    const string& GetUuid() const
    {
        return uuid;
    }

    void SetMapId(const string& strMapId)
    {
        mapId = strMapId;
    }
    string& GetMapId()
    {
        return mapId;
    }
    const string& GetMapId() const
    {
        return mapId;
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

    void SetZIndex(int nZIndex)
    {
        zIndex = nZIndex;
    }
    int GetZIndex() const
    {
        return zIndex;
    }
    void SetDescription(const string& strDescription)
    {
        description = strDescription;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }

};
typedef vector<MapLayer> MapLayerList;


class MapLayerCondition
{
public:
	MapLayerCondition()
	{

	}
private:
	string mapId;
	string layerId;	             		//��ͼͼ��ID,Ԥ���ֶΣ���ǰ���ÿ�
	string name;
	CQryOptionField optionField;        // ��ѯ�ֶα�ʶ
public:
	void SetLayerId(string uuid)
	{
		layerId = uuid;
		optionField.SetOption("layerId");
	}
	bool GetLayerId(string& val) const
	{
		if (!optionField.HasOption("layerId"))
			return false;
		val = layerId;
		return true;
	}
	void SetMapId(string uuid)
	{
		mapId = uuid;
		optionField.SetOption("mapId");
	}
	bool GetMapId(string& val) const
	{
		if (!optionField.HasOption("mapId"))
			return false;
		val = mapId;
		return true;
	}
	void SetName(string strName)
	{
		name = strName;
		optionField.SetOption("name");
	}
	bool GetName(string& val) const
	{
		if (!optionField.HasOption("name"))
			return false;
		val = name;
		return true;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class MapArea
{
public:
	MapArea()
	{
		uuid = CUUID();
		opacity = 0.5;
	}
private:
	string uuid;	             		//�����ʾID
	string ownId;	             		//����������ͼID
	string layerId;	             		//��ͼͼ��ID
	string refMapId;	             	//�������õ��ӵ�ͼID
	string color;		             	//�������ɫ
	string customStyle;	             	//�����Զ�����ʽ
	double opacity;		             	//������ʾ͸����
	vector<MapPosition> trace;	        //�������������
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetUuid(const string& strUuid)
	{
		uuid = strUuid;
	}
	string& GetUuid()
	{
		return uuid;
	}
	const string& GetUuid() const
	{
		return uuid;
	}

	void SetOwnId(const string& strOwnId)
	{
		ownId = strOwnId;
	}
	string& GetOwnId()
	{
		return ownId;
	}
	const string& GetOwnId() const
	{
		return ownId;
	}

	void SetRefMapId(const string& strRefMapId)
	{
		refMapId = strRefMapId;
	}
	string& GetRefMapId()
	{
		return refMapId;
	}
	const string& GetRefMapId() const
	{
		return refMapId;
	}

	void SetColor(string strColor)
	{
		color = strColor;
	}
	string GetColor() const
	{
		return color;
	}

	void SetCustomStyle(const string& strCustomStyle)
	{
		customStyle = strCustomStyle;
	}
	string& GetCustomStyle()
	{
		return customStyle;
	}
	const string& GetCustomStyle() const
	{
		return customStyle;
	}

	void SetOpacity(double nOpacity)
	{
		opacity = nOpacity;
	}
	double GetOpacity() const
	{
		return opacity;
	}

	void SetTrace(const vector<MapPosition>& cTrace)
	{
		trace = cTrace;
	}
	vector<MapPosition>& GetTrace()
	{
		return trace;
	}
	const vector<MapPosition>& GetTrace() const
	{
		return trace;
	}
    void SetLayerId(const string& strLayerId)
    {
        layerId = strLayerId;
    }
    string& GetLayerId()
    {
        return layerId;
    }
    const string& GetLayerId() const
    {
        return layerId;
    }
};
typedef vector<MapArea> MapAreaList;

class MapAreaCondition
{
public:
	MapAreaCondition()
	{

	}
private:
	string areaId;
	string ownId;
	string layerId;	             		//��ͼͼ��ID
	string refMapId;					//�������õĵ�ͼID����մ���ʾû�����õ�ͼ
	CQryOptionField optionField;        // ��ѯ�ֶα�ʶ
public:
	void SetAreaId(string uuid)
	{
		areaId = uuid;
		optionField.SetOption("areaId");
	}
	bool GetAreaId(string& val) const
	{
		if (!optionField.HasOption("areaId"))
			return false;
		val = areaId;
		return true;
	}
	void SetOwnId(string uuid)
	{
		ownId = uuid;
		optionField.SetOption("ownId");
	}
	bool GetOwnId(string& val) const
	{
		if (!optionField.HasOption("ownId"))
			return false;
		val = ownId;
		return true;
	}
	void SetRefMapId(string uuid)
	{
		refMapId = uuid;
		optionField.SetOption("refMapId");
	}
	bool GetRefMapId(string& val) const
	{
		if (!optionField.HasOption("refMapId"))
			return false;
		val = refMapId;
		return true;
	}
	void SetLayerId(string uuid)
	{
		layerId = uuid;
		optionField.SetOption("layerId");
	}
	bool GetLayerId(string& val) const
	{
		if (!optionField.HasOption("layerId"))
			return false;
		val = layerId;
		return true;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};


typedef string EmMapElementType;
#define	MAP_ElEMENT_COMMAN "common"			//����ͼԪ 
#define MAP_ELEMENT_LIVE	"live"			//�ƶ�ͼԪ


typedef string EmDeviceChannelType;
#define MAP_DEVCHANNEL_INVALID    "invalid"	//�Ƿ�ͨ��
#define MAP_DEVCHANNEL_VIDEO	"video"		//��ƵԴͨ��
#define MAP_DEVCHANNEL_ALARMIN "alarmIn"	//�澯����ͨ��
#define MAP_DEVCHANNEL_ENCODE "encode"            // �豸����-����ͨ��
#define MAP_DEVCHANNEL_DECODE  "decode"            // �豸����-����ͨ��
#define MAP_DEVCHANNEL__DECODE_OUTPUTCHANNEL "output"     // �豸����-�������ͨ�� ���繫˾��D04F�豸,�;��ж����ͨ��

class MapElement	
{
public:
	MapElement()
	{
		uuid = CUUID();
		type = MAP_ElEMENT_COMMAN;
		channelType = MAP_DEVCHANNEL_VIDEO;
		deviceChannel = 0;
		earthCoord = true;
		storeTrace = false;
		bHide = false;
	}
private:
	string uuid;					//ͼԪ��uuid
	string name;	             	// ͼԪ����.
	string mapId;	             	// �����ĵ�ͼ��Id.
	string layerId;	             	//����ͼ��ID

	EmMapElementType type;	            // ͼԪ����.

	string deviceId;	             	// ͼԪ��Ӧ�豸ID uuid@domain��ʽ
	int deviceChannel;		            // ͼԪ��Ӧ�豸ͨ��.
	EmDeviceChannelType channelType;	// ͼԪ��Ӧ���豸ͨ��������.

	MapPosition position;	            // ͼԪ���õĵ���λ��.
	bool earthCoord;              		//���õ�����ϵ
	string picUrl;	             		// ͼԪͼƬ·��(����ͼƬ�ļ���?),��һ����Ҫ..

	string description;	             	// ����.
	bool storeTrace;	                     //  ���ͼԪ����ΪMAP_ELEMENT_LIVE�������Ƿ�洢GPS
	bool bHide;							// ͼԪ�Ƿ��CU����

	MapPosition marsPosition;	            // ͼԪ���õĵ���λ��.---���������꣬����/�޸ĵ�ʱ����Ч��

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetId(const string& strId)
    {
        uuid = strId;
    }
    string& GetId()
    {
        return uuid;
    }
    const string& GetId() const
    {
        return uuid;
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

    void SetMapId(const string& strMapId)
    {
        mapId = strMapId;
    }
    string& GetMapId()
    {
        return mapId;
    }
    const string& GetMapId() const
    {
        return mapId;
    }

    void SetLayerId(const string& strGroupId)
    {
        layerId = strGroupId;
    }
    string& GetLayerId()
    {
        return layerId;
    }
    const string& GetLayerId() const
    {
        return layerId;
    }

    void SetType(const EmMapElementType& cType)
    {
        type = cType;
    }
    EmMapElementType& GetType()
    {
        return type;
    }
    const EmMapElementType& GetType() const
    {
        return type;
    }

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

    void SetDeviceChannel(int nDeviceChannel)
    {
        deviceChannel = nDeviceChannel;
    }
    int GetDeviceChannel() const
    {
        return deviceChannel;
    }

    void SetChannelType(const EmDeviceChannelType& cChannelType)
    {
        channelType = cChannelType;
    }
    EmDeviceChannelType& GetChannelType()
    {
        return channelType;
    }
    const EmDeviceChannelType& GetChannelType() const
    {
        return channelType;
    }

    void SetPosition(const MapPosition & cPosition)
    {
        position = cPosition;
    }
     MapPosition& GetPosition()
    {
        return position;
    }
    const MapPosition& GetPosition() const
    {
        return position;
    }

    void SetPicUrl(const string& strPicUrl)
    {
        picUrl = strPicUrl;
    }
    string& GetPicUrl()
    {
        return picUrl;
    }
    const string& GetPicUrl() const
    {
        return picUrl;
    }

    void SetDescription(const string& strDescription)
    {
        description = strDescription;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }

    void SetEarthCoord(bool bEarthCoord)
    {
        earthCoord = bEarthCoord;
    }
    bool GetEarthCoord() const
    {
        return earthCoord;
    }

public:
    void SetStoreTrace(bool bStoreTrace)
    {
        storeTrace = bStoreTrace;
    }
    bool GetStoreTrace() const
    {
        return storeTrace;
    }

	void SetHide(bool tHide)
	{
		bHide = tHide;
	}
	bool IsHide() const
	{
		return bHide;
	}

public:
	void SetMarsPosition(const MapPosition& cMarsPosition)
	{
		marsPosition = cMarsPosition;
	}
	MapPosition& GetMarsPosition()
	{
		return marsPosition;
	}
	const MapPosition& GetMarsPosition() const
	{
		return marsPosition;
	}
};
typedef vector<MapElement> MapElementList;

class MapElementCondition
{
private:
	string userId;					// �û�ID
	bool isSuperAdmin;			// �Ƿ�Ϊ��������Ա

	string id;	             
	string name;	             	// ͼԪ����.
	string mapId;	             	// �����ĵ�ͼ��Id.
	string layerId;	             	//����ͼ��Id
	bool earthCoord;               	 //ָ�����ص�ͼԪ�Ƿ���õ�������ϵ
	EmMapElementType type;	             	// ͼԪ����.

	string deviceId;	             		// ͼԪ��Ӧ�豸Id.
	int deviceChannel;	             		// ͼԪ��Ӧ�豸ͨ��.
	EmDeviceChannelType channelType;        // ͼԪ��Ӧ���豸ͨ��������.

	bool storeTrace;	             	// ��ѯ�洢GPS�켣��ͼԪ(MAP_ELEMENT_LIVE)ʱ��Ч
	CQryOptionField optionField;        // ��ѯ�ֶα�ʶ
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
	MapElementCondition()
	{
		Clear();
	}
	void Clear()
	{
		isSuperAdmin = false;
		type = MAP_ElEMENT_COMMAN;
		earthCoord = false;
		deviceChannel = 0;
		channelType = MAP_DEVCHANNEL_VIDEO;
	}
	void SetUserId(const string& strUserId)
	{
		userId = strUserId;
		optionField.SetOption("userId");
	}
	bool GetUserId(string& strUserId)
	{
		if (!optionField.HasOption("userId"))
			return false;
		strUserId = userId;
		return true;
	}

	void SetIsSuperAdmin(const bool& bIsSuperAdmin)
	{
		isSuperAdmin = bIsSuperAdmin;
		optionField.SetOption("isSuperAdmin");
	}
	bool GetIsSuperAdmin(bool& bIsSuperAdmin)
	{
		if (!optionField.HasOption("isSuperAdmin"))
			return false;
		bIsSuperAdmin = isSuperAdmin;
		return true;
	}

    void SetId(const string& strId)
    {
        id = strId;
		optionField.SetOption("id");
    }
    bool GetId(string& val)
    {
		if (!optionField.HasOption("id"))
			return false;
		val = id;
		return true;
    }
   
    void SetName(const string& strName)
    {
        name = strName;
		optionField.SetOption("name");
    }
    bool GetName(string& val) const
    {
		if (!optionField.HasOption("name"))
			return false;
		val = name;
		return true;
    }
    

    void SetMapId(const string& strMapId)
    {
        mapId = strMapId;
		optionField.SetOption("mapId");
    }
    bool GetMapId(string& val)
    {
		if (!optionField.HasOption("mapId"))
			return false;
		val = mapId;
		return true;
    }
    

    void SetGroupId(const string& strGroupId)
    {
        layerId = strGroupId;
		optionField.SetOption("groupId");
    }
    bool GetGroupId(string& val) const
    {
		if (!optionField.HasOption("groupId"))
			return false;
		val = layerId;
		return true;
    }
   
    void SetType(const EmMapElementType& cType)
    {
        type = cType;
		optionField.SetOption("type");
    }
    bool GetType(EmMapElementType& val) const
    {
		if (!optionField.HasOption("type"))
			return false;
		val = type;
		return true;
    }
    

    void SetDeviceId(const string& strDeviceId)
    {
        deviceId = strDeviceId;
		optionField.SetOption("deviceId");
    }
    bool GetDeviceId(string& val) const
    {
		if (!optionField.HasOption("deviceId"))
			return false;
		val = deviceId;
		return true;
    }
   
    void SetDeviceChannel(int nDeviceChannel)
    {
        deviceChannel = nDeviceChannel;
		optionField.SetOption("deviceChannel");
    }
    bool GetDeviceChannel(int& val) const
    {
		if (!optionField.HasOption("deviceChannel"))
			return false;
		val = deviceChannel;
		return true;
    }

    void SetChannelType(const EmDeviceChannelType& cChannelType)
    {
        channelType = cChannelType;
		optionField.SetOption("channelType");
    }
    bool GetChannelType(EmDeviceChannelType& val) const
    {
		if (!optionField.HasOption("channelType"))
			return false;
		val = channelType;
		return true;
    }
    
    void SetEarthCoord(bool bEarthCoord)
    {
        earthCoord = bEarthCoord;
    }
    bool GetEarthCoord() const
    {
        return earthCoord;
    }

	void SetStoreTrace(bool enable)
	{
		storeTrace = enable;
		optionField.SetOption("storeTrace");
	}
	bool GetStoreTrace(bool &val) const
	{
		if (!optionField.HasOption("storeTrace") && MAP_ELEMENT_LIVE != type)
			return false;
		val = storeTrace;
		return true;
	}

	void SetOptionField(const CQryOptionField& cOptionField)
	{
		optionField = cOptionField;
	}
	CQryOptionField& GetOptionField()
	{
		return optionField;
	}
	const CQryOptionField& GetOptionField() const
	{
		return optionField;
	}
};

class TGpsStatus
{
public:
	TGpsStatus()
	{
		longitude   = 0.0f;
		latitude    = 0.0f;
		speed       = 0.0f;
        altitude    = 0.0f;
        direction   = 0.0f;
	}
protected:
	string timestamp;					//��GpsData��ʱ���	
	double longitude;	                //����
	double latitude;	                //γ��
    double altitude;                    //����
	double speed;		                //�ٶ�
    double direction;		            //�����
    

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
	
	void SetLongitude(double lfLng)
	{
		longitude = lfLng;
	}
	double GetLongitude() const
	{
		return longitude;
	}
	void SetLatitude(double lfLng)
	{
		latitude = lfLng;
	}
	double GetLatitude() const
	{
		return latitude;
	}
	void SetSpeed(double lfSpeed)
	{
		speed = lfSpeed;
	}
	double GetSpeed() const
	{
		return speed;
	}

	void SetAltitude(double lfAlt )
	{
		altitude = lfAlt;
	}
	double GetAltitude() const
	{
		return altitude;
	}
    
    void SetDirection(double lfDir )
	{
        direction = lfDir;
	}
	double GetDirection() const
	{
		return direction;
	}

	string GetTimeStamp() const
	{
		return timestamp;
	}

	void SetTimeStamp(string timeStamp)
	{
		this->timestamp = timeStamp;
	}
};

class TGpsStatusExt : public TGpsStatus
{
private:	
	double raw_longitude;	
	double raw_latitude;
public:
	void SetRawLongitude(double lfLng)
	{
		raw_longitude = lfLng;
	}
	double GetRawLongitude() const
	{
		return raw_longitude;
	}
	void SetRawLatitude(double lfLng)
	{
		raw_latitude = lfLng;
	}
	double GetRawLatitude() const
	{
		return raw_latitude;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TGpsStatus> TGpsPointList;

class TraceCondition
{
private:
	string devUri;
	TTimeRange timeRange;
	CQryOptionField optionField;        // ��ѯ�ֶα�ʶ
	bool earthCoord;	             	// ָʾ��ѯ���ؽ���Ƿ���õ�������ϵ
public:
	TraceCondition()
	{
		earthCoord = false;
	}
	void SetDevUri(string strDevUri)
	{
		devUri = strDevUri;
		optionField.SetOption("devUri");
	}
	bool GetDevUri(string& val) const
	{
		if (!optionField.HasOption("devUri"))
			return false;
		val = devUri;
		return true;
	}
	void SetTimeRange(TTimeRange tTimeRange)
	{
		timeRange = tTimeRange;
		optionField.SetOption("timeRange");
	}
	bool GetTimeRange(TTimeRange& val) const
	{
		if (!optionField.HasOption("timeRange"))
			return false;
		val = timeRange;
		return true;
	}
	void SetEarthCoord(bool bEarthCoord)
	{
		earthCoord = bEarthCoord;
	}
	bool GetEarthCoord() const
	{
		return earthCoord;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};
typedef string DBType;
#define  DBTypeUnknow "Unknow"
#define DBTypeSybase "Sybase"
#define  DBTypeMysql "Mysql"
#define  DBTypeOracle "Orcale"


class CDbCfg
{
public:
	CDbCfg()
	{
		dbType = DBTypeMysql;
		port = 3306;
	}
private:
	DBType dbType;	               
	string ip;	               
	u16 port;	               
	string username;	               
	string password;	               
	string dbname;	                    // �������ݿ����ƣ�Ҳ����ǰ����ʹ�õ����ݿ�
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDbType(const DBType& strDbType)
    {
        dbType = strDbType;
    }
    DBType& GetDbType()
    {
        return dbType;
    }
    const DBType& GetDbType() const
    {
        return dbType;
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

    void SetPort(const u16& cPort)
    {
        port = cPort;
    }
    u16& GetPort()
    {
        return port;
    }
    const u16& GetPort() const
    {
        return port;
    }

    void SetUsername(const string& strUsername)
    {
        username = strUsername;
    }
    string& GetUsername()
    {
        return username;
    }
    const string& GetUsername() const
    {
        return username;
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

    void SetDbname(const string& strDbname)
    {
        dbname = strDbname;
    }
    string& GetDbname()
    {
        return dbname;
    }
    const string& GetDbname() const
    {
        return dbname;
    }

};
//�澯�������ð汾��Ϣ
class CGpsStoreCfg
{
public:
	CGpsStoreCfg()
	{
		maxGpsDayCount = 30;
		gpsSamplingCoff = 30;
		storeEnable = true;
	}
private:
	string gpsDataDir;		               
	int maxGpsDayCount;		               
	int gpsSamplingCoff;	               
	bool storeEnable;		               
	CDbCfg gpsDbCfg;		               
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGpsDataDir(const string& strGpsDataDir)
    {
        gpsDataDir = strGpsDataDir;
    }
    string& GetGpsDataDir()
    {
        return gpsDataDir;
    }
    const string& GetGpsDataDir() const
    {
        return gpsDataDir;
    }

    void SetMaxGpsDayCount(int nMaxGpsDayCount)
    {
        maxGpsDayCount = nMaxGpsDayCount;
    }
    int GetMaxGpsDayCount() const
    {
        return maxGpsDayCount;
    }

    void SetGpsSamplingCoff(int nGpsSamplingCoff)
    {
        gpsSamplingCoff = nGpsSamplingCoff;
    }
    int GetGpsSamplingCoff() const
    {
        return gpsSamplingCoff;
    }

    void SetStoreEnable(bool bStoreEnable)
    {
        storeEnable = bStoreEnable;
    }
    bool GetStoreEnable() const
    {
        return storeEnable;
    }

    void SetGpsDbCfg(const CDbCfg& cGpsDbCfg)
    {
        gpsDbCfg = cGpsDbCfg;
    }
    CDbCfg& GetGpsDbCfg()
    {
        return gpsDbCfg;
    }
    const CDbCfg& GetGpsDbCfg() const
    {
        return gpsDbCfg;
    }

};

//�澯�������ð汾��Ϣ
struct TMpsDataVersion
{
public:
	TMpsDataVersion()
	{
		elementVersion = 0;
	}

	int elementVersion;                //ͼԪ���ݰ汾��   2014.6.19 ����չelementVersion��ʾ��ͼ���ݣ����µ�ͼ������ͼԪ����Ķ��汾
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef string TMpsStoreType;
#define MPS_STORE_TYPE_GPS            ("Gps")
#define MPS_STORE_TYPE_OFFLINE_MAP    ("OfflineMap")

#endif
















