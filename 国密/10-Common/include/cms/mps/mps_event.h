/*****************************************************************************
   模块名      : mps_event
   文件名      : mps_event.h
   相关文件    : 
   文件实现功能: mps_event.h 定义了所有3AC_3AS的消息ID
   作者        : zhouchanghong
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/7/2   1.0         周昌鸿        创建
******************************************************************************/
#ifndef __MPS_EVENT_H__
#define __MPS_EVENT_H__
#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum EMPS_EVENT
{
	MPS_GET_SERVERINFO_REQ= MPS_EVENT_BEGIN,	//消息体： CGetServerInfoReq
	MPS_GET_SERVERINFO_RSP,						//消息体： CGetServerInfoRsp

	MPS_GET_SUPPORT_MAP_TYPES_REQ, 				//消息体： CGetSupportMapTypesReq	
	MPS_GET_SUPPORT_MAP_TYPES_RSP,				//消息体： CGetSupportMapTypesRsp

    MPS_MAP_CREATE_REQ,							//消息体： CMapCreateReq
	MPS_MAP_CREATE_RSP,							//消息体： CMapCreateRsp

	MPS_MAP_MOD_REQ,							//消息体： CMapModReq
	MPS_MAP_MOD_RSP,							//消息体： CMapModRsp

	MPS_MAP_DEL_REQ,							//消息体： CMapDelReq
	MPS_MAP_DEL_RSP,							//消息体： CMapDelRsp

	MPS_MAP_QRY_REQ,							//消息体： CMapQryReq
	MPS_MAP_QRY_RSP,							//消息体： CMapQryRsp

	MPS_MAP_QRY_NTF_REQ,						//消息体： CMapQryNtfReq
	MPS_MAP_QRY_NTF_RSP,						//消息体： CMapQryNtfRsp

	MPS_MAP_LAYER_CREATE_REQ,					//消息体： CMapLayerCreateReq   创建图层 
	MPS_MAP_LAYER_CREATE_RSP,					//消息体： CMapLayerCreateRsp

	MPS_MAP_LAYER_MOD_REQ,						//消息体： CMapLayerModReq	
	MPS_MAP_LAYER_MOD_RSP,						//消息体： CMapLayerModRsp

	MPS_MAP_LAYER_DEL_REQ,						//消息体： CMapLayerDelReq
	MPS_MAP_LAYER_DEL_RSP,						//消息体： CMapLayerDelRsp

	MPS_MAP_LAYER_QRY_REQ,						//消息体： CMapLayerQryReq
	MPS_MAP_LAYER_QRY_RSP,						//消息体： CMapLayerQryRsp

	MPS_MAP_LAYER_QRY_NTF_REQ,					//消息体： CMapLayerQryNtfReq
	MPS_MAP_LAYER_QRY_NTF_RSP,					//消息体： CMapLayerQryNtfRsp

	MPS_MAP_AREA_CREATE_REQ,					//消息体： CMapAreaCreateReq   创建区域子地图
	MPS_MAP_AREA_CREATE_RSP,					//消息体： CMapAreaCreateRsp

	MPS_MAP_AREA_MOD_REQ,						//消息体： CMapAreaModReq	
	MPS_MAP_AREA_MOD_RSP,						//消息体： CMapAreaModRsp

	MPS_MAP_AREA_DEL_REQ,						//消息体： CMapAreaDelReq
	MPS_MAP_AREA_DEL_RSP,						//消息体： CMapAreaDelRsp

	MPS_MAP_AREA_QRY_REQ,						//消息体： CMapAreaQryReq
	MPS_MAP_AREA_QRY_RSP,						//消息体： CMapAreaQryRsp

	MPS_MAP_AREA_QRY_NTF_REQ,					//消息体： CMapAreaQryNtfReq
	MPS_MAP_AREA_QRY_NTF_RSP,					//消息体： CMapAreaQryNtfRsp

	MPS_MAP_ELEMENT_CREATE_REQ,					//消息体： CMapElementCreateReq
	MPS_MAP_ELEMENT_CREATE_RSP,					//消息体： CMapElementCreateRsp

	MPS_MAP_ELEMENT_MOD_REQ,					//消息体： CMapElementModReq
	MPS_MAP_ELEMENT_MOD_RSP,					//消息体： CMapElementModRsp

	MPS_MAP_ELEMENT_DEL_REQ,					//消息体： CMapElementDelReq
	MPS_MAP_ELEMENT_DEL_RSP,					//消息体： CMapElementDelRsp

	MPS_MAP_ELEMENT_QRY_REQ,					//消息体： CMapElementQryReq
	MPS_MAP_ELEMENT_QRY_RSP,					//消息体： CMapElementQryRsp

	MPS_MAP_ELEMENT_QRY_NTF_REQ,				//消息体： CMapElementQryNtfReq
	MPS_MAP_ELEMENT_QRY_NTF_RSP,				//消息体： CMapElementQryNtfRsp

	MPS_MAP_ELEMENT_SEARCH_REQ,					//消息体： CMapElementSearchReq
	MPS_MAP_ELEMENT_SEARCH_RSP,					//消息体： CMapElementSearchRsp

	MPS_MAP_ELEMENT_SEARCH_NTF_REQ,				//消息体： CMapElementSearchNtfReq
	MPS_MAP_ELEMENT_SEARCH_NTF_RSP,				//消息体： CMapElementSearchNtfRsp

	MPS_POS_CORRECT_EARTH_TO_MARS_REQ,			//消息体： CPosCorrectEarthToMarsReq
	MPS_POS_CORRECT_EARTH_TO_MARS_RSP,			//消息体： CPosCorrectEarthToMarsRsp
	
	MPS_POS_CORRECT_MARS_TO_EARTH_REQ,			//消息体： CPosCorrectMarsToEarthReq
	MPS_POS_CORRECT_MARS_TO_EARTH_RSP,			//消息体： CPosCorrectMarsToEarthRsp

	MPS_ELEMENT_IMAGE_QRY_REQ,					//消息体： CElementImageQryReq  图元图片查询接口，分页无效
	MPS_ELEMENT_IMAGE_QRY_RSP,					//消息体： CElementImageQryRsp
	MPS_ELEMENT_IMAGE_QRY_NTF_REQ,				//消息体： CElementImageQryNtfReq
	MPS_ELEMENT_IMAGE_QRY_NTF_RSP,				//消息体： CElementImageQryNtfRsp

	MPS_MAP_UPLOAD_REQ,							//消息体： CMapUploadReq
	MPS_MAP_UPLOAD_RSP,							//消息体： CMapUploadRsp

	MPS_MAP_UPLOAD_ACK,							//消息体: 暂无		文件上传由通用上传后端处理，下面三条信令已经不需要
												//					这里保留为后续变更INVIT实现考虑
	MPS_MAP_UPLOAD_END_REQ,
	MPS_MAP_UPLOAD_END_RSP,

	MPS_ELEMENT_ICON_UPLOAD_REQ,					//消息体： CElementIconUploadReq	
	MPS_ELEMENT_ICON_UPLOAD_RSP,					//消息体： CElementIconUploadRsp

	MPS_ELEMENT_ICON_UPLOAD_ACK,					//消息体: 暂无		文件上传由通用上传后端处理，下面三条信令已经不需要
	MPS_ELEMENT_ICON_UPLOAD_END_REQ,
	MPS_ELEMENT_ICON_UPLOAD_END_RSP,

	MPS_MAP_ADD_NTF_REQ, // 消息体 CMapAddNtfReq
	MPS_MAP_ADD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_MOD_NTF_REQ, // 消息体 CMapModNtfReq
	MPS_MAP_MOD_NTF_RSP, // 消息体 CNtfRsp
	
	MPS_MAP_DEL_NTF_REQ, // 消息体 CMapDelNtfReq
	MPS_MAP_DEL_NTF_RSP, // 消息体 CNtfRsp


	MPS_MAP_LAYER_ADD_NTF_REQ, // 消息体 CMapLayerAddNtfReq
	MPS_MAP_LAYER_ADD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_LAYER_MOD_NTF_REQ, // 消息体 CMapLayerModNtfReq
	MPS_MAP_LAYER_MOD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_LAYER_DEL_NTF_REQ, // 消息体 CMapLayerDelNtfReq
	MPS_MAP_LAYER_DEL_NTF_RSP, // 消息体 CNtfRsp


	MPS_MAP_AREA_ADD_NTF_REQ, // 消息体 CMapAreaAddNtfReq
	MPS_MAP_AREA_ADD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_AREA_MOD_NTF_REQ, // 消息体 CMapAreaModNtfReq
	MPS_MAP_AREA_MOD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_AREA_DEL_NTF_REQ, // 消息体 CMapAreaDelNtfReq
	MPS_MAP_AREA_DEL_NTF_RSP, // 消息体 CNtfRsp


	MPS_MAP_ELEMENT_ADD_NTF_REQ, // 消息体 CMapElementAddNtfReq
	MPS_MAP_ELEMENT_ADD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_ELEMENT_MOD_NTF_REQ, // 消息体 CMapElementModNtfReq
	MPS_MAP_ELEMENT_MOD_NTF_RSP, // 消息体 CNtfRsp

	MPS_MAP_ELEMENT_DEL_NTF_REQ, // 消息体 CMapElementDelNtfReq
	MPS_MAP_ELEMENT_DEL_NTF_RSP, // 消息体 CNtfRsp


	MPS_PU_GPS_SS_REQ,        //消息体: CPuGpsSsReq
	MPS_PU_GPS_SS_RSP,        //消息体: CPuGpsSsRsp

	MPS_PU_GPS_REFRESH_SS_REQ,        //消息体: CPuGpsRefreshSsReq
	MPS_PU_GPS_REFRESH_SS_RSP,        //消息体: CPuGpsRefreshSsRsp

	MPS_PU_GPS_UNSS_REQ,        //消息体: CPuGpsUnSsReq
	MPS_PU_GPS_UNSS_RSP,        //消息体: CPuGpsUnSsRsp

	MPS_PU_GPS_NTF_REQ,       //消息体: CPuGpsNtyReq
	MPS_PU_GPS_NTF_RSP,       //消息体: CPuGpsNtyRsp

	MPS_GPS_TRACE_QRY_REQ,						//消息体： CGpsTraceQryReq
	MPS_GPS_TRACE_QRY_RSP,						//消息体： CGpsTraceQryRsp

	MPS_GPS_TRACE_QRY_NTF_REQ,					//消息体： CGpsTraceQryNtfReq
	MPS_GPS_TRACE_QRY_NTF_RSP,					//消息体： CGpsTraceQryNtfRsp

	MPS_GPS_TRACE_DEL_REQ,						//消息体：               暂时未用
	MPS_GPS_TRACE_DEL_RSP,						//消息体： 

	MPS_GPS_DATA_BAKEUP_REQ,					//消息体： CGpsDataBakeUpReq             
	MPS_GPS_DATA_BAKEUP_RSP,					//消息体： CGpsDataBakeUpRsp

	MPS_DATA_VERSION_GET_REQ,					//消息体： CMpsDataVersionGetReq             
	MPS_DATA_VERSION_GET_RSP,					//消息体： CMpsDataVersionGetRsp

	MPS_GPS_STORE_CONFIG_GET_REQ,				//消息体： CMpsGpsStoreCfgGetReq
	MPS_GPS_STORE_CONFIG_GET_RSP,				//消息体： CMpsGpsStoreCfgGetRsp
		
	MPS_GPS_STORE_CONFIG_SET_REQ,				//消息体： CMpsGpsStoreCfgSetReq
	MPS_GPS_STORE_CONFIG_SET_RSP,				//消息体： CMpsGpsStoreCfgSetRsp

	MPS_STORE_CONFIG_GET_REQ,				    //消息体： CMpsStoreCfgGetReq
	MPS_STORE_CONFIG_GET_RSP,				    //消息体： CMpsStoreCfgGetRsp
											    
	MPS_STORE_CONFIG_SET_REQ,				    //消息体： CMpsStoreCfgSetReq
	MPS_STORE_CONFIG_SET_RSP,				    //消息体： CMpsStoreCfgSetRsp

    MPS_CURRENT_EVENT_END
};


inline void InitMpsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(MPS_GET_SERVERINFO_REQ);                  //消息体： CGetServerInfoReq
    OSP_ADD_EVENT_DESC(MPS_GET_SERVERINFO_RSP);                  //消息体： CGetServerInfoRsp
    OSP_ADD_EVENT_DESC(MPS_GET_SUPPORT_MAP_TYPES_REQ);           //消息体： CGetSupportMapTypesReq	
    OSP_ADD_EVENT_DESC(MPS_GET_SUPPORT_MAP_TYPES_RSP);           //消息体： CGetSupportMapTypesRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_CREATE_REQ);                      //消息体： CMapCreateReq
    OSP_ADD_EVENT_DESC(MPS_MAP_CREATE_RSP);                      //消息体： CMapCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_REQ);                         //消息体： CMapModReq
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_RSP);                         //消息体： CMapModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_REQ);                         //消息体： CMapDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_RSP);                         //消息体： CMapDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_REQ);                         //消息体： CMapQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_RSP);                         //消息体： CMapQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_NTF_REQ);                     //消息体： CMapQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_NTF_RSP);                     //消息体： CMapQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_CREATE_REQ);                //消息体： CMapLayerCreateReq   创建图层 
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_CREATE_RSP);                //消息体： CMapLayerCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_REQ);                   //消息体： CMapLayerModReq	
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_RSP);                   //消息体： CMapLayerModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_REQ);                   //消息体： CMapLayerDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_RSP);                   //消息体： CMapLayerDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_REQ);                   //消息体： CMapLayerQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_RSP);                   //消息体： CMapLayerQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_NTF_REQ);               //消息体： CMapLayerQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_NTF_RSP);               //消息体： CMapLayerQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_CREATE_REQ);                 //消息体： CMapAreaCreateReq   创建区域子地图
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_CREATE_RSP);                 //消息体： CMapAreaCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_REQ);                    //消息体： CMapAreaModReq	
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_RSP);                    //消息体： CMapAreaModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_REQ);                    //消息体： CMapAreaDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_RSP);                    //消息体： CMapAreaDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_REQ);                    //消息体： CMapAreaQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_RSP);                    //消息体： CMapAreaQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_NTF_REQ);                //消息体： CMapAreaQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_NTF_RSP);                //消息体： CMapAreaQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_CREATE_REQ);              //消息体： CMapElementCreateReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_CREATE_RSP);              //消息体： CMapElementCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_REQ);                 //消息体： CMapElementModReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_RSP);                 //消息体： CMapElementModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_REQ);                 //消息体： CMapElementDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_RSP);                 //消息体： CMapElementDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_REQ);                 //消息体： CMapElementQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_RSP);                 //消息体： CMapElementQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_NTF_REQ);             //消息体： CMapElementQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_NTF_RSP);             //消息体： CMapElementQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_REQ);              //消息体： CMapElementSearchReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_RSP);              //消息体： CMapElementSearchRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_NTF_REQ);          //消息体： CMapElementSearchNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_NTF_RSP);          //消息体： CMapElementSearchNtfRsp
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_EARTH_TO_MARS_REQ);       //消息体： CPosCorrectEarthToMarsReq
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_EARTH_TO_MARS_RSP);       //消息体： CPosCorrectEarthToMarsRsp
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_MARS_TO_EARTH_REQ);       //消息体： CPosCorrectMarsToEarthReq
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_MARS_TO_EARTH_RSP);       //消息体： CPosCorrectMarsToEarthRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_REQ);               //消息体： CElementImageQryReq  图元图片查询接口，分页无效
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_RSP);               //消息体： CElementImageQryRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_NTF_REQ);           //消息体： CElementImageQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_NTF_RSP);           //消息体： CElementImageQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_REQ);                      //消息体： CMapUploadReq
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_RSP);                      //消息体： CMapUploadRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_ACK);                      //消息体: 暂无		文件上传由通用上传后端处理，下面三条信令已经不需要
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_END_REQ);                  
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_END_RSP);                  
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_REQ);             //消息体： CElementIconUploadReq	
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_RSP);             //消息体： CElementIconUploadRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_ACK);             //消息体: 暂无		文件上传由通用上传后端处理，下面三条信令已经不需要
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_END_REQ);         
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_END_RSP);         
    OSP_ADD_EVENT_DESC(MPS_MAP_ADD_NTF_REQ);                     // 消息体 CMapAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ADD_NTF_RSP);                     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_NTF_REQ);                     // 消息体 CMapModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_NTF_RSP);                     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_NTF_REQ);                     // 消息体 CMapDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_NTF_RSP);                     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_ADD_NTF_REQ);               // 消息体 CMapLayerAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_ADD_NTF_RSP);               // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_NTF_REQ);               // 消息体 CMapLayerModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_NTF_RSP);               // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_NTF_REQ);               // 消息体 CMapLayerDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_NTF_RSP);               // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_ADD_NTF_REQ);                // 消息体 CMapAreaAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_ADD_NTF_RSP);                // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_NTF_REQ);                // 消息体 CMapAreaModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_NTF_RSP);                // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_NTF_REQ);                // 消息体 CMapAreaDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_NTF_RSP);                // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_ADD_NTF_REQ);             // 消息体 CMapElementAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_ADD_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_NTF_REQ);             // 消息体 CMapElementModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_NTF_REQ);             // 消息体 CMapElementDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_SS_REQ);                       //消息体: CPuGpsSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_SS_RSP);                       //消息体: CPuGpsSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_REFRESH_SS_REQ);               //消息体: CPuGpsRefreshSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_REFRESH_SS_RSP);               //消息体: CPuGpsRefreshSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_UNSS_REQ);                     //消息体: CPuGpsUnSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_UNSS_RSP);                     //消息体: CPuGpsUnSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_NTF_REQ);                      //消息体: CPuGpsNtyReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_NTF_RSP);                      //消息体: CPuGpsNtyRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_REQ);                   //消息体： CGpsTraceQryReq
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_RSP);                   //消息体： CGpsTraceQryRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_NTF_REQ);               //消息体： CGpsTraceQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_NTF_RSP);               //消息体： CGpsTraceQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_DEL_REQ);                   //消息体：               暂时未用
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_DEL_RSP);                   //消息体： 
    OSP_ADD_EVENT_DESC(MPS_GPS_DATA_BAKEUP_REQ);                 //消息体： CGpsDataBakeUpReq             
    OSP_ADD_EVENT_DESC(MPS_GPS_DATA_BAKEUP_RSP);                 //消息体： CGpsDataBakeUpRsp
    OSP_ADD_EVENT_DESC(MPS_DATA_VERSION_GET_REQ);                //消息体： CMpsDataVersionGetReq             
    OSP_ADD_EVENT_DESC(MPS_DATA_VERSION_GET_RSP);                //消息体： CMpsDataVersionGetRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_GET_REQ);            //消息体： CMpsGpsStoreCfgGetReq
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_GET_RSP);            //消息体： CMpsGpsStoreCfgGetRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_SET_REQ);            //消息体： CMpsGpsStoreCfgSetReq
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_SET_RSP);            //消息体： CMpsGpsStoreCfgSetRsp
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_GET_REQ);                //消息体： CMpsStoreCfgGetReq
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_GET_RSP);                //消息体： CMpsStoreCfgGetRsp
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_SET_REQ);                //消息体： CMpsStoreCfgSetReq
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_SET_RSP);                //消息体： CMpsStoreCfgSetRsp
    OSP_ADD_EVENT_DESC(MPS_CURRENT_EVENT_END);                   
}


#endif  //#ifndef __mps_EVENT_H__




















