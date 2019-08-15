/*****************************************************************************
   模块名      : mps_proto
   文件名      : mps_proto.h
   相关文件    : 
   文件实现功能: mps_proto.h 定义电子地图外设信令接口
   作者        : zhouchanghong
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/7/02   1.0         周昌鸿        创建
******************************************************************************/

#ifndef __MPS_PROTO_H__
#define __MPS_PROTO_H__

#include "cms/cms_proto.h"
#include "mps_event.h"
#include "mps_struct.h"
#include "cbb/osp/osp.h"
#include "cms/cmu/cmu_struct.h"

//////////////////////////////////////////////////////////////////////////

//class CGetServerInfoReq : public CEventReq
//{
//public:
//	CGetServerInfoReq()
//	{
//		event = OspExtEventDesc(MPS_GET_SERVERINFO_REQ);
//	}
//public:
//	const std::string ToXml()const;
//	void FromXml(const std::string& xml_string);
//	void ToXml(TiXmlDocument& xmlDoc)const;
//	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//	void FromXml(const TiXmlNode& xmlNode);
//
//};
//
//class CGetServerInfoRsp : public CEventRsp
//{
//public:
//	CGetServerInfoRsp()
//	{
//		event = OspExtEventDesc(MPS_GET_SERVERINFO_RSP);
//	}
//            
//public:
//	const std::string ToXml()const;
//	void FromXml(const std::string& xml_string);
//	void ToXml(TiXmlDocument& xmlDoc)const;
//	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//	void FromXml(const TiXmlNode& xmlNode);
//
//};


//获取电子地图外设支持的地图类型
class CGetSupportMapTypesReq : public CEventReq
{
public:
	CGetSupportMapTypesReq()
	{
		event = OspExtEventDesc(MPS_GET_SUPPORT_MAP_TYPES_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}
private:
	TNatInfo clientNatInfo;		             	//客户端Nat信息
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
           

public:
    void SetClientNatInfo(const TNatInfo& cClientNatInfo)
    {
        clientNatInfo = cClientNatInfo;
    }
    TNatInfo& GetClientNatInfo()
    {
        return clientNatInfo;
    }
    const TNatInfo& GetClientNatInfo() const
    {
        return clientNatInfo;
    }

};


class CGetSupportMapTypesRsp : public CEventRsp
{
public:
	CGetSupportMapTypesRsp()
	{
		event = OspExtEventDesc(MPS_GET_SUPPORT_MAP_TYPES_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	vector<MapTypeInfo> supportTypeList;      
public:
    void SetSupportTypeList(const vector<MapTypeInfo>& cSupportTypeList)
    {
        supportTypeList = cSupportTypeList;
    }
    vector<MapTypeInfo>& GetSupportTypeList()
    {
        return supportTypeList;
    }
    const vector<MapTypeInfo>& GetSupportTypeList() const
    {
        return supportTypeList;
    }

};

//创建电子地图，目前只用于Jpeg地图类型
class CMapCreateReq : public CEventReq
{
public:
    CMapCreateReq()
    {
        event = OspExtEventDesc(MPS_MAP_CREATE_REQ);
    }

    void Clear()
    {
        CEventReq::Clear();
    }

public:
	

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
	MapInfo mapInfo;               
public:
    void SetMapInfo(const MapInfo& cMapInfo)
    {
        mapInfo = cMapInfo;
    }
    MapInfo& GetMapInfo()
    {
        return mapInfo;
    }
    const MapInfo& GetMapInfo() const
    {
        return mapInfo;
    }

};

class CMapCreateRsp : public CEventRsp
{
public:
	CMapCreateRsp()
	{
		event = OspExtEventDesc(MPS_MAP_CREATE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString mapId;                
public:
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

};

class CMapModReq : public CEventReq
{
public:
	CMapModReq()
	{
		event = OspExtEventDesc(MPS_MAP_MOD_REQ);
	}
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapInfo mapInfo;               
public:
    void SetMapInfo(const MapInfo& cMapInfo)
    {
        mapInfo = cMapInfo;
    }
    MapInfo& GetMapInfo()
    {
        return mapInfo;
    }
    const MapInfo& GetMapInfo() const
    {
        return mapInfo;
    }

};

class CMapModRsp : public CEventRsp
{
public:
	CMapModRsp()
	{
		event = OspExtEventDesc(MPS_MAP_MOD_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};


class CMapDelReq : public CEventReq
{
public:
	CMapDelReq()
	{
		event = OspExtEventDesc(MPS_MAP_DEL_REQ);
	}
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string mapId;               
public:
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

};

class CMapDelRsp : public CEventRsp
{
public:
	CMapDelRsp()
	{
		event = OspExtEventDesc(MPS_MAP_DEL_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CMapQryReq : public CQueryReqBase
{
public:
	CMapQryReq()
	{
		event = OspExtEventDesc(MPS_MAP_QRY_REQ);
	}
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapCondition condition;               
public:
    void SetCondition(const MapCondition& cCondition)
    {
        condition = cCondition;
    }
    MapCondition& GetCondition()
    {
        return condition;
    }
    const MapCondition& GetCondition() const
    {
        return condition;
    }

};

class CMapQryRsp : public CQueryRspBase
{
public:
	CMapQryRsp()
	{
		event = OspExtEventDesc(MPS_MAP_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CMapQryNtfReq : public CQueryNtfReqBase
{
public:
	CMapQryNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_QRY_NTF_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapInfoList mapInfos;               
public:
    void SetMapInfos(MapInfoList& cMapInfos)
    {
        mapInfos = cMapInfos;
    }
    MapInfoList& GetMapInfos()
    {
        return mapInfos;
    }
    const MapInfoList& GetMapInfos() const
    {
        return mapInfos;
    }

};

class CMapQryNtfRsp : public CEventRsp
{
public:
	CMapQryNtfRsp()
	{
		event = OspExtEventDesc(MPS_MAP_QRY_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMapLayerCreateReq : public CEventReq
{
public:
	CMapLayerCreateReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_CREATE_REQ);
	}
private:
	MapLayer layer;		             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetLayer(const MapLayer& cLayer)
    {
        layer = cLayer;
    }
    MapLayer& GetLayer()
    {
        return layer;
    }
    const MapLayer& GetLayer() const
    {
        return layer;
    }

};

class CMapLayerCreateRsp : public CEventRsp
{
public:
	CMapLayerCreateRsp()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_CREATE_RSP);
	}
private:
	UUIDString layerId;	             	// 图层id值
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetLayerId(const UUIDString& strLayerId)
    {
        layerId = strLayerId;
    }
    UUIDString& GetLayerId()
    {
        return layerId;
    }
    const UUIDString& GetLayerId() const
    {
        return layerId;
    }

};

class CMapLayerModReq : public CEventReq
{
public:
	CMapLayerModReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_MOD_REQ);
	}
private:
	MapLayer layer;	             	//待修改的地图区域
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetLayer(const MapLayer& cLayer)
	{
		layer = cLayer;
	}
	MapLayer& GetLayer()
	{
		return layer;
	}
	const MapLayer& GetLayer() const
	{
		return layer;
	}

};

class CMapLayerModRsp : public CEventRsp
{
public:
	CMapLayerModRsp()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_MOD_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


//class CMapLayerDelReq : public CEventReq
//{
//public:
//	CMapLayerDelReq()
//	{
//		event = OspExtEventDesc(MPS_MAP_LAYER_DEL_REQ);
//	}
//private:
//	string layerId;	             	//待删除的地图区域Id
//public:
//	const std::string ToXml()const;
//	void FromXml(const std::string& xml_string);
//	void ToXml(TiXmlDocument& xmlDoc)const;
//	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//	void FromXml(const TiXmlNode& xmlNode);
//public:
//	void SetLayerId(const string& strLayerId)
//	{
//		layerId = strLayerId;
//	}
//	string& GetLayerId()
//	{
//		return layerId;
//	}
//	const string& GetLayerId() const
//	{
//		return layerId;
//	}
//
//};
//
//class CMapLayerDelRsp : public CEventRsp
//{
//public:
//	CMapLayerDelRsp()
//	{
//		event = OspExtEventDesc(MPS_MAP_LAYER_DEL_RSP);
//	}
//public:
//	const std::string ToXml()const;
//	void FromXml(const std::string& xml_string);
//	void ToXml(TiXmlDocument& xmlDoc)const;
//	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//	void FromXml(const TiXmlNode& xmlNode);
//};

class CMapLayerQryReq : public CQueryReqBase
{
public:
	CMapLayerQryReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_QRY_REQ);
	}
private:
	MapLayerCondition condition;	             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
	void SetCondition(const MapLayerCondition& cCondition)
	{
		condition = cCondition;
	}
	MapLayerCondition& GetCondition()
	{
		return condition;
	}
	const MapLayerCondition& GetCondition() const
	{
		return condition;
	}

};

class CMapLayerQryRsp : public CQueryRspBase
{
public:
	CMapLayerQryRsp()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CMapLayerQryNtfReq : public CQueryNtfReqBase
{
public:
	CMapLayerQryNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_QRY_NTF_REQ);
	}
private:
	vector<MapLayer> results;	             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetResults(const vector<MapLayer>& cResults)
	{
		results = cResults;
	}
	vector<MapLayer>& GetResults()
	{
		return results;
	}
	const vector<MapLayer>& GetResults() const
	{
		return results;
	}

};

class CMapLayerQryNtfRsp : public CEventRsp
{
public:
	CMapLayerQryNtfRsp()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_QRY_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CMapAreaCreateReq : public CEventReq
{
public:
	CMapAreaCreateReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_CREATE_REQ);
	}
private:
	MapArea area;	              //待创建的子地图代表区域
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetArea(const MapArea& cArea)
    {
        area = cArea;
    }
    MapArea& GetArea()
    {
        return area;
    }
    const MapArea& GetArea() const
    {
        return area;
    }

};

class CMapAreaCreateRsp : public CEventRsp
{
public:
	CMapAreaCreateRsp()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_CREATE_RSP);
	}
private:
	UUIDString areaId;	             	//区域ID
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetAreaId(const UUIDString& strAreaId)
    {
        areaId = strAreaId;
    }
    UUIDString& GetAreaId()
    {
        return areaId;
    }
    const UUIDString& GetAreaId() const
    {
        return areaId;
    }

};

class CMapAreaModReq : public CEventReq
{
public:
	CMapAreaModReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_MOD_REQ);
	}
private:
	MapArea area;	             	//待修改的地图区域
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetArea(const MapArea& cArea)
    {
        area = cArea;
    }
    MapArea& GetArea()
    {
        return area;
    }
    const MapArea& GetArea() const
    {
        return area;
    }

};

class CMapAreaModRsp : public CEventRsp
{
public:
	CMapAreaModRsp()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_MOD_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMapAreaDelReq : public CEventReq
{
public:
	CMapAreaDelReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_DEL_REQ);
	}
private:
	string areaId;	             	//待删除的地图区域Id
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetAreaId(const string& cAreaId)
    {
        areaId = cAreaId;
    }
    string& GetAreaId()
    {
        return areaId;
    }
    const string& GetAreaId() const
    {
        return areaId;
    }

};

class CMapAreaDelRsp : public CEventRsp
{
public:
	CMapAreaDelRsp()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_DEL_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMapAreaQryReq : public CQueryReqBase
{
public:
	CMapAreaQryReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_QRY_REQ);
	}
private:
	MapAreaCondition condition;	             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
    void SetCondition(const MapAreaCondition& cCondition)
    {
        condition = cCondition;
    }
    MapAreaCondition& GetCondition()
    {
        return condition;
    }
    const MapAreaCondition& GetCondition() const
    {
        return condition;
    }

};

class CMapAreaQryRsp : public CQueryRspBase
{
public:
	CMapAreaQryRsp()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CMapAreaQryNtfReq : public CQueryNtfReqBase
{
public:
	CMapAreaQryNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_QRY_NTF_REQ);
	}
private:
	vector<MapArea> results;	             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetResults(const vector<MapArea>& cResults)
    {
        results = cResults;
    }
    vector<MapArea>& GetResults()
    {
        return results;
    }
    const vector<MapArea>& GetResults() const
    {
        return results;
    }

};

class CMapAreaQryNtfRsp : public CEventRsp
{
public:
	CMapAreaQryNtfRsp()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_QRY_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMapElementCreateReq : public CEventReq
{
public:
	CMapElementCreateReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_CREATE_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElement mapElement;                
public:
    void SetMapElement(const MapElement& cMapElement)
    {
        mapElement = cMapElement;
    }
    MapElement& GetMapElement()
    {
        return mapElement;
    }
    const MapElement& GetMapElement() const
    {
        return mapElement;
    }

};

class CMapElementCreateRsp : public CEventRsp
{
public:
	CMapElementCreateRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_CREATE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string elementId;      //图元ID          
public:
    void SetElementId(const string& strElementId)
    {
        elementId = strElementId;
    }
    string& GetElementId()
    {
        return elementId;
    }
    const string& GetElementId() const
    {
        return elementId;
    }

};

class CMapElementModReq : public CEventReq
{
public:
	CMapElementModReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_MOD_REQ);
	}
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElement mapElement;               
public:
    void SetMapElement(const MapElement& cMapElement)
    {
        mapElement = cMapElement;
    }
    MapElement& GetMapElement()
    {
        return mapElement;
    }
    const MapElement& GetMapElement() const
    {
        return mapElement;
    }

};

class CMapElementModRsp : public CEventRsp
{
public:
	CMapElementModRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_MOD_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};


class CMapElementDelReq : public CEventReq
{
public:
	CMapElementDelReq()
	{
		Clear();
		event = OspExtEventDesc(MPS_MAP_ELEMENT_DEL_REQ);
	}

	void Clear()
	{
		bHide = false; // 默认是删除，不是隐藏图元
		elementId.clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string elementId;
	bool bHide; // 这个信令只有客户端会发，mps自行判断，如果图元为google图元，则隐藏，否则就删除。
public:
    void SetElementId(const string& strElementId)
    {
        elementId = strElementId;
    }
    string& GetElementId()
    {
        return elementId;
    }
    const string& GetElementId() const
    {
        return elementId;
    }

	void SetHide(bool tbHide)
	{
		bHide = tbHide;
	}
	bool IsHide() const
	{
		return bHide;
	}

};

class CMapElementDelRsp : public CEventRsp
{
public:
	CMapElementDelRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_DEL_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CMapElementQryReq : public CQueryReqBase
{
public:
	CMapElementQryReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_QRY_REQ);
	}
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElementCondition condition;               
public:
    void SetCondition(const MapElementCondition& cCondition)
    {
        condition = cCondition;
    }
    MapElementCondition& GetCondition()
    {
        return condition;
    }
    const MapElementCondition& GetCondition() const
    {
        return condition;
    }

};

class CMapElementQryRsp : public CQueryRspBase
{
public:
	CMapElementQryRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CMapElementQryNtfReq : public CQueryNtfReqBase
{
public:
	CMapElementQryNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_QRY_NTF_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElementList mapElements;               
public:
    void SetMapElements(const MapElementList& cMapElements)
    {
        mapElements = cMapElements;
    }
    MapElementList& GetMapElements()
    {
        return mapElements;
    }
    const MapElementList& GetMapElements() const
    {
        return mapElements;
    }

};

class CMapElementQryNtfRsp : public CEventRsp
{
public:
	CMapElementQryNtfRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_QRY_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMapElementSearchReq : public CQueryReqBase
{
public:
	CMapElementSearchReq()
	{
		Clear();
		event = OspExtEventDesc(MPS_MAP_ELEMENT_SEARCH_REQ);
	}
public:
	void Clear()
	{
		CQueryReqBase::Clear();

		userId.clear();
		isSuperAdmin = false;

		mapId.clear();

		searchPosEnable = false;
		upperLeft.SetLongitude(MPS_DEF_FLOAT_ZERO);
		upperLeft.SetLatitude(MPS_DEF_FLOAT_ZERO);
		bottomRight.SetLongitude(MPS_DEF_FLOAT_ZERO);
		bottomRight.SetLatitude(MPS_DEF_FLOAT_ZERO);
		
		searchKeyEnabled = false;
		searchKey.clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString userId;
	bool isSuperAdmin;

	UUIDString mapId;

	MapPosition upperLeft;
	MapPosition bottomRight;
	bool searchPosEnable;

	string searchKey;
	bool searchKeyEnabled;
public:
    void SetUserId(const UUIDString& strUserId)
    {
        userId = strUserId;
    }
    UUIDString& GetUserId()
    {
        return userId;
    }
    const UUIDString& GetUserId() const
    {
        return userId;
    }

    void SetMapId(const UUIDString& strMapId)
    {
        mapId = strMapId;
    }
    UUIDString& GetMapId()
    {
        return mapId;
    }
    const UUIDString& GetMapId() const
    {
        return mapId;
    }

    void SetUpperLeft(const MapPosition& cUpperLeft)
    {
        upperLeft = cUpperLeft;
    }
    MapPosition& GetUpperLeft()
    {
        return upperLeft;
    }
    const MapPosition& GetUpperLeft() const
    {
        return upperLeft;
    }

    void SetBottomRight(const MapPosition& cBottomRight)
    {
        bottomRight = cBottomRight;
    }
    MapPosition& GetBottomRight()
    {
        return bottomRight;
    }
    const MapPosition& GetBottomRight() const
    {
        return bottomRight;
    }

    void SetSearchPosEnable(bool bSearchPosEnable)
    {
        searchPosEnable = bSearchPosEnable;
    }
    bool GetSearchPosEnable() const
    {
        return searchPosEnable;
    }

    void SetSearchKey(const string& strSearchKey)
    {
        searchKey = strSearchKey;
    }
    string& GetSearchKey()
    {
        return searchKey;
    }
    const string& GetSearchKey() const
    {
        return searchKey;
    }

    void SetSearchKeyEnabled(bool bSearchKeyEnabled)
    {
        searchKeyEnabled = bSearchKeyEnabled;
    }
    bool GetSearchKeyEnabled() const
    {
        return searchKeyEnabled;
    }

public:
    void SetIsSuperAdmin(bool bIsSuperAdmin)
    {
        isSuperAdmin = bIsSuperAdmin;
    }
    bool GetIsSuperAdmin() const
    {
        return isSuperAdmin;
    }
};

class CMapElementSearchRsp : public CQueryRspBase
{
public:
	CMapElementSearchRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_SEARCH_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CMapElementSearchNtfReq : public CQueryNtfReqBase
{
public:
	CMapElementSearchNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_SEARCH_NTF_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElementList mapElements;
public:
    void SetMapElements(const MapElementList& cMapElements)
    {
        mapElements = cMapElements;
    }
    MapElementList& GetMapElements()
    {
        return mapElements;
    }
    const MapElementList& GetMapElements() const
    {
        return mapElements;
    }
};

class CMapElementSearchNtfRsp : public CEventRsp
{
public:
	CMapElementSearchNtfRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_SEARCH_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPosCorrectMarsToEarthReq : public CEventReq
{
public:
	CPosCorrectMarsToEarthReq()
	{
		event = OspExtEventDesc(MPS_POS_CORRECT_MARS_TO_EARTH_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	double mars_lng;	             	//火星坐标经度
	double mars_lat;	             	//火星坐标维度
public:
    void SetMars_lng(double nMars_lng)
    {
        mars_lng = nMars_lng;
    }
    double GetMars_lng() const
    {
        return mars_lng;
    }

    void SetMars_lat(double nMars_lat)
    {
        mars_lat = nMars_lat;
    }
    double GetMars_lat() const
    {
        return mars_lat;
    }

};

class CPosCorrectMarsToEarthRsp : public CEventRsp
{
public:
	CPosCorrectMarsToEarthRsp()
	{
		event = OspExtEventDesc(MPS_POS_CORRECT_MARS_TO_EARTH_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	double earth_lng;	             	//地球坐标经度
	double earth_lat;	             	//地球坐标维度

public:
    void SetEarth_lng(double nEarth_lng)
    {
        earth_lng = nEarth_lng;
    }
    double GetEarth_lng() const
    {
        return earth_lng;
    }

    void SetEarth_lat(double nEarth_lat)
    {
        earth_lat = nEarth_lat;
    }
    double GetEarth_lat() const
    {
        return earth_lat;
    }

};


class CPosCorrectEarthToMarsReq : public CEventReq
{
public:
	CPosCorrectEarthToMarsReq()
	{
		event = OspExtEventDesc(MPS_POS_CORRECT_EARTH_TO_MARS_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	double earth_lng;	             	//地球坐标经度
	double earth_lat;	             	//地球坐标维度

public:
    void SetEarth_lng(double nEarth_lng)
    {
        earth_lng = nEarth_lng;
    }
    double GetEarth_lng() const
    {
        return earth_lng;
    }

    void SetEarth_lat(double nEarth_lat)
    {
        earth_lat = nEarth_lat;
    }
    double GetEarth_lat() const
    {
        return earth_lat;
    }

};

class CPosCorrectEarthToMarsRsp : public CEventRsp
{
public:
	CPosCorrectEarthToMarsRsp()
	{
		event = OspExtEventDesc(MPS_POS_CORRECT_EARTH_TO_MARS_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	double mars_lng;	             	//火星坐标经度
	double mars_lat;	             	//火星坐标维度

public:
    void SetMars_lng(double nMars_lng)
    {
        mars_lng = nMars_lng;
    }
    double GetMars_lng() const
    {
        return mars_lng;
    }

    void SetMars_lat(double nMars_lat)
    {
        mars_lat = nMars_lat;
    }
    double GetMars_lat() const
    {
        return mars_lat;
    }

};

class CElementImageQryReq : public CQueryReqBase
{
public:
	CElementImageQryReq()
	{
		event = OspExtEventDesc(MPS_ELEMENT_IMAGE_QRY_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CElementImageQryRsp : public CQueryRspBase
{
public:
	CElementImageQryRsp()
	{
		event = OspExtEventDesc(MPS_ELEMENT_IMAGE_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CElementImageQryNtfReq : public CQueryNtfReqBase
{
public:
	CElementImageQryNtfReq()
	{
		event = OspExtEventDesc(MPS_ELEMENT_IMAGE_QRY_NTF_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetImageList(const vector<string>& imageList)
	{
		imageUrlList = imageList;
	}
	vector<string>& GetImageList()
	{
		return imageUrlList;
	}
	const vector<string>& GetImageList() const
	{
		return imageUrlList;
	}
private:
	vector<string> imageUrlList;

};

typedef CNtfRsp CElementImageQryNtfRsp;


class CMapUploadReq :  public CEventReq
{
public:
	CMapUploadReq()
	{
		event = OspExtEventDesc(MPS_MAP_UPLOAD_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	EmMapType mapType;	             	//请求上传地图类型
	string parentMapId;	             	//服务端地图采用层级存储，子地图存放在父地图的文件夹中
										//传递父地图ID，用于定位子地图存放目录
	TNatInfo clientNatInfo;		             	//客户端Nat信息
public:
    void SetMapType(const EmMapType& strMapType)
    {
        mapType = strMapType;
    }
    EmMapType& GetMapType()
    {
        return mapType;
    }
    const EmMapType& GetMapType() const
    {
        return mapType;
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

public:
    void SetClientNatInfo(const TNatInfo& cClientNatInfo)
    {
        clientNatInfo = cClientNatInfo;
    }
    TNatInfo& GetClientNatInfo()
    {
        return clientNatInfo;
    }
    const TNatInfo& GetClientNatInfo() const
    {
        return clientNatInfo;
    }

};

class CMapUploadRsp :  public CEventRsp
{
public:
	CMapUploadRsp()
	{
		event = OspExtEventDesc(MPS_MAP_UPLOAD_RSP);
	}
	void Clear()
	{
		CEventRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string uploadUrl;	             	//上传地址
public:
    void SetUploadUrl(const string& strUploadUrl)
    {
        uploadUrl = strUploadUrl;
    }
    string& GetUploadUrl()
    {
        return uploadUrl;
    }
    const string& GetUploadUrl() const
    {
        return uploadUrl;
    }

};


class CElementIconUploadReq :  public CEventReq
{
public:
	CElementIconUploadReq()
	{
		event = OspExtEventDesc(MPS_ELEMENT_ICON_UPLOAD_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string iconName;	                    //待上传图元名称
	TNatInfo clientNatInfo;		             	//客户端Nat信息
public:
    void SetIconName(const string& strIconName)
    {
        iconName = strIconName;
    }
    string& GetIconName()
    {
        return iconName;
    }
    const string& GetIconName() const
    {
        return iconName;
    }

public:
    void SetClientNatInfo(const TNatInfo& cClientNatInfo)
    {
        clientNatInfo = cClientNatInfo;
    }
    TNatInfo& GetClientNatInfo()
    {
        return clientNatInfo;
    }
    const TNatInfo& GetClientNatInfo() const
    {
        return clientNatInfo;
    }

};

class CElementIconUploadRsp :  public CEventRsp
{
public:
	CElementIconUploadRsp()
	{
		event = OspExtEventDesc(MPS_ELEMENT_ICON_UPLOAD_RSP);
	}
	void Clear()
	{
		CEventRsp::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string uploadUrl;	             	//上传地址
public:
    void SetUploadUrl(const string& strUploadUrl)
    {
        uploadUrl = strUploadUrl;
    }
    string& GetUploadUrl()
    {
        return uploadUrl;
    }
    const string& GetUploadUrl() const
    {
        return uploadUrl;
    }

};

class CMapAddNtfReq :public CEventReq
{
public:
	CMapAddNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ADD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapInfo mapInfo;	             
public:
    void SetMapInfo(const MapInfo& cMapInfo)
    {
        mapInfo = cMapInfo;
    }
    MapInfo& GetMapInfo()
    {
        return mapInfo;
    }
    const MapInfo& GetMapInfo() const
    {
        return mapInfo;
    }

};
class CMapModNtfReq :public CEventReq
{
public:
	CMapModNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_MOD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapInfo mapInfo;	             
public:
    void SetMapInfo(const MapInfo& cMapInfo)
    {
        mapInfo = cMapInfo;
    }
    MapInfo& GetMapInfo()
    {
        return mapInfo;
    }
    const MapInfo& GetMapInfo() const
    {
        return mapInfo;
    }

};
class CMapDelNtfReq :public CEventReq
{
public:
	CMapDelNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_DEL_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString mapId;	             
public:
    void SetMapId(const UUIDString& strMapId)
    {
        mapId = strMapId;
    }
    UUIDString& GetMapId()
    {
        return mapId;
    }
    const UUIDString& GetMapId() const
    {
        return mapId;
    }

};
class CMapLayerAddNtfReq :public CEventReq
{
public:
	CMapLayerAddNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_ADD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapLayer maplaver;	             
public:
    void SetMaplaver(const MapLayer& cMaplaver)
    {
        maplaver = cMaplaver;
    }
    MapLayer& GetMaplaver()
    {
        return maplaver;
    }
    const MapLayer& GetMaplaver() const
    {
        return maplaver;
    }

};
class CMapLayerModNtfReq :public CEventReq
{
public:
	CMapLayerModNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_MOD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapLayer mapLayer;	             
public:
    void SetMapLayer(const MapLayer& cMapLayer)
    {
        mapLayer = cMapLayer;
    }
    MapLayer& GetMapLayer()
    {
        return mapLayer;
    }
    const MapLayer& GetMapLayer() const
    {
        return mapLayer;
    }

};
class CMapLayerDelNtfReq :public CEventReq
{
public:
	CMapLayerDelNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_LAYER_DEL_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
		layerId.clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString layerId;	             
public:
    void SetLayerId(const UUIDString& strLayerId)
    {
        layerId = strLayerId;
    }
    UUIDString& GetLayerId()
    {
        return layerId;
    }
    const UUIDString& GetLayerId() const
    {
        return layerId;
    }

};
class CMapAreaAddNtfReq :public CEventReq
{
public:
	CMapAreaAddNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_ADD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapArea mapArea;	             
public:
    void SetMapArea(const MapArea& cMapArea)
    {
        mapArea = cMapArea;
    }
    MapArea& GetMapArea()
    {
        return mapArea;
    }
    const MapArea& GetMapArea() const
    {
        return mapArea;
    }

};
class CMapAreaModNtfReq :public CEventReq
{
public:
	CMapAreaModNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_MOD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapArea mapArea;	             
public:
    void SetMapArea(const MapArea& cMapArea)
    {
        mapArea = cMapArea;
    }
    MapArea& GetMapArea()
    {
        return mapArea;
    }
    const MapArea& GetMapArea() const
    {
        return mapArea;
    }

};
class CMapAreaDelNtfReq :public CEventReq
{
public:
	CMapAreaDelNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_AREA_DEL_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
		areaId.clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString areaId;	             
public:
    void SetAreaId(const UUIDString& strAreaId)
    {
        areaId = strAreaId;
    }
    UUIDString& GetAreaId()
    {
        return areaId;
    }
    const UUIDString& GetAreaId() const
    {
        return areaId;
    }

};
class CMapElementAddNtfReq :public CEventReq
{
public:
	CMapElementAddNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_ADD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElement mapElement;	             
public:
    void SetMapElement(const MapElement& cMapElement)
    {
        mapElement = cMapElement;
    }
    MapElement& GetMapElement()
    {
        return mapElement;
    }
    const MapElement& GetMapElement() const
    {
        return mapElement;
    }

};
class CMapElementModNtfReq :public CEventReq
{
public:
	CMapElementModNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_MOD_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	MapElement mapElement;	             
public:
    void SetMapElement(const MapElement& cMapElement)
    {
        mapElement = cMapElement;
    }
    MapElement& GetMapElement()
    {
        return mapElement;
    }
    const MapElement& GetMapElement() const
    {
        return mapElement;
    }

};
class CMapElementDelNtfReq :public CEventReq
{
public:
	CMapElementDelNtfReq()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_DEL_NTF_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
		elementId.clear();
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	UUIDString elementId;	             
public:
    void SetElementId(const UUIDString& strElementId)
    {
        elementId = strElementId;
    }
    UUIDString& GetElementId()
    {
        return elementId;
    }
    const UUIDString& GetElementId() const
    {
        return elementId;
    }

};


#define  PU_GPS_STATUS       "PU_GPS"

class CPuGpsSsReq : public CListSsReq
{
public:
	CPuGpsSsReq()
	{
		event = OspExtEventDesc(MPS_PU_GPS_SS_REQ);
	};

	void Clear()
	{
		CListSsReq::Clear();
	}


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
	
};

class CPuGpsSsRsp : public CListSsRsp
{
public:
	CPuGpsSsRsp()
	{
		event = OspExtEventDesc(MPS_PU_GPS_SS_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsRefreshSsReq : public CListRefreshSsReq
{
public:
	CPuGpsRefreshSsReq()
	{
		event = OspExtEventDesc(MPS_PU_GPS_REFRESH_SS_REQ);
	};

	void Clear()
	{
		CListRefreshSsReq::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};

class CPuGpsRefreshSsRsp : public CListRefreshSsRsp
{
public:
	CPuGpsRefreshSsRsp()
	{
		event = OspExtEventDesc(MPS_PU_GPS_REFRESH_SS_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsUnSsReq : public  CListUnSsReq
{
public:
	CPuGpsUnSsReq()
	{
		event = OspExtEventDesc(MPS_PU_GPS_UNSS_REQ);
	}
	
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsUnSsRsp : public CEventRsp
{
public:
	CPuGpsUnSsRsp()
	{
		event = OspExtEventDesc(MPS_PU_GPS_UNSS_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CPuGpsNtyReq : public CEventReq
{
public:
	CPuGpsNtyReq()
	{
		event = OspExtEventDesc(MPS_PU_GPS_NTF_REQ);
	};

	void Clear()
	{
		CEventReq::Clear();
	}

private:
	string              devURI;                    //pu uuid @domain
	TGpsStatusExt		gpsStatusExt;	               		//扩展的GPS信息，含原始和加偏后地理信息
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGpsStatusExt(const TGpsStatusExt& cGpsStatusExt)
    {
        gpsStatusExt = cGpsStatusExt;
    }
    TGpsStatusExt& GetGpsStatusExt()
    {
        return gpsStatusExt;
    }
    const TGpsStatusExt& GetGpsStatusExt() const
    {
        return gpsStatusExt;
    }

public:
    void SetDevURI(const string& strDevURI)
    {
        devURI = strDevURI;
    }
    string& GetDevURI()
    {
        return devURI;
    }
    const string& GetDevURI() const
    {
        return devURI;
    }

};

typedef CNtfRsp CPuGpsNtyRsp;

class CGpsTraceQryReq : public CQueryReqBase
{
public:
	CGpsTraceQryReq(){
		event = OspExtEventDesc(MPS_GPS_TRACE_QRY_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}

	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	TraceCondition	condition;	               
public:
    void SetCondition(const TraceCondition& cTraceCond)
    {
        condition = cTraceCond;
    }
    TraceCondition& GetCondition()
    {
        return condition;
    }
    const TraceCondition& GetCondition() const
    {
        return condition;
    }

};

class CGpsTraceQryRsp : public CQueryRspBase
{
public:
	CGpsTraceQryRsp()
	{
		event = OspExtEventDesc(MPS_GPS_TRACE_QRY_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};



class CGpsTraceQryNtfReq : public CQueryNtfReqBase
{
public:
	CGpsTraceQryNtfReq()
	{
		event = OspExtEventDesc(MPS_GPS_TRACE_QRY_NTF_REQ);
	}
private:
	TGpsPointList results;	             
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetResults(const TGpsPointList& cResults)
	{
		results = cResults;
	}
	TGpsPointList& GetResults()
	{
		return results;
	}
	const TGpsPointList& GetResults() const
	{
		return results;
	}

};

class CGpsTraceQryNtfRsp : public CEventRsp
{
public:
	CGpsTraceQryNtfRsp()
	{
		event = OspExtEventDesc(MPS_GPS_TRACE_QRY_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CGpsDataBakeUpReq : public CEventReq
{
public:
	CGpsDataBakeUpReq()
	{
		event = OspExtEventDesc(MPS_GPS_DATA_BAKEUP_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}

	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

};


class CGpsDataBakeUpRsp : public CEventRsp
{
public:
	CGpsDataBakeUpRsp()
	{
		event = OspExtEventDesc(MPS_GPS_DATA_BAKEUP_RSP);
	}
	void Clear()
	{
		CEventRsp::Clear();
	}

	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string dataUrl;		               
public:
    void SetDataUrl(const string& strDataUrl)
    {
        dataUrl = strDataUrl;
    }
    string& GetDataUrl()
    {
        return dataUrl;
    }
    const string& GetDataUrl() const
    {
        return dataUrl;
    }

};


class CMpsDataVersionGetReq : public CEventReq
{
public:
	CMpsDataVersionGetReq()
	{
		event = OspExtEventDesc(MPS_DATA_VERSION_GET_REQ);
	}

	~CMpsDataVersionGetReq()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMpsDataVersionGetRsp : public CEventRsp
{
public:
	CMpsDataVersionGetRsp()
	{
		event = OspExtEventDesc(MPS_DATA_VERSION_GET_RSP);
	}

	~CMpsDataVersionGetRsp()
	{

	}

public:
	const TMpsDataVersion &GetVersion() const
	{
		return version;
	}

	void SetVersion(const TMpsDataVersion &tVersion)
	{
		version = tVersion;
	}

private:
	TMpsDataVersion version;                    //版本信息
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMpsGpsStoreCfgGetReq : public CEventReq
{
public:
	CMpsGpsStoreCfgGetReq()
	{
		event = OspExtEventDesc(MPS_GPS_STORE_CONFIG_GET_REQ);
	}

	~CMpsGpsStoreCfgGetReq()
	{

	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef bool CDbConnectStatus;

class CMpsGpsStoreCfgGetRsp : public CEventRsp
{
public:
	CMpsGpsStoreCfgGetRsp()
	{
		event = OspExtEventDesc(MPS_GPS_STORE_CONFIG_GET_RSP);
		connectStatus = false;
	}

	~CMpsGpsStoreCfgGetRsp()
	{

	}

public:
	const CGpsStoreCfg &GetStoreCfg() const
	{
		return gpsStoreCfg;
	}

	void SetStoreCfg(const CGpsStoreCfg &tCfg)
	{
		gpsStoreCfg = tCfg;
	}
	void SetConnectStatus(CDbConnectStatus bConnectStatus)
	{
		connectStatus = bConnectStatus;
	}
	CDbConnectStatus GetConnectStatus() const
	{
		return connectStatus;
	}
private:
	CGpsStoreCfg gpsStoreCfg;                    //gps存储配置信息
	CDbConnectStatus connectStatus;			               
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
  

};


class CMpsGpsStoreCfgSetReq : public CEventReq
{
public:
	CMpsGpsStoreCfgSetReq()
	{
		event = OspExtEventDesc(MPS_GPS_STORE_CONFIG_SET_REQ);
	}

	~CMpsGpsStoreCfgSetReq()
	{

	}
	const CGpsStoreCfg &GetStoreCfg() const
	{
		return gpsStoreCfg;
	}

	void SetStoreCfg(const CGpsStoreCfg &tCfg)
	{
		gpsStoreCfg = tCfg;
	}

private:
	CGpsStoreCfg gpsStoreCfg;                    //gps存储配置信息
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMpsGpsStoreCfgSetRsp : public CEventRsp
{
public:
	CMpsGpsStoreCfgSetRsp()
	{
		event = OspExtEventDesc(MPS_GPS_STORE_CONFIG_SET_RSP);
	}

	~CMpsGpsStoreCfgSetRsp()
	{

	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CMpsStoreCfgGetReq : public CEventReq
{
private:
	TMpsStoreType storeType;

public:
	CMpsStoreCfgGetReq()
	{
		event = OspExtEventDesc(MPS_STORE_CONFIG_GET_REQ);
	}
	
	void Clear()
	{
		CEventReq::Clear();
		storeType.clear();
	}

public:
	const TMpsStoreType& GetStoreType() const
	{
		return storeType;
	}
	TMpsStoreType& GetStoreType()
	{
		return storeType;
	}
	void SetStoreType(const TMpsStoreType& var)
	{
		storeType = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMpsStoreCfgGetRsp : public CEventRsp
{
private:
	string        storePath;

public:
	CMpsStoreCfgGetRsp()
	{
		event = OspExtEventDesc(MPS_STORE_CONFIG_GET_RSP);
		Clear();
	}

	void Clear()
	{
		CEventRsp::Clear();
		storePath.clear();
	}

public:
	const string& GetStorePath() const
	{
		return storePath;
	}
	string& GetStorePath()
	{
		return storePath;
	}
	void SetStorePath(const string& var)
	{
		storePath = var;
	}
	               
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


class CMpsStoreCfgSetReq : public CEventReq
{
private:
	TMpsStoreType storeType;
	string        storePath;

public:
	CMpsStoreCfgSetReq()
	{
		event = OspExtEventDesc(MPS_STORE_CONFIG_SET_REQ);
		Clear();
	}

	void Clear()
	{
		CEventReq::Clear();
		storeType.clear();
		storePath.clear();
	}

public:
	const TMpsStoreType& GetStoreType() const
	{
		return storeType;
	}
	TMpsStoreType& GetStoreType()
	{
		return storeType;
	}
	void SetStoreType(const TMpsStoreType& var)
	{
		storeType = var;
	}

	const string& GetStorePath() const
	{
		return storePath;
	}
	string& GetStorePath()
	{
		return storePath;
	}
	void SetStorePath(const string& var)
	{
		storePath = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CMpsStoreCfgSetRsp : public CEventRsp
{
public:
	CMpsStoreCfgSetRsp()
	{
		event = OspExtEventDesc(MPS_STORE_CONFIG_SET_RSP);
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


#endif





















