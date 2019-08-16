/*****************************************************************************
   ģ����      : mps_event
   �ļ���      : mps_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: mps_event.h ����������3AC_3AS����ϢID
   ����        : zhouchanghong
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/7/2   1.0         �ܲ���        ����
******************************************************************************/
#ifndef __MPS_EVENT_H__
#define __MPS_EVENT_H__
#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum EMPS_EVENT
{
	MPS_GET_SERVERINFO_REQ= MPS_EVENT_BEGIN,	//��Ϣ�壺 CGetServerInfoReq
	MPS_GET_SERVERINFO_RSP,						//��Ϣ�壺 CGetServerInfoRsp

	MPS_GET_SUPPORT_MAP_TYPES_REQ, 				//��Ϣ�壺 CGetSupportMapTypesReq	
	MPS_GET_SUPPORT_MAP_TYPES_RSP,				//��Ϣ�壺 CGetSupportMapTypesRsp

    MPS_MAP_CREATE_REQ,							//��Ϣ�壺 CMapCreateReq
	MPS_MAP_CREATE_RSP,							//��Ϣ�壺 CMapCreateRsp

	MPS_MAP_MOD_REQ,							//��Ϣ�壺 CMapModReq
	MPS_MAP_MOD_RSP,							//��Ϣ�壺 CMapModRsp

	MPS_MAP_DEL_REQ,							//��Ϣ�壺 CMapDelReq
	MPS_MAP_DEL_RSP,							//��Ϣ�壺 CMapDelRsp

	MPS_MAP_QRY_REQ,							//��Ϣ�壺 CMapQryReq
	MPS_MAP_QRY_RSP,							//��Ϣ�壺 CMapQryRsp

	MPS_MAP_QRY_NTF_REQ,						//��Ϣ�壺 CMapQryNtfReq
	MPS_MAP_QRY_NTF_RSP,						//��Ϣ�壺 CMapQryNtfRsp

	MPS_MAP_LAYER_CREATE_REQ,					//��Ϣ�壺 CMapLayerCreateReq   ����ͼ�� 
	MPS_MAP_LAYER_CREATE_RSP,					//��Ϣ�壺 CMapLayerCreateRsp

	MPS_MAP_LAYER_MOD_REQ,						//��Ϣ�壺 CMapLayerModReq	
	MPS_MAP_LAYER_MOD_RSP,						//��Ϣ�壺 CMapLayerModRsp

	MPS_MAP_LAYER_DEL_REQ,						//��Ϣ�壺 CMapLayerDelReq
	MPS_MAP_LAYER_DEL_RSP,						//��Ϣ�壺 CMapLayerDelRsp

	MPS_MAP_LAYER_QRY_REQ,						//��Ϣ�壺 CMapLayerQryReq
	MPS_MAP_LAYER_QRY_RSP,						//��Ϣ�壺 CMapLayerQryRsp

	MPS_MAP_LAYER_QRY_NTF_REQ,					//��Ϣ�壺 CMapLayerQryNtfReq
	MPS_MAP_LAYER_QRY_NTF_RSP,					//��Ϣ�壺 CMapLayerQryNtfRsp

	MPS_MAP_AREA_CREATE_REQ,					//��Ϣ�壺 CMapAreaCreateReq   ���������ӵ�ͼ
	MPS_MAP_AREA_CREATE_RSP,					//��Ϣ�壺 CMapAreaCreateRsp

	MPS_MAP_AREA_MOD_REQ,						//��Ϣ�壺 CMapAreaModReq	
	MPS_MAP_AREA_MOD_RSP,						//��Ϣ�壺 CMapAreaModRsp

	MPS_MAP_AREA_DEL_REQ,						//��Ϣ�壺 CMapAreaDelReq
	MPS_MAP_AREA_DEL_RSP,						//��Ϣ�壺 CMapAreaDelRsp

	MPS_MAP_AREA_QRY_REQ,						//��Ϣ�壺 CMapAreaQryReq
	MPS_MAP_AREA_QRY_RSP,						//��Ϣ�壺 CMapAreaQryRsp

	MPS_MAP_AREA_QRY_NTF_REQ,					//��Ϣ�壺 CMapAreaQryNtfReq
	MPS_MAP_AREA_QRY_NTF_RSP,					//��Ϣ�壺 CMapAreaQryNtfRsp

	MPS_MAP_ELEMENT_CREATE_REQ,					//��Ϣ�壺 CMapElementCreateReq
	MPS_MAP_ELEMENT_CREATE_RSP,					//��Ϣ�壺 CMapElementCreateRsp

	MPS_MAP_ELEMENT_MOD_REQ,					//��Ϣ�壺 CMapElementModReq
	MPS_MAP_ELEMENT_MOD_RSP,					//��Ϣ�壺 CMapElementModRsp

	MPS_MAP_ELEMENT_DEL_REQ,					//��Ϣ�壺 CMapElementDelReq
	MPS_MAP_ELEMENT_DEL_RSP,					//��Ϣ�壺 CMapElementDelRsp

	MPS_MAP_ELEMENT_QRY_REQ,					//��Ϣ�壺 CMapElementQryReq
	MPS_MAP_ELEMENT_QRY_RSP,					//��Ϣ�壺 CMapElementQryRsp

	MPS_MAP_ELEMENT_QRY_NTF_REQ,				//��Ϣ�壺 CMapElementQryNtfReq
	MPS_MAP_ELEMENT_QRY_NTF_RSP,				//��Ϣ�壺 CMapElementQryNtfRsp

	MPS_MAP_ELEMENT_SEARCH_REQ,					//��Ϣ�壺 CMapElementSearchReq
	MPS_MAP_ELEMENT_SEARCH_RSP,					//��Ϣ�壺 CMapElementSearchRsp

	MPS_MAP_ELEMENT_SEARCH_NTF_REQ,				//��Ϣ�壺 CMapElementSearchNtfReq
	MPS_MAP_ELEMENT_SEARCH_NTF_RSP,				//��Ϣ�壺 CMapElementSearchNtfRsp

	MPS_POS_CORRECT_EARTH_TO_MARS_REQ,			//��Ϣ�壺 CPosCorrectEarthToMarsReq
	MPS_POS_CORRECT_EARTH_TO_MARS_RSP,			//��Ϣ�壺 CPosCorrectEarthToMarsRsp
	
	MPS_POS_CORRECT_MARS_TO_EARTH_REQ,			//��Ϣ�壺 CPosCorrectMarsToEarthReq
	MPS_POS_CORRECT_MARS_TO_EARTH_RSP,			//��Ϣ�壺 CPosCorrectMarsToEarthRsp

	MPS_ELEMENT_IMAGE_QRY_REQ,					//��Ϣ�壺 CElementImageQryReq  ͼԪͼƬ��ѯ�ӿڣ���ҳ��Ч
	MPS_ELEMENT_IMAGE_QRY_RSP,					//��Ϣ�壺 CElementImageQryRsp
	MPS_ELEMENT_IMAGE_QRY_NTF_REQ,				//��Ϣ�壺 CElementImageQryNtfReq
	MPS_ELEMENT_IMAGE_QRY_NTF_RSP,				//��Ϣ�壺 CElementImageQryNtfRsp

	MPS_MAP_UPLOAD_REQ,							//��Ϣ�壺 CMapUploadReq
	MPS_MAP_UPLOAD_RSP,							//��Ϣ�壺 CMapUploadRsp

	MPS_MAP_UPLOAD_ACK,							//��Ϣ��: ����		�ļ��ϴ���ͨ���ϴ���˴����������������Ѿ�����Ҫ
												//					���ﱣ��Ϊ�������INVITʵ�ֿ���
	MPS_MAP_UPLOAD_END_REQ,
	MPS_MAP_UPLOAD_END_RSP,

	MPS_ELEMENT_ICON_UPLOAD_REQ,					//��Ϣ�壺 CElementIconUploadReq	
	MPS_ELEMENT_ICON_UPLOAD_RSP,					//��Ϣ�壺 CElementIconUploadRsp

	MPS_ELEMENT_ICON_UPLOAD_ACK,					//��Ϣ��: ����		�ļ��ϴ���ͨ���ϴ���˴����������������Ѿ�����Ҫ
	MPS_ELEMENT_ICON_UPLOAD_END_REQ,
	MPS_ELEMENT_ICON_UPLOAD_END_RSP,

	MPS_MAP_ADD_NTF_REQ, // ��Ϣ�� CMapAddNtfReq
	MPS_MAP_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_MOD_NTF_REQ, // ��Ϣ�� CMapModNtfReq
	MPS_MAP_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
	
	MPS_MAP_DEL_NTF_REQ, // ��Ϣ�� CMapDelNtfReq
	MPS_MAP_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp


	MPS_MAP_LAYER_ADD_NTF_REQ, // ��Ϣ�� CMapLayerAddNtfReq
	MPS_MAP_LAYER_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_LAYER_MOD_NTF_REQ, // ��Ϣ�� CMapLayerModNtfReq
	MPS_MAP_LAYER_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_LAYER_DEL_NTF_REQ, // ��Ϣ�� CMapLayerDelNtfReq
	MPS_MAP_LAYER_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp


	MPS_MAP_AREA_ADD_NTF_REQ, // ��Ϣ�� CMapAreaAddNtfReq
	MPS_MAP_AREA_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_AREA_MOD_NTF_REQ, // ��Ϣ�� CMapAreaModNtfReq
	MPS_MAP_AREA_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_AREA_DEL_NTF_REQ, // ��Ϣ�� CMapAreaDelNtfReq
	MPS_MAP_AREA_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp


	MPS_MAP_ELEMENT_ADD_NTF_REQ, // ��Ϣ�� CMapElementAddNtfReq
	MPS_MAP_ELEMENT_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_ELEMENT_MOD_NTF_REQ, // ��Ϣ�� CMapElementModNtfReq
	MPS_MAP_ELEMENT_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp

	MPS_MAP_ELEMENT_DEL_NTF_REQ, // ��Ϣ�� CMapElementDelNtfReq
	MPS_MAP_ELEMENT_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp


	MPS_PU_GPS_SS_REQ,        //��Ϣ��: CPuGpsSsReq
	MPS_PU_GPS_SS_RSP,        //��Ϣ��: CPuGpsSsRsp

	MPS_PU_GPS_REFRESH_SS_REQ,        //��Ϣ��: CPuGpsRefreshSsReq
	MPS_PU_GPS_REFRESH_SS_RSP,        //��Ϣ��: CPuGpsRefreshSsRsp

	MPS_PU_GPS_UNSS_REQ,        //��Ϣ��: CPuGpsUnSsReq
	MPS_PU_GPS_UNSS_RSP,        //��Ϣ��: CPuGpsUnSsRsp

	MPS_PU_GPS_NTF_REQ,       //��Ϣ��: CPuGpsNtyReq
	MPS_PU_GPS_NTF_RSP,       //��Ϣ��: CPuGpsNtyRsp

	MPS_GPS_TRACE_QRY_REQ,						//��Ϣ�壺 CGpsTraceQryReq
	MPS_GPS_TRACE_QRY_RSP,						//��Ϣ�壺 CGpsTraceQryRsp

	MPS_GPS_TRACE_QRY_NTF_REQ,					//��Ϣ�壺 CGpsTraceQryNtfReq
	MPS_GPS_TRACE_QRY_NTF_RSP,					//��Ϣ�壺 CGpsTraceQryNtfRsp

	MPS_GPS_TRACE_DEL_REQ,						//��Ϣ�壺               ��ʱδ��
	MPS_GPS_TRACE_DEL_RSP,						//��Ϣ�壺 

	MPS_GPS_DATA_BAKEUP_REQ,					//��Ϣ�壺 CGpsDataBakeUpReq             
	MPS_GPS_DATA_BAKEUP_RSP,					//��Ϣ�壺 CGpsDataBakeUpRsp

	MPS_DATA_VERSION_GET_REQ,					//��Ϣ�壺 CMpsDataVersionGetReq             
	MPS_DATA_VERSION_GET_RSP,					//��Ϣ�壺 CMpsDataVersionGetRsp

	MPS_GPS_STORE_CONFIG_GET_REQ,				//��Ϣ�壺 CMpsGpsStoreCfgGetReq
	MPS_GPS_STORE_CONFIG_GET_RSP,				//��Ϣ�壺 CMpsGpsStoreCfgGetRsp
		
	MPS_GPS_STORE_CONFIG_SET_REQ,				//��Ϣ�壺 CMpsGpsStoreCfgSetReq
	MPS_GPS_STORE_CONFIG_SET_RSP,				//��Ϣ�壺 CMpsGpsStoreCfgSetRsp

	MPS_STORE_CONFIG_GET_REQ,				    //��Ϣ�壺 CMpsStoreCfgGetReq
	MPS_STORE_CONFIG_GET_RSP,				    //��Ϣ�壺 CMpsStoreCfgGetRsp
											    
	MPS_STORE_CONFIG_SET_REQ,				    //��Ϣ�壺 CMpsStoreCfgSetReq
	MPS_STORE_CONFIG_SET_RSP,				    //��Ϣ�壺 CMpsStoreCfgSetRsp

    MPS_CURRENT_EVENT_END
};


inline void InitMpsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(MPS_GET_SERVERINFO_REQ);                  //��Ϣ�壺 CGetServerInfoReq
    OSP_ADD_EVENT_DESC(MPS_GET_SERVERINFO_RSP);                  //��Ϣ�壺 CGetServerInfoRsp
    OSP_ADD_EVENT_DESC(MPS_GET_SUPPORT_MAP_TYPES_REQ);           //��Ϣ�壺 CGetSupportMapTypesReq	
    OSP_ADD_EVENT_DESC(MPS_GET_SUPPORT_MAP_TYPES_RSP);           //��Ϣ�壺 CGetSupportMapTypesRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_CREATE_REQ);                      //��Ϣ�壺 CMapCreateReq
    OSP_ADD_EVENT_DESC(MPS_MAP_CREATE_RSP);                      //��Ϣ�壺 CMapCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_REQ);                         //��Ϣ�壺 CMapModReq
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_RSP);                         //��Ϣ�壺 CMapModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_REQ);                         //��Ϣ�壺 CMapDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_RSP);                         //��Ϣ�壺 CMapDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_REQ);                         //��Ϣ�壺 CMapQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_RSP);                         //��Ϣ�壺 CMapQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_NTF_REQ);                     //��Ϣ�壺 CMapQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_QRY_NTF_RSP);                     //��Ϣ�壺 CMapQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_CREATE_REQ);                //��Ϣ�壺 CMapLayerCreateReq   ����ͼ�� 
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_CREATE_RSP);                //��Ϣ�壺 CMapLayerCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_REQ);                   //��Ϣ�壺 CMapLayerModReq	
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_RSP);                   //��Ϣ�壺 CMapLayerModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_REQ);                   //��Ϣ�壺 CMapLayerDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_RSP);                   //��Ϣ�壺 CMapLayerDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_REQ);                   //��Ϣ�壺 CMapLayerQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_RSP);                   //��Ϣ�壺 CMapLayerQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_NTF_REQ);               //��Ϣ�壺 CMapLayerQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_QRY_NTF_RSP);               //��Ϣ�壺 CMapLayerQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_CREATE_REQ);                 //��Ϣ�壺 CMapAreaCreateReq   ���������ӵ�ͼ
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_CREATE_RSP);                 //��Ϣ�壺 CMapAreaCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_REQ);                    //��Ϣ�壺 CMapAreaModReq	
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_RSP);                    //��Ϣ�壺 CMapAreaModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_REQ);                    //��Ϣ�壺 CMapAreaDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_RSP);                    //��Ϣ�壺 CMapAreaDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_REQ);                    //��Ϣ�壺 CMapAreaQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_RSP);                    //��Ϣ�壺 CMapAreaQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_NTF_REQ);                //��Ϣ�壺 CMapAreaQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_QRY_NTF_RSP);                //��Ϣ�壺 CMapAreaQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_CREATE_REQ);              //��Ϣ�壺 CMapElementCreateReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_CREATE_RSP);              //��Ϣ�壺 CMapElementCreateRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_REQ);                 //��Ϣ�壺 CMapElementModReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_RSP);                 //��Ϣ�壺 CMapElementModRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_REQ);                 //��Ϣ�壺 CMapElementDelReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_RSP);                 //��Ϣ�壺 CMapElementDelRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_REQ);                 //��Ϣ�壺 CMapElementQryReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_RSP);                 //��Ϣ�壺 CMapElementQryRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_NTF_REQ);             //��Ϣ�壺 CMapElementQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_QRY_NTF_RSP);             //��Ϣ�壺 CMapElementQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_REQ);              //��Ϣ�壺 CMapElementSearchReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_RSP);              //��Ϣ�壺 CMapElementSearchRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_NTF_REQ);          //��Ϣ�壺 CMapElementSearchNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_SEARCH_NTF_RSP);          //��Ϣ�壺 CMapElementSearchNtfRsp
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_EARTH_TO_MARS_REQ);       //��Ϣ�壺 CPosCorrectEarthToMarsReq
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_EARTH_TO_MARS_RSP);       //��Ϣ�壺 CPosCorrectEarthToMarsRsp
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_MARS_TO_EARTH_REQ);       //��Ϣ�壺 CPosCorrectMarsToEarthReq
    OSP_ADD_EVENT_DESC(MPS_POS_CORRECT_MARS_TO_EARTH_RSP);       //��Ϣ�壺 CPosCorrectMarsToEarthRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_REQ);               //��Ϣ�壺 CElementImageQryReq  ͼԪͼƬ��ѯ�ӿڣ���ҳ��Ч
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_RSP);               //��Ϣ�壺 CElementImageQryRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_NTF_REQ);           //��Ϣ�壺 CElementImageQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_IMAGE_QRY_NTF_RSP);           //��Ϣ�壺 CElementImageQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_REQ);                      //��Ϣ�壺 CMapUploadReq
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_RSP);                      //��Ϣ�壺 CMapUploadRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_ACK);                      //��Ϣ��: ����		�ļ��ϴ���ͨ���ϴ���˴����������������Ѿ�����Ҫ
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_END_REQ);                  
    OSP_ADD_EVENT_DESC(MPS_MAP_UPLOAD_END_RSP);                  
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_REQ);             //��Ϣ�壺 CElementIconUploadReq	
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_RSP);             //��Ϣ�壺 CElementIconUploadRsp
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_ACK);             //��Ϣ��: ����		�ļ��ϴ���ͨ���ϴ���˴����������������Ѿ�����Ҫ
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_END_REQ);         
    OSP_ADD_EVENT_DESC(MPS_ELEMENT_ICON_UPLOAD_END_RSP);         
    OSP_ADD_EVENT_DESC(MPS_MAP_ADD_NTF_REQ);                     // ��Ϣ�� CMapAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ADD_NTF_RSP);                     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_NTF_REQ);                     // ��Ϣ�� CMapModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_MOD_NTF_RSP);                     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_NTF_REQ);                     // ��Ϣ�� CMapDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_DEL_NTF_RSP);                     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_ADD_NTF_REQ);               // ��Ϣ�� CMapLayerAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_ADD_NTF_RSP);               // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_NTF_REQ);               // ��Ϣ�� CMapLayerModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_MOD_NTF_RSP);               // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_NTF_REQ);               // ��Ϣ�� CMapLayerDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_LAYER_DEL_NTF_RSP);               // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_ADD_NTF_REQ);                // ��Ϣ�� CMapAreaAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_ADD_NTF_RSP);                // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_NTF_REQ);                // ��Ϣ�� CMapAreaModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_MOD_NTF_RSP);                // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_NTF_REQ);                // ��Ϣ�� CMapAreaDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_AREA_DEL_NTF_RSP);                // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_ADD_NTF_REQ);             // ��Ϣ�� CMapElementAddNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_ADD_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_NTF_REQ);             // ��Ϣ�� CMapElementModNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_MOD_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_NTF_REQ);             // ��Ϣ�� CMapElementDelNtfReq
    OSP_ADD_EVENT_DESC(MPS_MAP_ELEMENT_DEL_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_SS_REQ);                       //��Ϣ��: CPuGpsSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_SS_RSP);                       //��Ϣ��: CPuGpsSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_REFRESH_SS_REQ);               //��Ϣ��: CPuGpsRefreshSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_REFRESH_SS_RSP);               //��Ϣ��: CPuGpsRefreshSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_UNSS_REQ);                     //��Ϣ��: CPuGpsUnSsReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_UNSS_RSP);                     //��Ϣ��: CPuGpsUnSsRsp
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_NTF_REQ);                      //��Ϣ��: CPuGpsNtyReq
    OSP_ADD_EVENT_DESC(MPS_PU_GPS_NTF_RSP);                      //��Ϣ��: CPuGpsNtyRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_REQ);                   //��Ϣ�壺 CGpsTraceQryReq
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_RSP);                   //��Ϣ�壺 CGpsTraceQryRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_NTF_REQ);               //��Ϣ�壺 CGpsTraceQryNtfReq
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_QRY_NTF_RSP);               //��Ϣ�壺 CGpsTraceQryNtfRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_DEL_REQ);                   //��Ϣ�壺               ��ʱδ��
    OSP_ADD_EVENT_DESC(MPS_GPS_TRACE_DEL_RSP);                   //��Ϣ�壺 
    OSP_ADD_EVENT_DESC(MPS_GPS_DATA_BAKEUP_REQ);                 //��Ϣ�壺 CGpsDataBakeUpReq             
    OSP_ADD_EVENT_DESC(MPS_GPS_DATA_BAKEUP_RSP);                 //��Ϣ�壺 CGpsDataBakeUpRsp
    OSP_ADD_EVENT_DESC(MPS_DATA_VERSION_GET_REQ);                //��Ϣ�壺 CMpsDataVersionGetReq             
    OSP_ADD_EVENT_DESC(MPS_DATA_VERSION_GET_RSP);                //��Ϣ�壺 CMpsDataVersionGetRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_GET_REQ);            //��Ϣ�壺 CMpsGpsStoreCfgGetReq
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_GET_RSP);            //��Ϣ�壺 CMpsGpsStoreCfgGetRsp
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_SET_REQ);            //��Ϣ�壺 CMpsGpsStoreCfgSetReq
    OSP_ADD_EVENT_DESC(MPS_GPS_STORE_CONFIG_SET_RSP);            //��Ϣ�壺 CMpsGpsStoreCfgSetRsp
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_GET_REQ);                //��Ϣ�壺 CMpsStoreCfgGetReq
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_GET_RSP);                //��Ϣ�壺 CMpsStoreCfgGetRsp
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_SET_REQ);                //��Ϣ�壺 CMpsStoreCfgSetReq
    OSP_ADD_EVENT_DESC(MPS_STORE_CONFIG_SET_RSP);                //��Ϣ�壺 CMpsStoreCfgSetRsp
    OSP_ADD_EVENT_DESC(MPS_CURRENT_EVENT_END);                   
}


#endif  //#ifndef __mps_EVENT_H__




















