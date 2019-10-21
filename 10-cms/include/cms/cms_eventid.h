/*****************************************************************************
   ģ����      : cms_eventid
   �ļ���      : cms_eventid.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cms_eventid.h ��������SIP��Ϣ�������
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli        ����
   2013/11/25               huzhiyun       ���gbs��Ϣ��Χ
******************************************************************************/

#ifndef _CMS_EVENT_ID_H_
#define _CMS_EVENT_ID_H_

#define OSP_SIP_EVENT_BEGIN         (1000)          //OspSip���ϲ�ҵ�񽻻���Ϣ��ʼ��
#define DBIX_EVENT_BEGIN            (1600)          //DBIX���ϲ�ҵ�񽻻���Ϣ��ʼ��
#define OSP_EXT_USER_EVENT_BEGIN    (OSP_SIP_EVENT_BEGIN + 1000)        //�����û���Sip��Ϣ��ʼ�� 2000
#define OSP_EXT_INNER_MSG_BEGIN     (50000)         //OspExt���ڲ���Ϣ, �������û�ģ���Զ�����Ϣ����
#define USER_INNER_MSG_BEGIN        (55000)         //�û�ģ���ڲ��Զ�����Ϣ, �Ͻ��Զ����ڲ���Ϣ��ģ�����ͨ

/*
    BEGIN: NMS�Ᵽ�ֲ��䣬����ԭ����Ϣ�ź�Ӧ�úŶ��巶Χ
*/
/*�豸������NMS�ӿ���Ϣ��Χ[52001-53000]*/
/*NMCӦ�úŷ�Χ[1-10]*/
/*NMSӦ�úŷ�Χ[11-20]*/
/*
    END: NMS�Ᵽ�ֲ��䣬����ԭ����Ϣ���巶Χ
*/

enum TAC_TAS_EVENT
{
    TAC_TAS_EVENT_BEGIN = OSP_EXT_USER_EVENT_BEGIN + 1, //TAC_TAS_EVENT_BEGIN = 2001
    TAC_TAS_EVENT_END = TAC_TAS_EVENT_BEGIN + 999       //TAC_TAS_EVENT_END = 3000
};

enum PMC_PMS_EVENT
{
    PMC_PMS_EVENT_BEGIN = TAC_TAS_EVENT_END + 1, //PMC_PMS_EVENT_BEGIN = 3001
    PMC_PMS_EVENT_END = PMC_PMS_EVENT_BEGIN + 999  //PMC_PMS_EVENT_END = 4000
};

enum CMS_EVENT
{
    CMS_EVENT_BEGIN = PMC_PMS_EVENT_END + 1,        //CMS_EVENT_BEGIN = 4001
    CMS_EVENT_END = CMS_EVENT_BEGIN + 19999         //CMS_EVENT_END = 24000
};

enum PMCS_EVENT
{
	PMC_PMCS_EVENT_BEGIN = CMS_EVENT_END + 1,		//PMC_PMCS_EVENT_BEGIN = 24001
	PMC_PMCS_EVENT_END = PMC_PMCS_EVENT_BEGIN + 999	//PMC_PMCS_EVENT_END =  25000
};

enum VTDU_EVENT
{
    VTDU_EVENT_BEGIN = PMC_PMCS_EVENT_END + 1,      //VTDU_EVENT_BEGIN = 25001
    VTDU_EVENT_END = VTDU_EVENT_BEGIN + 499         //VTDU_EVENT_END = 25500
};

enum RCS_EVENT
{
    RCS_EVENT_BEGIN = VTDU_EVENT_END + 1,          //RCS_EVENT_BEGIN = 25501
    RCS_EVENT_END = RCS_EVENT_BEGIN + 499          //RCS_EVENT_END = 26000
};

enum UAS_EVENT
{
	UAS_EVENT_BEGIN = RCS_EVENT_END + 1,          //UAS_EVENT_BEGIN = 26001
	UAS_EVENT_END = UAS_EVENT_BEGIN + 499         //UAS_EVENT_END = 26500
};

enum MPS_EVENT
{
	MPS_EVENT_BEGIN = UAS_EVENT_END +1,			//MPS_EVENT_BEGIN = 26501
	MPS_EVENT_END = MPS_EVENT_BEGIN + 499		//MPS_EVENT_END	= 27000
};

enum GBS_EVENT
{
    GBS_EVENT_BEGIN = MPS_EVENT_END +1,			//MPS_EVENT_BEGIN = 27001
    GBS_EVENT_END = GBS_EVENT_BEGIN + 499		//MPS_EVENT_END	= 27500
};

enum CMSSDK_EVENT
{
    CMSSDK_EVENT_BEGIN = GBS_EVENT_END +1,			//CMSSDK_EVENT_BEGIN = 27501
    CMSSDK_EVENT_END = CMSSDK_EVENT_BEGIN + 499		//CMSSDK_EVENT_END	= 28000
};

enum CAPS_EVENT
{
	CAPS_EVENT_BEGIN = CMSSDK_EVENT_END +1,			//CAPS_EVENT_BEGIN = 28001
	CAPS_EVENT_END = CAPS_EVENT_BEGIN + 499		//CMSSDK_EVENT_END	= 28500
};

enum DPS_EVENT
{
	DPS_EVENT_BEGIN = CAPS_EVENT_END + 1,	//	DPS_EVENT_BEGIN = 28501
	DPS_EVENT_END = DPS_EVENT_BEGIN + 499,	// DPS_EVENT_END = 29000
};

enum SECURITY_MODULE_EVENT
{
    SECURITY_MODULE_EVENT_BEGIN = DPS_EVENT_END + 1,	//	DPS_EVENT_BEGIN = 29001
    SECURITY_MODULE_EVENT_END = DPS_EVENT_BEGIN + 499,	// DPS_EVENT_END = 29500
};

#endif // _CMS_EVENT_ID_H_
