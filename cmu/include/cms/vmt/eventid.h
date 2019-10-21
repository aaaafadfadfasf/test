/*=======================================================================
ģ����      :ȫ����
�ļ���      : eventid.h
����ļ�    :
�ļ�ʵ�ֹ��� :
����        : ����
�汾        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2002/03/01  0.9         ����        ����
2004/03/11  3.0         κ�α�      ���ӵ�ַ����Ϣ
2006/02/22  3.9         ���ǿ      ����Ƶ��Ʒ�޸�Ϊȫ���۵�����
=======================================================================*/
#ifndef _EVENT_ID_H_
#define _EVENT_ID_H_

#define EV_KDM_BASE     10000

// ��ģ���ڲ���Ϣ
#define EV_KDM_INNER_BASE   (EV_KDM_BASE + 10000)
// ��ģ���ⲿ��Ϣ
#define EV_KDM_OUTTER_BASE  (EV_KDM_INNER_BASE + 20000)

//////////////////////////////////////////////////////////////////////////
//  ��ģ���ڲ���Ϣ
// 
/*NMC�ڲ���Ϣ��10001-11000��*/
#define EV_NMC_BGN      (EV_KDM_BASE + 1)
#define EV_NMC_END      (EV_NMC_BGN + 999)

/*NMS�ڲ���Ϣ��11001-12000��*/
#define EV_NMS_BGN      (EV_NMC_END + 1)
#define EV_NMS_END      (EV_NMS_BGN + 999)

/*3AC�ڲ���Ϣ��12001-13000��*/
#define EV_3AC_BGN              (EV_NMS_END + 1)
#define EV_3AC_END              (EV_3AC_BGN + 999)

/*3AS�ڲ���Ϣ��13001-14000��*/
#define EV_3AS_BGN              (EV_3AC_END + 1)
#define EV_3AS_END              (EV_3AS_BGN + 999)

/*CMU�ڲ���Ϣ��14001-15000��*/
#define EV_CMU_BGN              (EV_3AS_END + 1)
#define EV_CMU_END              (EV_CMU_BGN + 999)

/*CU�ڲ���Ϣ��15001-16000��*/
#define EV_CU_BGN               (EV_CMU_END + 1)
#define EV_CU_END               (EV_CU_BGN + 999)

/*NRU�ڲ���Ϣ��16001-17000��*/
#define EV_NRU_BGN              (EV_CU_END + 1)
#define EV_NRU_END              (EV_NRU_BGN + 999)

/*VTDU�ڲ���Ϣ��17001-18000��*/
#define EV_VTDU_BGN             (EV_NRU_END + 1)
#define EV_VTDU_END             (EV_VTDU_BGN + 999)

/*PU�ڲ���Ϣ��18001-19000��*/
#define EV_PU_BGN               (EV_VTDU_END + 1)
#define EV_PU_END               (EV_PU_BGN + 999)

/*UAS�ڲ���Ϣ��19001-20000��*/
#define EV_UAS_BGN              (EV_PU_END + 1)
#define EV_UAS_END              (EV_UAS_BGN + 999)

/*CUI�ڲ���Ϣ��20001-21000��*/
#define EV_CUI_BGN              (EV_UAS_END + 1)
#define EV_CUI_END              (EV_CUI_BGN + 999)

/*PUI�ڲ���Ϣ��21001-22000��*/
#define EV_PUI_BGN              (EV_CUI_END + 1)
#define EV_PUI_END              (EV_PUI_BGN + 999)

/*VPU�ڲ���Ϣ��22001-23000��*/
#define EV_VPU_BGN              (EV_PUI_END + 1)
#define EV_VPU_END              (EV_VPU_BGN + 999)

/*add by aaron 2006.03.28*/
/*internel message of guard module(23001-24000)*/
#define EV_GUARD_BGN            (EV_VPU_END + 1)
#define EV_GUARD_END            (EV_GUARD_BGN + 999)
/*end by aaron*/

/*add by aaron 2006.10.26*/
/*internel message of guard module(24001-25000)*/
#define EV_WATCHDOG_BGN            (EV_GUARD_END + 1)
#define EV_WATCHDOG_END            (EV_WATCHDOG_BGN + 999)
/*end by aaron*/

/*add by aaron 2006.03.28*/
/*internel message of guard module*/
/*internel message of guard module(25001-26000)*/
#define EV_LADEN_BGN            (EV_WATCHDOG_END + 1)
#define EV_LADEN_END            (EV_LADEN_BGN    + 999)
/*end by aaron*/

/*add by yangxin 2007.12.05*/
/* pms module(26001-27000)*/
#define EV_PMS_BGN            (EV_LADEN_END + 1)
#define EV_PMS_END            (EV_PMS_BGN    + 999)
/*end by yangxin*/

//IDS Ϊ���ֺ���Ӫһ��,�����31001��ʼ��35000
#define EV_IDS_BGN						(31001)
#define EV_IDS_END						(EV_IDS_BGN + 3999)
//////////////////////////////////////////////////////////////////////////
//  ��ģ�����Ϣ
// 
/*NMC��NMS�ӿ���Ϣ��40001-41000��*/
#define EV_NMC_NMS_BGN          (EV_KDM_OUTTER_BASE + 1)
#define EV_NMC_NMS_END          (EV_NMC_NMS_BGN + 999)

/*NMS��3AS�ӿ���Ϣ��41001-42000��*/
#define EV_NMS_3AS_BGN          (EV_NMC_NMS_END + 1)
#define EV_NMS_3AS_END          (EV_NMS_3AS_BGN + 999)

/*3AC��3AS�ӿ���Ϣ��42001-43000��*/
#define EV_3AC_3AS_BGN          (EV_NMS_3AS_END + 1)
#define EV_3AC_3AS_END          (EV_3AC_3AS_BGN + 999)

/*CUI��CU�ӿ���Ϣ��43001-44000��*/
#define EV_CUI_CU_BGN           (EV_3AC_3AS_END + 1)
#define EV_CUI_CU_END           (EV_CUI_CU_BGN + 999)

/*CMU��NRU�ӿ���Ϣ��44001-45000��*/
#define EV_CMU_NRU_BGN          (EV_CUI_CU_END + 1)
#define EV_CMU_NRU_END          (EV_CMU_NRU_BGN + 999)

/*CMU��VTDU�ӿ���Ϣ��45001-46000��*/
#define EV_CMU_VTDU_BGN         (EV_CMU_NRU_END + 1)
#define EV_CMU_VTDU_END         (EV_CMU_VTDU_BGN + 999)

/*CMU��UAS�ӿ���Ϣ��46001-47000��*/
#define EV_CMU_UAS_BGN          (EV_CMU_VTDU_END + 1)
#define EV_CMU_UAS_END          (EV_CMU_UAS_BGN + 999)

/*PUI��PU�ӿ���Ϣ��47001-48000��*/
#define EV_PUI_PU_BGN           (EV_CMU_UAS_END + 1)
#define EV_PUI_PU_END           (EV_PUI_PU_BGN + 999)

/*CMU��3AS�ӿ���Ϣ��48001-49000��*/
#define EV_CMU_3AS_BGN          (EV_PUI_PU_END + 1)
#define EV_CMU_3AS_END          (EV_CMU_3AS_BGN + 999)

/*CMU��CUI�ӿ���Ϣ��49001-50000��*/
#define EV_CMU_CUI_BGN          (EV_CMU_3AS_END + 1)
#define EV_CMU_CUI_END          (EV_CMU_CUI_BGN + 999)

/*CMU��PUI�ӿ���Ϣ��50001-51000��*/
#define EV_CMU_PUI_BGN          (EV_CMU_CUI_END + 1)
#define EV_CMU_PUI_END          (EV_CMU_PUI_BGN + 999)

/*CMU��VPU�ӿ���Ϣ��51001-52000��*/
#define EV_CMU_VPU_BGN          (EV_CMU_PUI_END + 1)
#define EV_CMU_VPU_END          (EV_CMU_VPU_BGN + 999)

/*�豸������NMS�ӿ���Ϣ��52001-53000��*/
#define EV_DEVAGT_NMS_BGN       (EV_CMU_VPU_END + 1)
#define EV_DEVAGT_NMS_END       (EV_DEVAGT_NMS_BGN + 999)

//add by ½ѩ��
/*CMU��CMU�����Ϣ(530001-54000)*/
#define EV_CMU_CMU_BGN      (EV_DEVAGT_NMS_END + 1)
#define EV_CMU_CMU_END      (EV_CMU_CMU_BGN + 999)

/*ģ�������ģ����Ϣ(54001-55000)*/
#define EV_MODULE_GUARD_BGN     (EV_CMU_CMU_END + 1)
#define EV_MODULE_GUARD_END     (EV_MODULE_GUARD_BGN + 999)
//end of adding by ½ѩ��

// add by panyf
/*ģ���ǰ��ģ����Ϣ(55001-56000)*/
#define EV_MT_BGN       (EV_MODULE_GUARD_END + 1)
#define EV_MT_END       (EV_MT_BGN + 999)
// end by panyf

/*add by suncy 2007.05.11 (56001-57000)*/
#define EV_CMU_LADEN_BGN            (EV_MT_END           + 1)
#define EV_CMU_LADEN_END            (EV_CMU_LADEN_BGN    + 999)
/*end by suncy*/

/*add by yangxin 2007.12.05 (57001-58000)*/
#define EV_PMS_PMC_BGN              (EV_CMU_LADEN_END  + 1)
#define EV_PMS_PMC_END              (EV_PMS_PMC_BGN    + 799)

#define EV_KDM2800_ERROR_REPORT_BGN (EV_CMU_LADEN_END  + 800)
#define EV_KDM2800_ERROR_REPORT_END (EV_KDM2800_ERROR_REPORT_BGN + 999)
/*end by yangxin*/

/*add by huangxl 2007.12.6 (59001-60000)*/
#define  EV_3AS_3AS_BGN           (EV_PMS_PMC_END  +1)
#define  EV_3AS_3AS_END           (EV_3AS_3AS_BGN + 999)
/*add by linlifen 2009.03.03*/
#define EV_CMU_MTU_BGN          (EV_3AS_3AS_END + 1)
#define EV_CMU_MTU_END          (EV_CMU_MTU_BGN + 999)
/*end by linlifen*/
#endif /* _EVENT_ID_H_ */

/* end of file eventid.h */


