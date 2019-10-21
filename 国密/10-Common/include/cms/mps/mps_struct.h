/*****************************************************************************
   模块名      : mps_struct
   文件名      : mps_struct.h
   相关文件    : 
   文件实现功能: mps_struct.h 定义电子地图外设信令接口用到的结构体
   作者        : zhouchanghong
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/7/02   1.0         周昌鸿        创建
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
	EmMapType name;	             	//地图类型名称
	string version;	             	//地图服务版本号
	string accessUrl;	            //访问服务地址
	               							
	MODULE_ATTRI_MAP moduleAttrs;		   //服务支持的模块，如gps历史轨迹模块启用则含有“gps”字符串,如果已配置内网google地图，则包含“tiles”
										   //key为模块名，value为模块对应的属性值，如key为tiles时，value存放离线地图的访问地址。
										   //对应模块和属性字段含义表：
										   //    key  |  value    |   意义
										   //    gps  |  local/IP |   local表示本地数据库，IP地址则为远程数据库地址
										   //    tiles | url      |   url访问地址，可以为相对目录和绝对地址
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


class MapPosition		//地图坐标点，在google地图中表现为经纬度，在Jpeg地图中表现为像素点
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
	double	longitude;	             	// 经度.
	double	latitude;	               // 纬度.

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

class Bounds		//边界
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
	string mapId;	             	// 地图Id
	string mapName;	             	// 地图名称.

	EmMapType mapType;              // 地图类型.

	string mapUrl;	             	// 地图图片文件url. ( 由地图插件定义格式 ) jpeg地图要先上传图片.

	string parentMapId;             // 父地图Id.

	string description;	            // 描述

	// 地图边界.
	Bounds bounds;	             

	// 缩放范围.
	int minScaleLevel;	             
	int maxScaleLevel;	             
	int defaultScaleLevel;	        // 默认的缩放级别

	MapPosition centerPos;	             
	bool earthCoord;              	//采用的坐标系
	string version;	             	// 地图版本.?
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
	string mapId;	             	// 地图Id
	string mapName;              	// 地图名称.

	EmMapType mapType;              	// 地图类型.
	bool earthCoord;	             	// 指示查询返回结果是否采用地球坐标系
	string parentMapId;              	// 父地图Id. 填空串表示没有父地图，当前地图为根地图
	CQryOptionField optionField;        // 查询字段标识
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
	string name;	             	//图层名称
	int zIndex;		             	//叠放次序
	string description;	            //描述
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
	string layerId;	             		//地图图层ID,预留字段，当前请置空
	string name;
	CQryOptionField optionField;        // 查询字段标识
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
	string uuid;	             		//区域标示ID
	string ownId;	             		//区域所属地图ID
	string layerId;	             		//地图图层ID
	string refMapId;	             	//区域引用的子地图ID
	string color;		             	//区域的颜色
	string customStyle;	             	//区域自定义样式
	double opacity;		             	//区域显示透明度
	vector<MapPosition> trace;	        //区域各顶点坐标
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
	string layerId;	             		//地图图层ID
	string refMapId;					//区域引用的地图ID，填空串标示没有引用地图
	CQryOptionField optionField;        // 查询字段标识
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
#define	MAP_ElEMENT_COMMAN "common"			//常规图元 
#define MAP_ELEMENT_LIVE	"live"			//移动图元


typedef string EmDeviceChannelType;
#define MAP_DEVCHANNEL_INVALID    "invalid"	//非法通道
#define MAP_DEVCHANNEL_VIDEO	"video"		//视频源通道
#define MAP_DEVCHANNEL_ALARMIN "alarmIn"	//告警输入通道
#define MAP_DEVCHANNEL_ENCODE "encode"            // 设备能力-编码通道
#define MAP_DEVCHANNEL_DECODE  "decode"            // 设备能力-解码通道
#define MAP_DEVCHANNEL__DECODE_OUTPUTCHANNEL "output"     // 设备能力-解码输出通道 比如公司的D04F设备,就具有多输出通道

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
	string uuid;					//图元的uuid
	string name;	             	// 图元名称.
	string mapId;	             	// 所属的地图的Id.
	string layerId;	             	//所属图层ID

	EmMapElementType type;	            // 图元类型.

	string deviceId;	             	// 图元对应设备ID uuid@domain形式
	int deviceChannel;		            // 图元对应设备通道.
	EmDeviceChannelType channelType;	// 图元对应的设备通道的类型.

	MapPosition position;	            // 图元布置的地理位置.
	bool earthCoord;              		//采用的坐标系
	string picUrl;	             		// 图元图片路径(或者图片文件名?),不一定需要..

	string description;	             	// 描述.
	bool storeTrace;	                     //  针对图元类型为MAP_ELEMENT_LIVE，设置是否存储GPS
	bool bHide;							// 图元是否对CU隐藏

	MapPosition marsPosition;	            // 图元布置的地理位置.---（火星坐标，创建/修改的时候无效）

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
	string userId;					// 用户ID
	bool isSuperAdmin;			// 是否为超级管理员

	string id;	             
	string name;	             	// 图元名称.
	string mapId;	             	// 所属的地图的Id.
	string layerId;	             	//所属图层Id
	bool earthCoord;               	 //指定返回的图元是否采用地球坐标系
	EmMapElementType type;	             	// 图元类型.

	string deviceId;	             		// 图元对应设备Id.
	int deviceChannel;	             		// 图元对应设备通道.
	EmDeviceChannelType channelType;        // 图元对应的设备通道的类型.

	bool storeTrace;	             	// 查询存储GPS轨迹的图元(MAP_ELEMENT_LIVE)时有效
	CQryOptionField optionField;        // 查询字段标识
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
	string timestamp;					//该GpsData的时间戳	
	double longitude;	                //经度
	double latitude;	                //纬度
    double altitude;                    //海拔
	double speed;		                //速度
    double direction;		            //方向角
    

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
	CQryOptionField optionField;        // 查询字段标识
	bool earthCoord;	             	// 指示查询返回结果是否采用地球坐标系
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
	string dbname;	                    // 工作数据库名称，也即当前正在使用的数据库
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
//告警联动配置版本信息
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

//告警联动配置版本信息
struct TMpsDataVersion
{
public:
	TMpsDataVersion()
	{
		elementVersion = 0;
	}

	int elementVersion;                //图元数据版本号   2014.6.19 新扩展elementVersion表示地图数据，更新地图，区域，图元都会改动版本
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
















