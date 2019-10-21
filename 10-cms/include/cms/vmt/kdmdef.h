/*=======================================================================
ģ����      : KDM define
�ļ���      : kdmdef.h
����ļ�    :
�ļ�ʵ�ֹ��� :
����        : κ�α�
�汾        : V3.9  Copyright(C) 2001-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2004/02/17  3.0         κ�α�      ����
2006/02/23  3.9         ���ǿ      �޸�Ϊȫ������Ŀ�Ķ���
=======================================================================*/
#ifndef _KDM_DEFINE_H_

#define _KDM_DEFINE_H_



#ifdef __cplusplus

extern "C" {

#endif /* __cplusplus */


	

//APP ID
#define AID_KDM_BASE                      (u16)0 

/*NMC�ڲ�Ӧ�úţ�1-10��*/
#define AID_TNMC_BGN					AID_KDM_BASE + 1
#define AID_TNMC_END					AID_TNMC_BGN + 9

/*NMS�ڲ�Ӧ�úţ�11-20��*/
#define AID_TNMS_BGN					AID_TNMC_END + 1
#define AID_TNMS_END					AID_TNMS_BGN + 9	

/*3AC�ڲ�Ӧ�úţ�21-30��*/
#define AID_3AC_BGN						AID_TNMS_END + 1
#define AID_3AC_END						AID_3AC_BGN + 9

/*3AS�ڲ�Ӧ�úţ�31-40��*/

#define AID_3AS_BGN						AID_3AC_END + 1
#define AID_3AS_END						AID_3AS_BGN + 9

/*CMU�ڲ�Ӧ�úţ�41-50��*/
#define AID_CMU_BGN						AID_3AS_END + 1
#define AID_CMU_END						AID_CMU_BGN + 9

/*CU�ڲ�Ӧ�úţ�51-100��*/
#define AID_CU_BGN						AID_CMU_END + 1
#define AID_CU_END						AID_CU_BGN + 49

/*NRU�ڲ�Ӧ�úţ�101-150��*/
#define AID_NRU_BGN						AID_CU_END + 1
#define AID_NRU_END						AID_NRU_BGN + 49

/*VTDU�ڲ�Ӧ�úţ�151-160��*/
#define AID_VTDU_BGN					AID_NRU_END + 1
#define AID_VTDU_END					AID_VTDU_BGN + 9

/*UAS�ڲ�Ӧ�úţ�161-170��*/
#define AID_UAS_BGN						AID_VTDU_END + 1
#define AID_UAS_END						AID_UAS_BGN + 9

//Add by luxz
/*CUI�ڲ�Ӧ�úţ�171-180��*/
#define AID_CUI_BGN						AID_UAS_END + 1
#define AID_CUI_END						AID_CUI_BGN + 9

/*PUI�ڲ�Ӧ�úţ�181-190��*/
#define AID_PUI_BGN						AID_CUI_END + 1
#define AID_PUI_END						AID_PUI_BGN + 9
//end of adding by luxz

/*PU�ڲ�Ӧ�úţ�191-200��*/
#define AID_PU_BGN						AID_PUI_END + 1
#define AID_PU_END						AID_PU_BGN + 9

/*add by aaron 2006.09.11*/
/*VPU�ڲ�Ӧ�úţ�201-210��*/
#define AID_VPU_BGN						AID_PU_END + 1
#define AID_VPU_END						AID_VPU_BGN + 9
/*end by aaron*/

/*add by aaron 2006.09.11*/
/*watchdog�ڲ�Ӧ�úţ�211-220��*/
#define AID_WTDOG_BGN					AID_VPU_END + 1
#define AID_WTDOG_END					AID_WTDOG_BGN + 9
/*end by aaron*/

//Add by suncy
/*����ƽ���ڲ�Ӧ�úţ�221-230��*/
#define AID_LADEN_BGN   				AID_WTDOG_END + 1
#define AID_LADEN_END					AID_LADEN_BGN + 9

// add by yangxin 2007-12-05
/*PMC�ڲ�Ӧ�úţ�231-330��*/
#define AID_TPMC_BGN					(AID_LADEN_END + 1)
#define AID_TPMC_END					(AID_TPMC_BGN + 99)//ModifybyYzzrn

/*PMS�ڲ�Ӧ�úţ�331-340��*/
#define AID_TPMS_BGN					AID_TPMC_END + 1
#define AID_TPMS_END					AID_TPMS_BGN + 9
// edit end
	
// ��ز�Ʒ��ģ��ļ����˿�
const u16 PORT_TNMS				= 60000;
const u16 PORT_TNMC				= 60000;
const u16 PORT_CMU				= 1722;
const u16 PORT_CU				= 60000;
const u16 PORT_3AS				= 60000;
const u16 PORT_3AC				= 60000;
const u16 PORT_NRU				= 50050;   /*modified by aaron 2006.05.09*/
const u16 PORT_VTDU				= 60000;
const u16 PORT_UAS        		= 60000;
const u16 PORT_WATCHDOG         = 59000;   /*modified by aaron 2006.10.27*/

//add by luxz
const u16 PORT_CUI        		= 1722;  /*modified by zhengrong 2006.05.24*/
const u16 PORT_LDS              = 5510;  //port for lds
const u16 PORT_CAM				= 1729;  /*modified by zhengrong 2007.04.05*/
//end of adding by luxz
	
#ifdef __cplusplus

}

#endif  /* __cplusplus */



#endif /* _KDM_DEFINE_H_ */



/* end of file kdmdef.h */
























