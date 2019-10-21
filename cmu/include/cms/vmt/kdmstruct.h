/*****************************************************************************
   模块名      : KDM
   文件名      : kdmsturst.h
   相关文件    : 
   文件实现功能: KDM公用接口的结构定义头文件
   作者        : 万春雷
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2006/02/20  1.0         万春雷        创建
   2006/04/30  1.0         万春雷        公用类的定义与实现分开
   2006/09/07  1.0         万春雷        修改编号规则，在原有8层域的基础上，
                                         使用18位域编号中的末位2位作为行业序号
                                         一起作为域节点编号
******************************************************************************/

#ifndef _STRUCT_KDM_H_
#define _STRUCT_KDM_H_

/////////////////////////////////////////////////////////////////////////////////

#include "kdmconst.h"

#ifdef WIN32
#pragma comment( lib, "ws2_32.lib" ) 
#define window( x )	x
#else
#include <netinet/in.h>
#define window( x )
#endif

//...........................................
#ifdef WIN32
#pragma pack( push )
#pragma pack( 1 )
#ifndef PACKED
#define PACKED
#endif
#endif//.....................................

//...........................................
#ifdef _VXWORKS_
#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif
#endif//.....................................

//...........................................
#ifdef _LINUX_
#ifndef PACKED
#define PACKED __attribute__ ((packed)) 
#endif
#ifndef NULL
#define NULL 0
#endif
#endif//.....................................

#ifdef WIN32
#ifndef INLINEFLAG
#define INLINEFLAG inline
#endif
#else
#ifndef INLINEFLAG
#define INLINEFLAG inline
#endif
#endif

//1. 全局编号
struct TKDMNO
{
public:
	TKDMNO( void );
	void SetNull();//置空
	BOOL32 IsNull() const; 

	BOOL32 operator == ( const TKDMNO &tObj ) const;
	BOOL32 operator != ( const TKDMNO &tObj ) const;
	
	void operator = ( const TKDMNO &tObj );
	
	//add by wdl
	BOOL32 SetKdmNO(const TKDMNO& tKdmNo);
	const TKDMNO GetKdmNo() const { return (*this); }
	
	u8  GetNOType( void ) const;     //获取编号类型, 参见常量定义1.1.2
	u8  GetTradeNO( void ) const;    //获取扩展的行业序号, 参见常量定义
	u8  GetCPeriType( void ) const;  //获取平台设备类型, 参见常量定义4.1
	u8  GetCPeriExType( void ) const; //获取平台设备编号扩展类型
	u8  GetPUType( void ) const;     //获取前端设备类型, 参见常量定义4.2
	u8  GetPUMType( void ) const;    //获取前端子设备的主设备类型, 参见常量定义4.2.1
	u8  GetPUPType( void ) const;    //获取前端子设备的外围设备类型, 参见常量定义4.2.2

	s32 GetDomainLevel( void ) const;//获取编号所在域的层级,最大8层, [建议3A内部使用]
	TKDMNO GetCmuNO( void ) const;   //得到所处的[域/CMU平台]编号
    TKDMNO GetTopCmuNO( void ) const;   //得到所在[域/CMU平台]的顶级[域/CMU平台]编号，即是CMU的顶级CMU
	TKDMNO GetMCmuNO( void ) const;  //得到所在[域/CMU平台]的父[域/CMU平台]编号，即是CMU的上级CMU
	BOOL32 IsMCmuNO(const TKDMNO &tMCMUId) const;    //是否为父[域/CMU平台]
	//wdl 06.03.20
	BOOL32 IsSCmuNO(const TKDMNO &tSCMUId) const;    //是否为子[域/CMU平台]
 	TKDMNO GetNextCmu(const TKDMNO &tDstCMUId) const;//得到下一站CMU
	BOOL32 IsCmuInPath(const TKDMNO &tSrcCMUId,const TKDMNO &tDstCMUId) const;
    BOOL32 IsCmuDown(const TKDMNO &tDstCmuId) const;
    TKDMNO GetSameCmu(const TKDMNO &tDstCmuId) const;

	TKDMNO GetCustomerNO( void ) const;    //得到客户用户/客户用户组/客户设备组 所属的[客户]编号
	
	s32 GetSN( void ) const;               //获取编号的序号, [建议3A内部使用]

	u8  GetDeviceGlobalType( void ) const; //获取设备统一类型, 参见常量定义5.

	//获取编号字串
	const s8* GetNOString() const { return m_achNO; }
	//设置编号字串
	BOOL32 SetNOString(const s8* pachString);
	
	const s8* GetStrIdType();//获取设备类型描述字串

protected:
	//将指定的字符数组转换为相应的数字, [TKDMNO结构内部使用]
	s32 StringToNONum(const s8* pachString, s32 nStringLen) const;
	
	//将指定的数字转换为相应的字符数组,数字长度小于nStringLen则在头部补零, [TKDMNO结构内部使用]
	BOOL32 NumToNOString(s32 nNONum, s8* pachString, s32 nStringLen) const;

protected:
	s8  m_achNO[LEN_KDM_NO+1]; //每一字节为十进制数值, 

//如顶级CMU编号有效18位,整体为              [99,00,00,00,00,00,00,00,00], [5], 0,00,00,00,00,00,00
//  顶级CMU下的第一个DVR编号有效27位,整体为 [99,00,00,00,00,00,00,00,00], [1],[2], [1],[0,00,00,1]0,00,00
//  顶级CMU下的第一个NRU编号有效23位,整体为 [99,00,00,00,00,00,00,00,00], [2],[2], [00, 1]0,00,00,00,00
//  顶级CMU下的第一个客户编号有效24位,整体为[99,00,00,00,00,00,00,00,00], [3],[0,00,01],00,00,00,00
}PACKED;

	
//2. <日期＋时间>结构
struct TKDMTime
{
public:
	TKDMTime( void );
	TKDMTime(time_t tTime);
	
	void SetNull();//置空
	BOOL32 IsNull() const;

	void SetTime( const time_t *ptTime ); //设置时间
	void GetTime( time_t &tTime ) const;  //得到时间结构    
	time_t GetTime(void) const;           //得到时间结构

	BOOL32 operator == ( const TKDMTime &tObj ) const;
	void operator = ( const TKDMTime &tObj );
	//一个有效时间与待检测的时间线作比较
	BOOL32 operator > ( const TKDMTime &tTimeLine ) const;

	//获取时间对应的格式字串
	BOOL32 GetString(s8* pachString, s32 nStringLen) const;
	
	BOOL32 SetYear( u16 wYear/*主机序*/ );
	BOOL32 SetMonth( u8 byMonth );
	BOOL32 SetDay( u8 byDay );
	BOOL32 SetHour( u8 byHour );
	BOOL32 SetMinute( u8 byMinute );
	BOOL32 SetSecond( u8 bySecond );
	u16  GetYear( void ) const    { return ntohs(m_wYear); }//返回主机序
	u8   GetMonth( void ) const   { return m_byMonth; }
	u8   GetDay( void ) const     { return m_byMDay; }
	u8   GetHour( void ) const    { return m_byHour; }
	u8   GetMinute( void ) const  { return m_byMinute; }
	u8   GetSecond( void ) const  { return m_bySecond; }

	time_t GetDayTime() const;
	s32 GetWeeklyInfo() const;
	BOOL32 IsTimeIn(u32 dwPreTime, u32 dwNextTime) const;
	BOOL32 IsTimeIn(const TKDMTime& tPreTime, const TKDMTime& tNextTime) const;
protected:	
	u16  m_wYear;   //年(网络序)
	u8   m_byMonth; //月
	u8   m_byMDay;  //日
	u8   m_byHour;  //时
	u8   m_byMinute;//分
	u8   m_bySecond;//秒
}PACKED;

//3. 版本信息(KDM.Version)
struct TKDMVersion
{
public:
	TKDMVersion( void );
	void SetNull();//置空
	
	BOOL32 SetVersion( const s8 *pachVersion );
	const s8* GetVersion( void ) const { return m_achVersion; }

	BOOL32 operator == ( const TKDMVersion &tObj ) const;
	void operator = ( const TKDMVersion &tObj );
	
protected:
	s8   m_achVersion[MAXLEN_VERSION_STRING+1];//版本信息字串
}PACKED;

//4. 加密器
class CEncryptor
{
public:
    CEncryptor( void );
    virtual ~CEncryptor();
    // 从pachString中取出dwBitPos位置Bit的数值
    static BOOL32 GetBitAt(const s8* pachString, u32 dwBitPos);
    // 从pachString中设置dwBitPos位置Bit的数值
    static void SetBitAt(s8* pachString, u32 dwBitPos, BOOL32 bSetToOne);
    // 加密。把[pachPlainText+wPlainSize]指定的密文转化为[pachCipherText+wCipherSize]指定的明文
    static BOOL32 EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize);
    // 解密。把[pachCipherText+wCipherSize]指定的密文转化为[pachPlainText+wPlainSize]指定的明文
    static BOOL32 DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize);
};
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//部分公用类的实现部分

/*=============================================================================
    函 数 名： TKDMNO
    功    能： 缺省构造
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值：  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMNO::TKDMNO( void )
{
	SetNull();
}

/*=============================================================================
    函 数 名： SetNull
    功    能： 置空
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMNO::SetNull()
{
	memset(m_achNO, NOFIELD_CHAR_INVALID, LEN_KDM_NO);
	m_achNO[LEN_KDM_NO] = '\0';
}

/*=============================================================================
    函 数 名： IsNull
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::IsNull() const
{
	u8 byNOType = GetNOType();
	if (byNOType >= NOFIELD_TYPE_PRECEDINGDEVICE && 
		byNOType <= NOFIELD_TYPE_CUGROUP)
	{
		return FALSE;
	}
	return TRUE;
}

/*=============================================================================
    函 数 名： operator == 
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tObj
    返 回 值： BOOL32
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::operator == ( const TKDMNO &tObj ) const
{
	if (0 == memcmp(m_achNO, tObj.m_achNO, LEN_KDM_NO))
	{
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
    函 数 名： operator != 
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tObj
    返 回 值： BOOL32
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::operator != ( const TKDMNO &tObj ) const
{
	if (0 != memcmp(m_achNO, tObj.m_achNO, LEN_KDM_NO))
	{
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
    函 数 名： operator = 
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tObj
    返 回 值： void
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMNO::operator = ( const TKDMNO &tObj )
{
	memcpy((s8*)m_achNO, (s8*)tObj.m_achNO, LEN_KDM_NO);
	m_achNO[LEN_KDM_NO] = '\0';
}

/*=============================================================================
    函 数 名： SetKdmNO
    功    能： 
    算法实现： 
    全局变量： 
    参    数： const TKDMNO& tKdmNo
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			wdl                     创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::SetKdmNO(const TKDMNO& tKdmNo)
{
	return SetNOString(tKdmNo.GetNOString());
}

/*=============================================================================
    函 数 名： GetNOType
    功    能： 获取编号类型, 参见常量定义1.1.2
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetNOType( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS], FIELD_LEN_NOTYPE));
}

/*=============================================================================
    函 数 名： GetTradeNO
    功    能： 获取扩展的行业序号
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/9/7    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetTradeNO( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_DOMAIN], FIELD_LEN_SINGLE_DOMAIN));
}

/*=============================================================================
    函 数 名： GetCPeriType
    功    能： 获取平台设备类型, 参见常量定义4.1
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetCPeriType( void ) const
{
	u8 byType = DEVICE_TYPE_INVALID;
	if (NOFIELD_TYPE_PLATFORMDEVICE == GetNOType())
	{
		byType = (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
				                    FIELD_LEN_CDTYPE));
	}

	// 如果是扩展平台，则获取具体扩展类型
	if (PLATFORM_DEVICE_TYPE_EX == byType)
	{
		byType = GetCPeriExType();
	}

	return byType;
}

/*=============================================================================
    函 数 名： GetPUType
    功    能： 获取前端设备类型, 参见常量定义4.2
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetPUType( void ) const
{
	u8 byType = DEVICE_TYPE_INVALID;
	if (NOFIELD_TYPE_PRECEDINGDEVICE == GetNOType())
	{
		byType = (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
				                    FIELD_LEN_PDTYPE));
	}
	return byType;
}

/*=============================================================================
    函 数 名： GetPUMType
    功    能： 获取前端子设备的主设备类型, 参见常量定义4.2.1
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetPUMType( void ) const
{
	u8 byType = DEVICE_TYPE_INVALID;
	if (PRECEDING_DEVICE_TYPE_MAIN == GetPUType())
	{
		byType = (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_PDTYPE], 
				                    FIELD_LEN_PDSUBTYPE));
	}
	return byType;
}

/*=============================================================================
    函 数 名： GetPUPType
    功    能： 获取前端子设备的外围设备类型, 参见常量定义4.2.2
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetPUPType( void ) const
{
	u8 byType = DEVICE_TYPE_INVALID;
	if (PRECEDING_DEVICE_TYPE_PER == GetPUType())
	{
		byType = (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_PDTYPE], 
				                    FIELD_LEN_PDSUBTYPE));				
	}
	return byType;
}

/*=============================================================================
    函 数 名： GetCPeriExType
    功    能： 获取平台设备编号扩展类型
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u8 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2008/8/22   1.0			韩旺                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetCPeriExType( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+
		FIELD_LEN_CDTYPE+FIELD_LEN_CDSN], FIELD_LEN_CDTYPE_EX));
}

/*=============================================================================
    函 数 名： GetDomainLevel
    功    能： 获取编号所在域的层级,最大8层, [建议3A内部使用]
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： s32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG s32 TKDMNO::GetDomainLevel( void ) const
{
	s32 nLevel = 0;
	if (NOFIELD_TYPE_INVALID != GetNOType())
	{
		for (u8 byLoop=MAXNUM_DOMAIN_LEVEL; byLoop>0; byLoop--)
		{
			if (0 != memcmp((s8*)&m_achNO[(byLoop-1)*FIELD_LEN_SINGLE_DOMAIN], 
					        "00", 
							FIELD_LEN_SINGLE_DOMAIN))
			{
				nLevel = (s32)byLoop;
				break;
			}
		}
	}
	return nLevel;
}

/*=============================================================================
    函 数 名： GetCmuNO
    功    能： 得到所处的[域/CMU平台]编号
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： TKDMNO 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMNO TKDMNO::GetCmuNO( void ) const
{
	TKDMNO tCmuId;
	tCmuId.SetNull();
	if (NOFIELD_TYPE_INVALID != GetNOType())
	{
		s8  achBuf[FIELD_LEN_NOTYPE+1] = {'\0'};
		NumToNOString(NOFIELD_TYPE_DOMAINORCMU, achBuf, FIELD_LEN_NOTYPE);
		memcpy(tCmuId.m_achNO, m_achNO, FIELD_LEN_CMS);	
		memcpy((s8*)&tCmuId.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);
	}
	return tCmuId;
}

/*=============================================================================
    函 数 名： GetTopCmuNO
    功    能： 得到所在[域/CMU平台]的顶级[域/CMU平台]编号，即是CMU的顶级CMU
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： TKDMNO 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/9/13   1.0			陆雪忠                  创建
=============================================================================*/
INLINEFLAG TKDMNO TKDMNO::GetTopCmuNO( void ) const
{
	TKDMNO tTopCmuId;
	tTopCmuId.SetNull();
    
	s32 nLevel = GetDomainLevel();
	if (NOFIELD_TYPE_DOMAINORCMU == GetNOType() && nLevel > 0)
	{
		s8  achBuf[FIELD_LEN_NOTYPE+1] = {'\0'};
		NumToNOString(NOFIELD_TYPE_DOMAINORCMU, achBuf, FIELD_LEN_NOTYPE);
		memcpy(tTopCmuId.m_achNO, m_achNO, FIELD_LEN_SINGLE_DOMAIN);
		memcpy((s8*)&tTopCmuId.m_achNO[FIELD_LEN_DOMAIN], (s8*)&m_achNO[FIELD_LEN_DOMAIN], FIELD_LEN_CMU);
		memcpy((s8*)&tTopCmuId.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);
	}
	return tTopCmuId;
}

/*=============================================================================
    函 数 名： GetMCmuNO
    功    能： 得到所在[域/CMU平台]的父[域/CMU平台]编号，即是CMU的上级CMU
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： TKDMNO 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMNO TKDMNO::GetMCmuNO( void ) const
{
	TKDMNO tMCmuId;
	tMCmuId.SetNull();
	if (NOFIELD_TYPE_DOMAINORCMU == GetNOType())
	{
		s32 nLevel = GetDomainLevel();
		s8  achBuf[FIELD_LEN_NOTYPE+1] = {'\0'};
		NumToNOString(NOFIELD_TYPE_DOMAINORCMU, achBuf, FIELD_LEN_NOTYPE);
		if (nLevel > 1)
		{
			memcpy(tMCmuId.m_achNO, m_achNO, (nLevel-1)*FIELD_LEN_SINGLE_DOMAIN);
			//2006-09-07 拷贝行业序号
			memcpy((s8*)&tMCmuId.m_achNO[FIELD_LEN_DOMAIN], 
				   (s8*)&m_achNO[FIELD_LEN_DOMAIN], FIELD_LEN_CMU);
			memcpy((s8*)&tMCmuId.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);
		}
	}
	return tMCmuId;
}

/*=============================================================================
    函 数 名： IsMCmuNO
    功    能： 是否为父[域/CMU平台]
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tMCMUId
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::IsMCmuNO(const TKDMNO &tMCMUId) const
{
	BOOL32 bRet = FALSE;
	TKDMNO tCmuId = GetMCmuNO();
	if (FALSE == tCmuId.IsNull() && tCmuId == tMCMUId)
	{
		bRet = TRUE;
	}
	return bRet;
}

/*=============================================================================
    函 数 名： IsSCmuNO
    功    能： 是否为子[域/CMU平台]
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tSCMUId
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/20   1.0			wdl                     创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::IsSCmuNO(const TKDMNO &tSCMUId) const
{
	BOOL32 bRet = FALSE;
	TKDMNO tTempSCMUId = tSCMUId;
	TKDMNO tCmuId = tTempSCMUId.GetMCmuNO();
	if (FALSE == tCmuId.IsNull() && 
		0 == memcmp(m_achNO, tCmuId.m_achNO, LEN_KDM_NO))
	{
		bRet = TRUE;
	}
	return bRet;
}

/*=============================================================================
    函 数 名： GetNextCmu
    功    能： 得到下一站CMU
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tDstCMUId
    返 回 值： TKDMNO 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			wdl                     创建
=============================================================================*/
INLINEFLAG TKDMNO TKDMNO::GetNextCmu(const TKDMNO &tDstCMUId) const
{
    if(GetTradeNO() == tDstCMUId.GetTradeNO())
    {
        s32 nDstLevel = tDstCMUId.GetDomainLevel();
        s32 nLevel    = GetDomainLevel();
        if(nDstLevel==nLevel&&tDstCMUId.GetCmuNO()==GetCmuNO())
        {
            return GetCmuNO();
        }
        else if(nDstLevel==nLevel&&tDstCMUId.GetCmuNO()!=GetCmuNO())
        {
            if(nDstLevel != 1)
            {
                return GetMCmuNO();
            }
            else
            {
                return tDstCMUId;
            }
        }
        else if(nDstLevel<nLevel)
        {
            return GetMCmuNO();
        }
        else
        {
            if(GetTopCmuNO() == tDstCMUId.GetTopCmuNO())
            {
                if(memcmp(m_achNO,tDstCMUId.m_achNO,nLevel*FIELD_LEN_SINGLE_DOMAIN)==0)
                {
                    TKDMNO tMCmuId;
                    tMCmuId=tDstCMUId;
                    if (NOFIELD_TYPE_INVALID != GetNOType())
                    {
                        for (u8 byLoop=MAXNUM_DOMAIN_LEVEL; byLoop>nLevel+1; byLoop--)
                        {
                            memcpy((s8*)&tMCmuId.m_achNO[(byLoop-1)*FIELD_LEN_SINGLE_DOMAIN], 
                                "00", FIELD_LEN_SINGLE_DOMAIN);
                        }
                    }	
                    return tMCmuId;
                }
                else
                {
                    return GetMCmuNO();
                }
            }
            else
            {
                if(GetTopCmuNO() == GetCmuNO())
                {
                    return tDstCMUId.GetTopCmuNO();
                }
                else
                {
                    return GetMCmuNO();
                }
            }
        }
    }
    else
    {
        if(GetTopCmuNO() == GetCmuNO())
        {
            return tDstCMUId.GetTopCmuNO();
        }
        else
        {
            return GetMCmuNO();
        }
    }
}

INLINEFLAG TKDMNO TKDMNO::GetSameCmu(const TKDMNO &tDstCmuId) const
{    
    s32 sdwDstLevel = tDstCmuId.GetDomainLevel();
    s32 sdwLocalLevel = GetDomainLevel();

    s32 sdwLevel = 0;
    if(sdwLocalLevel < sdwDstLevel)
        sdwLevel = sdwLocalLevel;
    else
        sdwLevel = sdwDstLevel;

    TKDMNO tCmuId;
    
    s8  achBuf[FIELD_LEN_NOTYPE+1] = {'\0'};
	NumToNOString(NOFIELD_TYPE_DOMAINORCMU, achBuf, FIELD_LEN_NOTYPE);

    for(s32 sdwIndex = 0; sdwIndex < sdwLevel; sdwIndex++)
    {
        if(memcmp(m_achNO, tDstCmuId.m_achNO, (sdwIndex + 1) * FIELD_LEN_SINGLE_DOMAIN) != 0)
            break;
        
        memcpy(tCmuId.m_achNO, m_achNO, (sdwIndex + 1) * FIELD_LEN_SINGLE_DOMAIN);
    }
	memcpy((s8*)&tCmuId.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);

    return tCmuId;
}

INLINEFLAG BOOL32 TKDMNO::IsCmuDown(const TKDMNO &tDownCmuId) const
{
    if(GetTradeNO() != tDownCmuId.GetTradeNO())
        return FALSE;

    s32 sdwDstLevel = tDownCmuId.GetDomainLevel();
    s32 sdwLocalLevel = GetDomainLevel();

    if(sdwLocalLevel < sdwDstLevel)
    {
        return memcmp(m_achNO, tDownCmuId.m_achNO, sdwLocalLevel * FIELD_LEN_SINGLE_DOMAIN) == 0;
    }
    return FALSE;
}

/*=============================================================================
    函 数 名： IsCmuInPath
    功    能： 判别CMU是否在源CMU到目的CMU的路径上
    算法实现： 
    全局变量： 
    参    数： const TKDMNO &tSrcCMUId
               const TKDMNO &tDstCMUId
    返 回 值： BO0L32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/13 1.0			王冬林              创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::IsCmuInPath(const TKDMNO &tSrcCMUId,const TKDMNO &tDstCMUId) const
{
    if(GetCmuNO()==tSrcCMUId || GetCmuNO()==tDstCMUId)
		return TRUE;
    
    if(GetTradeNO() != tSrcCMUId.GetTradeNO() && GetTradeNO() != tDstCMUId.GetTradeNO())
        return FALSE;

    if(GetTradeNO() == tSrcCMUId.GetTradeNO() && GetTradeNO() == tDstCMUId.GetTradeNO())
    {
        if(tSrcCMUId.IsCmuDown(GetCmuNO()) && IsCmuDown(tDstCMUId))
            return TRUE;

        if(IsCmuDown(tSrcCMUId) && tDstCMUId.IsCmuDown(GetCmuNO()))
            return TRUE;

        TKDMNO tSameCmuId = tSrcCMUId.GetSameCmu(tDstCMUId);
        if(tSameCmuId == GetCmuNO())
            return TRUE;

        if(tSameCmuId.GetDomainLevel() != 0)
        {
            if(tSameCmuId.IsCmuDown(GetCmuNO()) && (IsCmuDown(tSrcCMUId) || IsCmuDown(tDstCMUId)))
                return TRUE;   
        }
        else
        {
            if(IsCmuDown(tSrcCMUId) || IsCmuDown(tDstCMUId))
                return TRUE;
        } 
            
        return FALSE;
    }
    else
    {
        if(IsCmuDown(tSrcCMUId) || IsCmuDown(tDstCMUId))
            return TRUE;

        return FALSE;
    }
}

/*=============================================================================
    函 数 名： GetCustomerNO
    功    能： 得到客户用户/客户用户组/客户设备组 所属的[客户]编号
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： TKDMNO 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMNO TKDMNO::GetCustomerNO( void ) const
{
	TKDMNO tCustomerNO;
	tCustomerNO.SetNull();
	if (NOFIELD_TYPE_CUSTOMER == GetNOType() || 
		NOFIELD_TYPE_CUSER    == GetNOType() || 
		NOFIELD_TYPE_CDGROUP  == GetNOType() || 
		NOFIELD_TYPE_CUGROUP  == GetNOType())
	{
		s8  achBuf[FIELD_LEN_NOTYPE+1] = {'\0'};
		NumToNOString(NOFIELD_TYPE_CUSTOMER, achBuf, FIELD_LEN_NOTYPE);
		memcpy(tCustomerNO.m_achNO, m_achNO, 
			   FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN);
		memcpy((s8*)&tCustomerNO.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);
	}
	return tCustomerNO;
}

/*=============================================================================
    函 数 名： GetSN
    功    能： 获取编号的序号，注意与设备类型的宏定义同步[建议3A内部使用]
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： s32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG s32 TKDMNO::GetSN( void ) const
{
	s32 nSN = 0;
	u8  byNOType = GetNOType();
	switch(byNOType)
	{
	case NOFIELD_TYPE_PLATFORMDEVICE://平台设备序号
		{
			if (DEVICE_TYPE_INVALID != GetCPeriType())
			{
				nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CDTYPE], 
						            FIELD_LEN_CDSN);
			}
		}
		break;
	case NOFIELD_TYPE_PRECEDINGDEVICE://前端设备序号
		{
			if (DEVICE_TYPE_INVALID != GetPUType()) //&& 
			{
				nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_PDTYPE+FIELD_LEN_PDSUBTYPE], 
						            FIELD_LEN_PDSN);
			}
		}
		break;
	case NOFIELD_TYPE_TUSER://电信用户序号
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
						        FIELD_LEN_TELUSERSN);
		}
		break;
	case NOFIELD_TYPE_DOMAINORCMU://所在域的序号
		{
			s32 nLevel = GetDomainLevel();
			if (nLevel > 0)
			{
				nSN = StringToNONum((s8*)&m_achNO[(nLevel-1)*FIELD_LEN_SINGLE_DOMAIN], 
							        FIELD_LEN_SINGLE_DOMAIN);
			}
		}
		break;
	case NOFIELD_TYPE_CUSTOMER://客户序号
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
						        FIELD_LEN_CUSTOMERSN);
		}
		break;
	case NOFIELD_TYPE_CUSER://客户用户序号
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN], 
						        FIELD_LEN_CUSTOMERUSERSN);
		}
		break;
	case NOFIELD_TYPE_CUGROUP://客户用户组序号
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN], 
						        FIELD_LEN_CUGROUPSN);
		}
		break;
	case NOFIELD_TYPE_CDGROUP://客户设备组序号
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN], 
						        FIELD_LEN_CDGROUPSN);
		}
		break;
	default:
		break;
	}
	return nSN;
}

/*=============================================================================
    函 数 名： GetDeviceGlobalType
    功    能： 获取设备统一类型，注意与设备类型的宏定义同步
    算法实现： 
    全局变量： 
    参    数： void
               
    返 回 值： u8 const
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetDeviceGlobalType( void ) const
{
	u8 byGlobalDeviceType = DEVICE_TYPE_INVALID;
	u8 byNODeviceType = DEVICE_TYPE_INVALID;

	if (NOFIELD_TYPE_DOMAINORCMU == GetNOType())   //CMU...
	{
		byGlobalDeviceType = DEVICE_GLOBAL_TYPE_CMU;
	}
	if (NOFIELD_TYPE_PLATFORMDEVICE == GetNOType())   //平台...
	{
		byNODeviceType = GetCPeriType();              //平台设备类型......
		switch (byNODeviceType) 
		{
		case PLATFORM_DEVICE_TYPE_VTDU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_VTDU;
			break;
		case PLATFORM_DEVICE_TYPE_NRU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_NRU;
			break;
		case PLATFORM_DEVICE_TYPE_VPU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_VPU;
			break;
		case PLATFORM_DEVICE_TYPE_UAS:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_UAS;
			break;
		case PLATFORM_DEVICE_TYPE_PUI:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_PUI;
			break;
		case PLATFORM_DEVICE_TYPE_CUI:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_CUI;
			break;
		case PLATFORM_DEVICE_TYPE_DNS:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_DNS;
			break;
		case PLATFORM_DEVICE_TYPE_EX_NPU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_NPU;
			break;
		case PLATFORM_DEVICE_TYPE_EX_IMU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_IMU;
			break;
		case PLATFORM_DEVICE_TYPE_EX_IAS:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_IAS;
			break;
		case PLATFORM_DEVICE_TYPE_EX_TAS:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_TAS;
			break;
		case PLATFORM_DEVICE_TYPE_EX_MTU:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_MTU;
			break;
		default:
			byGlobalDeviceType = DEVICE_GLOBAL_TYPE_INVALID;
			break;
		}
	}
	if (NOFIELD_TYPE_PRECEDINGDEVICE == GetNOType())  //前端...
	{
		if (PRECEDING_DEVICE_TYPE_MAIN == GetPUType())
		{
			byNODeviceType = GetPUMType();            //前端子设备的主设备类型......
			switch (byNODeviceType) 
			{
			case MAIN_DEVICE_TYPE_DVR:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_DVR;
				break;
			case MAIN_DEVICE_TYPE_ENCODER:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_ENCODER;
				break;
			case MAIN_DEVICE_TYPE_IPCAMERA:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_IPCAMERA;
				break;
			case MAIN_DEVICE_TYPE_DECODER:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_DECODER;
				break;
			default:
				break;
			}
		}
		if (PRECEDING_DEVICE_TYPE_PER == GetPUType())
		{
			byNODeviceType = GetPUPType();            //前端子设备的外围设备类型......
			switch (byNODeviceType) 
			{
			case PER_DEVICE_TYPE_VIDICON:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_VIDICON;
				break;
			case PER_DEVICE_TYPE_YT:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_YT;
				break;
			case PER_DEVICE_TYPE_IOIN:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_IOIN;
				break;
			case PER_DEVICE_TYPE_INOUT:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_INOUT;
				break;
			case PER_DEVICE_TYPE_AIN:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_AIN;
				break;
			case PER_DEVICE_TYPE_AOUT:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_AOUT;
				break;
			case PER_DEVICE_TYPE_MATRIX:
				byGlobalDeviceType = DEVICE_GLOBAL_TYPE_MATRIX;
				break;
			default:
				break;
			}
		}
	}

	return byGlobalDeviceType;
}

/*=============================================================================
    函 数 名： SetNOString
    功    能： 设置编号字串
    算法实现： 
    全局变量： 
    参    数： const s8* pachString
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::SetNOString(const s8* pachString)
{
	if (NULL == pachString)
	{
		return FALSE;
	}
	//wancl
	//注意:3A在编号使用中有通配编号的情况,如"99%",总长度不一定为32
	if (0 == strlen(pachString))
	{
		SetNull();
		return TRUE;
	}
	memcpy(m_achNO, pachString, LEN_KDM_NO);
	m_achNO[LEN_KDM_NO] = '\0';

	return TRUE;
}

/*=============================================================================
    函 数 名： GetStrIdType
    功    能： 获取设备类型描述字串，注意与设备类型的宏定义同步
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： const s8* 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/20   1.0			wdl                     创建
=============================================================================*/
INLINEFLAG const s8* TKDMNO::GetStrIdType()
{
	switch(GetDeviceGlobalType())
	{
	case DEVICE_GLOBAL_TYPE_CMU:   //CMU
		return "DEVICE_GLOBAL_TYPE_CMU";
	case DEVICE_GLOBAL_TYPE_VTDU:  //码流转发
		return "DEVICE_GLOBAL_TYPE_VTDU";
	case DEVICE_GLOBAL_TYPE_NRU:  //网络录放像
		return "DEVICE_GLOBAL_TYPE_NRU";
	case DEVICE_GLOBAL_TYPE_VPU:  //画面复合
		return "DEVICE_GLOBAL_TYPE_VPU";
	case DEVICE_GLOBAL_TYPE_UAS:  //CMU的个性化数据服务器
		return "DEVICE_GLOBAL_TYPE_UAS";
	case DEVICE_GLOBAL_TYPE_PUI:  //PU接入
		return "DEVICE_GLOBAL_TYPE_PUI";
	case DEVICE_GLOBAL_TYPE_CUI:  //CU接入
		return "DEVICE_GLOBAL_TYPE_CUI";
	case DEVICE_GLOBAL_TYPE_DVR:          //DVR带存储的编码器（带本地存储的视频服务器）
		return "DEVICE_GLOBAL_TYPE_DVR";
	case DEVICE_GLOBAL_TYPE_ENCODER:      //DV 编码器（不带本地存储的视频服务器）
		return "DEVICE_GLOBAL_TYPE_ENCODER";
	case DEVICE_GLOBAL_TYPE_IPCAMERA:     //IP-Camera
		return "DEVICE_GLOBAL_TYPE_IPCAMERA";
	case DEVICE_GLOBAL_TYPE_DECODER:      //解码器
		return "DEVICE_GLOBAL_TYPE_DECODER";
	case DEVICE_GLOBAL_TYPE_VIDICON:      //摄像机
		return "DEVICE_GLOBAL_TYPE_VIDICON";
	case DEVICE_GLOBAL_TYPE_YT:           //云台
		return "DEVICE_GLOBAL_TYPE_IOIN";
	case DEVICE_GLOBAL_TYPE_IOIN:         //报警输入设备－开关量/模拟量，如红外、烟感、门磁等
		return "DEVICE_GLOBAL_TYPE_IOIN";
	case DEVICE_GLOBAL_TYPE_INOUT:        //报警输出设备:如警灯、警铃
		return "DEVICE_GLOBAL_TYPE_INOUT";
	case DEVICE_GLOBAL_TYPE_AIN:          //语音输入设备
		return "DEVICE_GLOBAL_TYPE_AIN";
	case DEVICE_GLOBAL_TYPE_AOUT:         //语音输出设备
		return "DEVICE_GLOBAL_TYPE_AOUT";
	case DEVICE_GLOBAL_TYPE_MATRIX:       //矩阵
		return "DEVICE_GLOBAL_TYPE_MATRIX";
	case DEVICE_GLOBAL_TYPE_DNS:          //
		return "DEVICE_GLOBAL_TYPE_DNS";
	case DEVICE_GLOBAL_TYPE_NPU:
		return "DEVICE_GLOBAL_TYPE_NPU";
	case DEVICE_GLOBAL_TYPE_IMU:
		return "DEVICE_GLOBAL_TYPE_IMU";
	case DEVICE_GLOBAL_TYPE_IAS:
		return "DEVICE_GLOBAL_TYPE_IAS";
	case DEVICE_GLOBAL_TYPE_TAS:
		return "DEVICE_GLOBAL_TYPE_TAS";
	case DEVICE_GLOBAL_TYPE_MTU:
		return "DEVICE_GLOBAL_TYPE_MTU";
	default:        
		return "Unknown Device";
	}
}

/*=============================================================================
    函 数 名： StringToNONum
    功    能： 将指定的字符数组转换为相应的数字, [TKDMNO结构内部使用]
    算法实现： 
    全局变量： 
    参    数： const s8* pachString
               s32 nStringLen
    返 回 值： s32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG s32 TKDMNO::StringToNONum(const s8* pachString, s32 nStringLen) const
{
	s32 nNONum = 0;
	s8  achBuf[LEN_KDM_NO+1] = {'\0'};
	if (NULL == pachString || 
		nStringLen <= 0 || nStringLen > LEN_KDM_NO)
	{
		return nNONum;
	}
	memcpy(achBuf, pachString, nStringLen);
	nNONum = atoi(achBuf);//字节序问题??
	return nNONum;
}

/*=============================================================================
    函 数 名： NumToNOString
    功    能： 将指定的数字转换为相应的字符数组,数字长度小于nStringLen则在头部补零, 
	           [TKDMNO结构内部使用]
    算法实现： 
    全局变量： 
    参    数： s32 nNONum
               s8* pachString
               s32 nStringLen
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::NumToNOString(s32 nNONum, s8* pachString, s32 nStringLen) const
{
	s32 nRealStringLen = 0;	
	s8  achBuf[LEN_KDM_NO+1] = {'\0'};
	if (NULL == pachString || nNONum < 0 || 
		nStringLen <= 0 || nStringLen > LEN_KDM_NO)
	{
		return FALSE;
	}
	memset(pachString, NOFIELD_CHAR_INVALID, nStringLen);
	//十进制转换
	nRealStringLen = sprintf(achBuf, "%d", nNONum);//字节序问题??
	if (nRealStringLen > nStringLen)
	{
		return FALSE;
	}
	memcpy((pachString+nStringLen-nRealStringLen), achBuf, nRealStringLen);

	return TRUE;
}


/*=============================================================================
    函 数 名： TKDMTime
    功    能： 缺省构造
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMTime::TKDMTime( void )
{
	SetNull();
}

/*=============================================================================
    函 数 名： TKDMTime
    功    能： 构造
    算法实现： 
    全局变量： 
    参    数： time_t tTime
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMTime::TKDMTime(time_t tTime)
{
	SetTime(&tTime);
}

/*=============================================================================
    函 数 名： SetNull
    功    能： 置空
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMTime::SetNull()
{
	memset(this, 0, sizeof(TKDMTime));
}

/*=============================================================================
    函 数 名： IsNull
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::IsNull() const
{
	TKDMTime tNullTime;
	if (0 == memcmp(this, &tNullTime, sizeof(TKDMTime)))
	{
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
    函 数 名： SetTime
    功    能： 设置时间
    算法实现： 
    全局变量： 
    参    数： const time_t *ptTime
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMTime::SetTime( const time_t *ptTime )
{
	if (NULL == ptTime)
	{
		return;
	}
	//2006-04-15 WANCL ADD
	if (UNUSED_TIMELINE_NO_LIMIT == *ptTime)
	{
		SetNull();
		return;
	}

	tm *ptLocalTime = ::localtime( ptTime );
	if (NULL != ptLocalTime) //增加指针判断，chenhongbin
	{
		m_wYear    = htons( (u16)ptLocalTime->tm_year + 1900 );
		m_byMonth  = (u8)ptLocalTime->tm_mon + 1;
		m_byMDay   = (u8)ptLocalTime->tm_mday;
		m_byHour   = (u8)ptLocalTime->tm_hour;
		m_byMinute = (u8)ptLocalTime->tm_min;
		m_bySecond = (u8)ptLocalTime->tm_sec;
	}
	else
	{
		SetNull();
	}
	return;
}

/*=============================================================================
    函 数 名： GetTime
    功    能： 得到时间结构
    算法实现： 
    全局变量： 
    参    数： time_t &tTime
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMTime::GetTime( time_t &tTime ) const
{
	//IsNull 时表示日期不作限制
	if (TRUE == IsNull())
	{
		tTime = UNUSED_TIMELINE_NO_LIMIT;
		return;
	}
	tm tTmCurTime;
	tTmCurTime.tm_year = ntohs( m_wYear ) - 1900;
	tTmCurTime.tm_mon  = m_byMonth - 1;
	tTmCurTime.tm_mday = m_byMDay;
	tTmCurTime.tm_hour = m_byHour;
	tTmCurTime.tm_min  = m_byMinute;
	tTmCurTime.tm_sec  = m_bySecond;
	
	tTime = ::mktime( &tTmCurTime );
	return;
}

/*=============================================================================
    函 数 名： GetTime
    功    能： 得到时间结构，可用于单独存储时间或者日期或者时间＋日期
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： time_t 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG time_t TKDMTime::GetTime(void) const
{
	//IsNull 时表示日期不作限制
	if (TRUE == IsNull())
	{
		return UNUSED_TIMELINE_NO_LIMIT;
	}
	
	tm  tTmCurTime;

	//chg by linlifen 2010.10.20
	//必须将该变量初始化为-1，否则在有夏令时的地方，时间有可能会出现1个小时的时差
	tTmCurTime.tm_isdst = -1;

	u16 wYear = ntohs( m_wYear );
	if (wYear >= 1900)
	{
		tTmCurTime.tm_year = wYear - 1900;
	}
	else
	{
		tTmCurTime.tm_year = 106;
	}
	if (m_byMonth > 0 && m_byMonth <= 12)
	{
		tTmCurTime.tm_mon  = m_byMonth - 1;
	}
	else
	{
		tTmCurTime.tm_mon  = 0;
	}
	if (m_byMDay > 0 && m_byMDay <= 31)
	{
		tTmCurTime.tm_mday = m_byMDay;
	}
	else
	{
		tTmCurTime.tm_mday = 1;
	}
	if (m_byHour < 24)
	{
		tTmCurTime.tm_hour = m_byHour;
	}
	if (m_byMinute < 60)
	{
		tTmCurTime.tm_min  = m_byMinute;
	}
	if (m_bySecond < 60)
	{
		tTmCurTime.tm_sec  = m_bySecond;
	}
	
	return ::mktime( &tTmCurTime );
}

/*=============================================================================
    函 数 名： operator ==  
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMTime &tObj
    返 回 值： BOOL32
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::operator == ( const TKDMTime &tObj ) const
{
	if (0 == memcmp(this, &tObj, sizeof(TKDMTime)))
	{
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
    函 数 名： operator = 
    功    能： 重载 拷贝构造
    算法实现： 
    全局变量： 
    参    数： const TKDMTime &tObj
    返 回 值： void
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMTime::operator = ( const TKDMTime &tObj )
{
	m_wYear    = tObj.m_wYear;
	m_byMonth  = tObj.m_byMonth;
	m_byMDay   = tObj.m_byMDay;
	m_byHour   = tObj.m_byHour;
	m_byMinute = tObj.m_byMinute;
	m_bySecond = tObj.m_bySecond;
}

/*=============================================================================
    函 数 名： operator > 
    功    能： 重载 一个有效时间与待检测的时间线作比较
    算法实现： 
    全局变量： 
    参    数： const TKDMTime &tTimeLine
    返 回 值： BOOL32
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::operator > ( const TKDMTime &tTimeLine ) const
{
	BOOL32 bRet = FALSE;
	
	//IsNull 时表示日期不作限制
	if (FALSE == tTimeLine.IsNull())
	{
		u32 dwCurTime  = (u32)GetTime();
		u32 dwTimeLine = (u32)tTimeLine.GetTime();
		if (dwCurTime > dwTimeLine)
		{
			bRet = TRUE;
		}
	}
	
	return bRet;
}

/*=============================================================================
    函 数 名： GetString
    功    能： 获取时间对应的格式字串
    算法实现： 
    全局变量： 
    参    数： s8* pachString
               s32 nStringLen
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::GetString(s8* pachString, s32 nStringLen) const
{
	if (nStringLen < 20)
	{
		return FALSE;
	}
	sprintf(pachString, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 
		    GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond()); 		
	return TRUE;
}

/*=============================================================================
    函 数 名： SetYear
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u16 wYear [1900,2037]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetYear( u16 wYear/*主机序*/ )
{
	if (wYear < 1900)
	{
		return FALSE;
	}
	m_wYear = htons(wYear);
	return TRUE;
}

/*=============================================================================
    函 数 名： SetMonth
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u8 byMonth [1,12]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetMonth( u8 byMonth )
{
	if (0 == byMonth || byMonth > 12)
	{
		return FALSE;
	}
	m_byMonth = byMonth;
	return TRUE;
}

/*=============================================================================
    函 数 名： SetDay
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u8 byDay [1,31]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetDay( u8 byDay )
{
	if (0 == byDay || byDay > 31)
	{
		return FALSE;
	}
	m_byMDay = byDay;
	return TRUE;
}

/*=============================================================================
    函 数 名： SetHour
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u8 byHour [0,23]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetHour( u8 byHour )
{
	if (byHour >= 24)
	{
		return FALSE;
	}
	m_byHour = byHour;
	return TRUE;
}

/*=============================================================================
    函 数 名： SetMinute
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u8 byMinute [0,59]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetMinute( u8 byMinute )
{
	if (byMinute >= 60)
	{
		return FALSE;
	}
	m_byMinute = byMinute;
	return TRUE;
}

/*=============================================================================
    函 数 名： SetSecond
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u8 bySecond [0,59]
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetSecond( u8 bySecond )
{
	if (bySecond >= 60)
	{
		return FALSE;
	}
	m_bySecond = bySecond;
	return TRUE;
}

/*=============================================================================
    函 数 名： GetDayTime
    功    能： 
    算法实现： 不再使用mktime, 直接根据时分秒计算日时间
    全局变量： 
    参    数： 清除日期
    返 回 值： void
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			陆雪忠                  创建
    2009/7/7    1.0			梁  黎                  修改
=============================================================================*/
INLINEFLAG time_t TKDMTime::GetDayTime() const
{
// 	TKDMTime tTempTime(GetTime());
// 	tTempTime.SetHour(0);
// 	tTempTime.SetMinute(0);
// 	tTempTime.SetSecond(0);
// 
// 	return (GetTime() - tTempTime.GetTime());
    return (GetHour()*3600 + GetMinute()*60 + GetSecond());
}

/*=============================================================================
    函 数 名： GetWeeklyInfo
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 获取星期信息
    返 回 值： 0~6, (0--monday, 6--sunday)
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			陆雪忠                  创建
=============================================================================*/
INLINEFLAG s32 TKDMTime::GetWeeklyInfo() const
{
	struct tm *newtime;
	time_t ltime = GetTime();	
	newtime = localtime(&ltime);

	return newtime->tm_wday == 0 ? 6 : (newtime->tm_wday - 1);
}

/*=============================================================================
    函 数 名： GetWeeklyInfo
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 是否在范围之内
    返 回 值： 0~6, (0--monday, 6--sunday)
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			陆雪忠                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::IsTimeIn(u32 dwPreTime, u32 dwNextTime) const
{
	u32 dwTime = (u32)GetTime();
	return (dwPreTime < dwTime && dwNextTime > dwTime);
}


/*=============================================================================
    函 数 名： GetWeeklyInfo
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 是否在范围之内
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/5/8    1.0			陆雪忠                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::IsTimeIn(const TKDMTime& tPreTime, const TKDMTime& tNextTime) const
{
	//modify by Royan 20121009
	//return IsTimeIn(tPreTime.GetTime(), tNextTime.GetTime());
	return IsTimeIn((u32)tPreTime.GetTime(), (u32)tNextTime.GetTime());
}

/*=============================================================================
    函 数 名： TKDMVersion
    功    能： 缺省构造
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG TKDMVersion::TKDMVersion( void )
{
	SetNull();
}

/*=============================================================================
    函 数 名： SetNull
    功    能： 置空
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMVersion::SetNull()
{
	memset(this, 0, sizeof(TKDMVersion));
}

/*=============================================================================
    函 数 名： SetVersion
    功    能： 
    算法实现： 
    全局变量： 
    参    数： const s8 *pachVersion
    返 回 值： BOOL32 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMVersion::SetVersion( const s8 *pachVersion )
{
	if (NULL == pachVersion)
	{
		return FALSE;
	}
	strncpy(m_achVersion, pachVersion, MAXLEN_VERSION_STRING);
	m_achVersion[MAXLEN_VERSION_STRING] = '\0';
	return TRUE;
}

/*=============================================================================
    函 数 名： operator == 
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMVersion &tObj
    返 回 值： BOOL32
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG BOOL32 TKDMVersion::operator == ( const TKDMVersion &tObj ) const
{
	if (0 == memcmp(m_achVersion, tObj.m_achVersion, MAXLEN_VERSION_STRING))
	{
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
    函 数 名： operator = 
    功    能： 重载
    算法实现： 
    全局变量： 
    参    数： const TKDMVersion &tObj
    返 回 值： void
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			万春雷                  创建
=============================================================================*/
INLINEFLAG void TKDMVersion::operator = ( const TKDMVersion &tObj )
{
	memcpy((s8*)m_achVersion, (s8*)tObj.m_achVersion, MAXLEN_VERSION_STRING);
	m_achVersion[MAXLEN_VERSION_STRING] = '\0';
}






/*=============================================================================
    函 数 名： CEncryptor
    功    能： 缺省构造
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG CEncryptor::CEncryptor( void )
{
}


/*=============================================================================
    函 数 名： ~CEncryptor
    功    能： 缺省析构
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG CEncryptor::~CEncryptor( void )
{
}

/*=============================================================================
    函 数 名： GetBitAt
    功    能： 获取Bit位数值
    算法实现： 
    全局变量： 
    参    数： [IN]  pachString : 处理字串
               [IN]  dwBitPos : 操作的Bit位
    返 回 值： 该位为1[TRUE]   该位为0[FLASE]
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/6/9    1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::GetBitAt(const s8* pachString, u32 dwBitPos)
{
    // 计算Byte
    u32 dwBypePost= dwBitPos/8;
    dwBitPos = dwBitPos%8;
    // 生成掩码
    u8 byMask = 0x80;
    byMask = (byMask >> dwBitPos);
    // 判断
    if ( (pachString[dwBypePost] & byMask) != 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*=============================================================================
    函 数 名： SetBitAt
    功    能： 设置Bit位数值
    算法实现： 
    全局变量： 
    参    数： [IN/OUT]  pachString : 处理字串
               [IN]  dwBitPos : 操作的Bit位
               [IN] bSetToOne : 设置为1或0
    返 回 值： 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/6/9    1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG void CEncryptor::SetBitAt(s8* pachString, u32 dwBitPos, BOOL32 bSetToOne)
{
    // 计算Byte
    u32 dwBypePost= dwBitPos/8;
    dwBitPos = dwBitPos%8;
    // 生成掩码
    u8 byMask = 0x80;
    byMask = (byMask >> dwBitPos);
    // 设置位
    u8 bySetData;
    if (bSetToOne == TRUE)
    {
        bySetData = byMask;
    }
    else
    {
        bySetData = 0;
    }
    // 生成掩码
    byMask = ~byMask;
    // 设置
    pachString[dwBypePost] = (pachString[dwBypePost] & byMask) | bySetData;
}

// 把[pachCipherText+dwCipherSize]指定的密文转化为[pachPlainText+dwPlainSize]指定的明文
/*=============================================================================
    函 数 名： EncryptString
    功    能： 加密
    算法实现： 明文pachPlainText是字符串，由dwPlainSize指定大小
               密文pachCipherText是二进制流，由dwCipherSize指定大小。目前密文全部是32位。
               加密算法   1、把明文字符串后面补0，填满32位，得到二进制流A。
                          2、对A取反，得到B。
                          3、把B按照bit分解。共32*8=256bit。按照bit的奇偶分别操作：
                              3.1 偶数的bit，移动128位。如第0个bit和第128个bit互换，第2个bit和第130个bit互换……
                              3.2 奇数的bit，前后对调。如第1个bit和第255个bit互换，第3个bit和第253个bit互换……
    全局变量： 
    参    数： [IN]  pachPlainText : 输入明文字串
               [IN]  wPlainSize : 输入明文字串的长度(不包括字串尾的'\0')
               [OUT] pachCipherText : 取得密文字串的缓存，不可以和pachPlainText相等
               [IN/OUT] wCipherSize : 输入pachCipherText的大小，返回密文的有效大小(一定是32)
    返 回 值： TRUE=成功; FALSE=失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize)
{
    // 输入检查
    if (pachPlainText == pachCipherText)
    {
        return FALSE;
    }
    if (wPlainSize > MAXLEN_PWD)
    {
        return FALSE;
    }
    if (wCipherSize < MAXLEN_PWD)
    {
        return FALSE;
    }
    // 1、把明文字符串后面补0，填满32位，得到二进制流A。
    memset(pachCipherText, 0, MAXLEN_PWD);
    memcpy(pachCipherText, pachPlainText, wPlainSize);
    // 2、对A取反，得到B。
    s32 nIndex;
    for (nIndex=0; nIndex<MAXLEN_PWD; nIndex++)
    {
        pachCipherText[nIndex] = ~(pachCipherText[nIndex]);
    }
    // 3、把B按照bit分解。共32*8=256bit。按照bit的奇偶分别操作：
    // 3.1 偶数的bit，移动128位。如第0个bit和第128个bit互换，第2个bit和第130个bit互换……
    for (nIndex=0; nIndex<(MAXLEN_PWD*8/2); nIndex+=2)
    {
        BOOL32 bBit1 = GetBitAt(pachCipherText, nIndex);
        BOOL32 bBit2 = GetBitAt(pachCipherText, MAXLEN_PWD*8/2+nIndex);
        SetBitAt(pachCipherText, MAXLEN_PWD*8/2+nIndex, bBit1);
        SetBitAt(pachCipherText, nIndex, bBit2);
    }
    // 3.2 奇数的bit，前后对调。如第1个bit和第255个bit互换，第3个bit和第253个bit互换……
    for (nIndex=1; nIndex<(MAXLEN_PWD*8/2); nIndex+=2)
    {
        BOOL32 bBit1 = GetBitAt(pachCipherText, nIndex);
        BOOL32 bBit2 = GetBitAt(pachCipherText, MAXLEN_PWD*8-nIndex);
        SetBitAt(pachCipherText, MAXLEN_PWD*8-nIndex, bBit1);
        SetBitAt(pachCipherText, nIndex, bBit2);
    }
    return TRUE;
}

/*=============================================================================
    函 数 名： DecryptString
    功    能： 解密
    算法实现： 同加密
    全局变量： 
    参    数： [IN]  pachCipherText : 输入密文字串
               [IN]  wCipherSize : 输入密文字串的长度(不包括字串尾的'\0')
               [OUT] pachPlainText : 取得明文字串的缓存，可以和pachCipherText相等
               [IN/OUT] wPlainSize : 输入pachPlainText的大小，返回明文的有效大小(不包括字串尾的'\0')
    返 回 值： TRUE=成功; FALSE=失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/4/30   1.0			赖齐                  创建
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize)
{
    // 由于加密算法是对称算法，直接调用加密接口
    EncryptString(pachCipherText, wCipherSize, pachPlainText, wPlainSize);
    // 由于明文是字串，因此可以得到长度
    wPlainSize = strlen(pachPlainText);
    if (wPlainSize > MAXLEN_PWD)
    {
        return FALSE;
    }
    return TRUE;
}


#ifdef WIN32
#pragma pack( pop )
#endif

/////////////////////////////////////////////////////////////////////////////////

#endif /* _STRUCT_KDM_H_ */
//END OF FILE
