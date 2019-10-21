/*=======================================================================
模块名      :全球眼
文件名      : eventid.h
相关文件    :
文件实现功能 :
作者        : 李屹
版本        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2002/03/01  0.9         李屹        创建
2004/03/11  3.0         魏治兵      增加地址簿消息
2006/02/22  3.9         李洪强      由视频产品修改为全球眼的内容
=======================================================================*/
#ifndef _EVENT_ID_H_
#define _EVENT_ID_H_

#define EV_KDM_BASE     10000

// 各模块内部消息
#define EV_KDM_INNER_BASE   (EV_KDM_BASE + 10000)
// 各模块外部消息
#define EV_KDM_OUTTER_BASE  (EV_KDM_INNER_BASE + 20000)

//////////////////////////////////////////////////////////////////////////
//  各模块内部消息
// 
/*NMC内部消息（10001-11000）*/
#define EV_NMC_BGN      (EV_KDM_BASE + 1)
#define EV_NMC_END      (EV_NMC_BGN + 999)

/*NMS内部消息（11001-12000）*/
#define EV_NMS_BGN      (EV_NMC_END + 1)
#define EV_NMS_END      (EV_NMS_BGN + 999)

/*3AC内部消息（12001-13000）*/
#define EV_3AC_BGN              (EV_NMS_END + 1)
#define EV_3AC_END              (EV_3AC_BGN + 999)

/*3AS内部消息（13001-14000）*/
#define EV_3AS_BGN              (EV_3AC_END + 1)
#define EV_3AS_END              (EV_3AS_BGN + 999)

/*CMU内部消息（14001-15000）*/
#define EV_CMU_BGN              (EV_3AS_END + 1)
#define EV_CMU_END              (EV_CMU_BGN + 999)

/*CU内部消息（15001-16000）*/
#define EV_CU_BGN               (EV_CMU_END + 1)
#define EV_CU_END               (EV_CU_BGN + 999)

/*NRU内部消息（16001-17000）*/
#define EV_NRU_BGN              (EV_CU_END + 1)
#define EV_NRU_END              (EV_NRU_BGN + 999)

/*VTDU内部消息（17001-18000）*/
#define EV_VTDU_BGN             (EV_NRU_END + 1)
#define EV_VTDU_END             (EV_VTDU_BGN + 999)

/*PU内部消息（18001-19000）*/
#define EV_PU_BGN               (EV_VTDU_END + 1)
#define EV_PU_END               (EV_PU_BGN + 999)

/*UAS内部消息（19001-20000）*/
#define EV_UAS_BGN              (EV_PU_END + 1)
#define EV_UAS_END              (EV_UAS_BGN + 999)

/*CUI内部消息（20001-21000）*/
#define EV_CUI_BGN              (EV_UAS_END + 1)
#define EV_CUI_END              (EV_CUI_BGN + 999)

/*PUI内部消息（21001-22000）*/
#define EV_PUI_BGN              (EV_CUI_END + 1)
#define EV_PUI_END              (EV_PUI_BGN + 999)

/*VPU内部消息（22001-23000）*/
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

//IDS 为保持和运营一致,这里从31001开始到35000
#define EV_IDS_BGN						(31001)
#define EV_IDS_END						(EV_IDS_BGN + 3999)
//////////////////////////////////////////////////////////////////////////
//  各模块间消息
// 
/*NMC与NMS接口消息（40001-41000）*/
#define EV_NMC_NMS_BGN          (EV_KDM_OUTTER_BASE + 1)
#define EV_NMC_NMS_END          (EV_NMC_NMS_BGN + 999)

/*NMS与3AS接口消息（41001-42000）*/
#define EV_NMS_3AS_BGN          (EV_NMC_NMS_END + 1)
#define EV_NMS_3AS_END          (EV_NMS_3AS_BGN + 999)

/*3AC与3AS接口消息（42001-43000）*/
#define EV_3AC_3AS_BGN          (EV_NMS_3AS_END + 1)
#define EV_3AC_3AS_END          (EV_3AC_3AS_BGN + 999)

/*CUI与CU接口消息（43001-44000）*/
#define EV_CUI_CU_BGN           (EV_3AC_3AS_END + 1)
#define EV_CUI_CU_END           (EV_CUI_CU_BGN + 999)

/*CMU与NRU接口消息（44001-45000）*/
#define EV_CMU_NRU_BGN          (EV_CUI_CU_END + 1)
#define EV_CMU_NRU_END          (EV_CMU_NRU_BGN + 999)

/*CMU与VTDU接口消息（45001-46000）*/
#define EV_CMU_VTDU_BGN         (EV_CMU_NRU_END + 1)
#define EV_CMU_VTDU_END         (EV_CMU_VTDU_BGN + 999)

/*CMU与UAS接口消息（46001-47000）*/
#define EV_CMU_UAS_BGN          (EV_CMU_VTDU_END + 1)
#define EV_CMU_UAS_END          (EV_CMU_UAS_BGN + 999)

/*PUI与PU接口消息（47001-48000）*/
#define EV_PUI_PU_BGN           (EV_CMU_UAS_END + 1)
#define EV_PUI_PU_END           (EV_PUI_PU_BGN + 999)

/*CMU与3AS接口消息（48001-49000）*/
#define EV_CMU_3AS_BGN          (EV_PUI_PU_END + 1)
#define EV_CMU_3AS_END          (EV_CMU_3AS_BGN + 999)

/*CMU与CUI接口消息（49001-50000）*/
#define EV_CMU_CUI_BGN          (EV_CMU_3AS_END + 1)
#define EV_CMU_CUI_END          (EV_CMU_CUI_BGN + 999)

/*CMU与PUI接口消息（50001-51000）*/
#define EV_CMU_PUI_BGN          (EV_CMU_CUI_END + 1)
#define EV_CMU_PUI_END          (EV_CMU_PUI_BGN + 999)

/*CMU与VPU接口消息（51001-52000）*/
#define EV_CMU_VPU_BGN          (EV_CMU_PUI_END + 1)
#define EV_CMU_VPU_END          (EV_CMU_VPU_BGN + 999)

/*设备代理与NMS接口消息（52001-53000）*/
#define EV_DEVAGT_NMS_BGN       (EV_CMU_VPU_END + 1)
#define EV_DEVAGT_NMS_END       (EV_DEVAGT_NMS_BGN + 999)

//add by 陆雪忠
/*CMU和CMU间的消息(530001-54000)*/
#define EV_CMU_CMU_BGN      (EV_DEVAGT_NMS_END + 1)
#define EV_CMU_CMU_END      (EV_CMU_CMU_BGN + 999)

/*模块和守卫模块消息(54001-55000)*/
#define EV_MODULE_GUARD_BGN     (EV_CMU_CMU_END + 1)
#define EV_MODULE_GUARD_END     (EV_MODULE_GUARD_BGN + 999)
//end of adding by 陆雪忠

// add by panyf
/*模块和前端模块消息(55001-56000)*/
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


