/*****************************************************************************
   ģ����      : KDM
   �ļ���      : kdmsturst.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDM���ýӿڵĽṹ����ͷ�ļ�
   ����        : ����
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���        �޸�����
   2006/02/20  1.0         ����        ����
   2006/04/30  1.0         ����        ������Ķ�����ʵ�ַֿ�
   2006/09/07  1.0         ����        �޸ı�Ź�����ԭ��8����Ļ����ϣ�
                                         ʹ��18λ�����е�ĩλ2λ��Ϊ��ҵ���
                                         һ����Ϊ��ڵ���
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

//1. ȫ�ֱ��
struct TKDMNO
{
public:
	TKDMNO( void );
	void SetNull();//�ÿ�
	BOOL32 IsNull() const; 

	BOOL32 operator == ( const TKDMNO &tObj ) const;
	BOOL32 operator != ( const TKDMNO &tObj ) const;
	
	void operator = ( const TKDMNO &tObj );
	
	//add by wdl
	BOOL32 SetKdmNO(const TKDMNO& tKdmNo);
	const TKDMNO GetKdmNo() const { return (*this); }
	
	u8  GetNOType( void ) const;     //��ȡ�������, �μ���������1.1.2
	u8  GetTradeNO( void ) const;    //��ȡ��չ����ҵ���, �μ���������
	u8  GetCPeriType( void ) const;  //��ȡƽ̨�豸����, �μ���������4.1
	u8  GetCPeriExType( void ) const; //��ȡƽ̨�豸�����չ����
	u8  GetPUType( void ) const;     //��ȡǰ���豸����, �μ���������4.2
	u8  GetPUMType( void ) const;    //��ȡǰ�����豸�����豸����, �μ���������4.2.1
	u8  GetPUPType( void ) const;    //��ȡǰ�����豸����Χ�豸����, �μ���������4.2.2

	s32 GetDomainLevel( void ) const;//��ȡ���������Ĳ㼶,���8��, [����3A�ڲ�ʹ��]
	TKDMNO GetCmuNO( void ) const;   //�õ�������[��/CMUƽ̨]���
    TKDMNO GetTopCmuNO( void ) const;   //�õ�����[��/CMUƽ̨]�Ķ���[��/CMUƽ̨]��ţ�����CMU�Ķ���CMU
	TKDMNO GetMCmuNO( void ) const;  //�õ�����[��/CMUƽ̨]�ĸ�[��/CMUƽ̨]��ţ�����CMU���ϼ�CMU
	BOOL32 IsMCmuNO(const TKDMNO &tMCMUId) const;    //�Ƿ�Ϊ��[��/CMUƽ̨]
	//wdl 06.03.20
	BOOL32 IsSCmuNO(const TKDMNO &tSCMUId) const;    //�Ƿ�Ϊ��[��/CMUƽ̨]
 	TKDMNO GetNextCmu(const TKDMNO &tDstCMUId) const;//�õ���һվCMU
	BOOL32 IsCmuInPath(const TKDMNO &tSrcCMUId,const TKDMNO &tDstCMUId) const;
    BOOL32 IsCmuDown(const TKDMNO &tDstCmuId) const;
    TKDMNO GetSameCmu(const TKDMNO &tDstCmuId) const;

	TKDMNO GetCustomerNO( void ) const;    //�õ��ͻ��û�/�ͻ��û���/�ͻ��豸�� ������[�ͻ�]���
	
	s32 GetSN( void ) const;               //��ȡ��ŵ����, [����3A�ڲ�ʹ��]

	u8  GetDeviceGlobalType( void ) const; //��ȡ�豸ͳһ����, �μ���������5.

	//��ȡ����ִ�
	const s8* GetNOString() const { return m_achNO; }
	//���ñ���ִ�
	BOOL32 SetNOString(const s8* pachString);
	
	const s8* GetStrIdType();//��ȡ�豸���������ִ�

protected:
	//��ָ�����ַ�����ת��Ϊ��Ӧ������, [TKDMNO�ṹ�ڲ�ʹ��]
	s32 StringToNONum(const s8* pachString, s32 nStringLen) const;
	
	//��ָ��������ת��Ϊ��Ӧ���ַ�����,���ֳ���С��nStringLen����ͷ������, [TKDMNO�ṹ�ڲ�ʹ��]
	BOOL32 NumToNOString(s32 nNONum, s8* pachString, s32 nStringLen) const;

protected:
	s8  m_achNO[LEN_KDM_NO+1]; //ÿһ�ֽ�Ϊʮ������ֵ, 

//�綥��CMU�����Ч18λ,����Ϊ              [99,00,00,00,00,00,00,00,00], [5], 0,00,00,00,00,00,00
//  ����CMU�µĵ�һ��DVR�����Ч27λ,����Ϊ [99,00,00,00,00,00,00,00,00], [1],[2], [1],[0,00,00,1]0,00,00
//  ����CMU�µĵ�һ��NRU�����Ч23λ,����Ϊ [99,00,00,00,00,00,00,00,00], [2],[2], [00, 1]0,00,00,00,00
//  ����CMU�µĵ�һ���ͻ������Ч24λ,����Ϊ[99,00,00,00,00,00,00,00,00], [3],[0,00,01],00,00,00,00
}PACKED;

	
//2. <���ڣ�ʱ��>�ṹ
struct TKDMTime
{
public:
	TKDMTime( void );
	TKDMTime(time_t tTime);
	
	void SetNull();//�ÿ�
	BOOL32 IsNull() const;

	void SetTime( const time_t *ptTime ); //����ʱ��
	void GetTime( time_t &tTime ) const;  //�õ�ʱ��ṹ    
	time_t GetTime(void) const;           //�õ�ʱ��ṹ

	BOOL32 operator == ( const TKDMTime &tObj ) const;
	void operator = ( const TKDMTime &tObj );
	//һ����Чʱ���������ʱ�������Ƚ�
	BOOL32 operator > ( const TKDMTime &tTimeLine ) const;

	//��ȡʱ���Ӧ�ĸ�ʽ�ִ�
	BOOL32 GetString(s8* pachString, s32 nStringLen) const;
	
	BOOL32 SetYear( u16 wYear/*������*/ );
	BOOL32 SetMonth( u8 byMonth );
	BOOL32 SetDay( u8 byDay );
	BOOL32 SetHour( u8 byHour );
	BOOL32 SetMinute( u8 byMinute );
	BOOL32 SetSecond( u8 bySecond );
	u16  GetYear( void ) const    { return ntohs(m_wYear); }//����������
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
	u16  m_wYear;   //��(������)
	u8   m_byMonth; //��
	u8   m_byMDay;  //��
	u8   m_byHour;  //ʱ
	u8   m_byMinute;//��
	u8   m_bySecond;//��
}PACKED;

//3. �汾��Ϣ(KDM.Version)
struct TKDMVersion
{
public:
	TKDMVersion( void );
	void SetNull();//�ÿ�
	
	BOOL32 SetVersion( const s8 *pachVersion );
	const s8* GetVersion( void ) const { return m_achVersion; }

	BOOL32 operator == ( const TKDMVersion &tObj ) const;
	void operator = ( const TKDMVersion &tObj );
	
protected:
	s8   m_achVersion[MAXLEN_VERSION_STRING+1];//�汾��Ϣ�ִ�
}PACKED;

//4. ������
class CEncryptor
{
public:
    CEncryptor( void );
    virtual ~CEncryptor();
    // ��pachString��ȡ��dwBitPosλ��Bit����ֵ
    static BOOL32 GetBitAt(const s8* pachString, u32 dwBitPos);
    // ��pachString������dwBitPosλ��Bit����ֵ
    static void SetBitAt(s8* pachString, u32 dwBitPos, BOOL32 bSetToOne);
    // ���ܡ���[pachPlainText+wPlainSize]ָ��������ת��Ϊ[pachCipherText+wCipherSize]ָ��������
    static BOOL32 EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize);
    // ���ܡ���[pachCipherText+wCipherSize]ָ��������ת��Ϊ[pachPlainText+wPlainSize]ָ��������
    static BOOL32 DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize);
};
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//���ֹ������ʵ�ֲ���

/*=============================================================================
    �� �� ���� TKDMNO
    ��    �ܣ� ȱʡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ��  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG TKDMNO::TKDMNO( void )
{
	SetNull();
}

/*=============================================================================
    �� �� ���� SetNull
    ��    �ܣ� �ÿ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMNO::SetNull()
{
	memset(m_achNO, NOFIELD_CHAR_INVALID, LEN_KDM_NO);
	m_achNO[LEN_KDM_NO] = '\0';
}

/*=============================================================================
    �� �� ���� IsNull
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator == 
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tObj
    �� �� ֵ�� BOOL32
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator != 
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tObj
    �� �� ֵ�� BOOL32
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator = 
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tObj
    �� �� ֵ�� void
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMNO::operator = ( const TKDMNO &tObj )
{
	memcpy((s8*)m_achNO, (s8*)tObj.m_achNO, LEN_KDM_NO);
	m_achNO[LEN_KDM_NO] = '\0';
}

/*=============================================================================
    �� �� ���� SetKdmNO
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO& tKdmNo
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			wdl                     ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::SetKdmNO(const TKDMNO& tKdmNo)
{
	return SetNOString(tKdmNo.GetNOString());
}

/*=============================================================================
    �� �� ���� GetNOType
    ��    �ܣ� ��ȡ�������, �μ���������1.1.2
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetNOType( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS], FIELD_LEN_NOTYPE));
}

/*=============================================================================
    �� �� ���� GetTradeNO
    ��    �ܣ� ��ȡ��չ����ҵ���
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/9/7    1.0			����                  ����
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetTradeNO( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_DOMAIN], FIELD_LEN_SINGLE_DOMAIN));
}

/*=============================================================================
    �� �� ���� GetCPeriType
    ��    �ܣ� ��ȡƽ̨�豸����, �μ���������4.1
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetCPeriType( void ) const
{
	u8 byType = DEVICE_TYPE_INVALID;
	if (NOFIELD_TYPE_PLATFORMDEVICE == GetNOType())
	{
		byType = (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
				                    FIELD_LEN_CDTYPE));
	}

	// �������չƽ̨�����ȡ������չ����
	if (PLATFORM_DEVICE_TYPE_EX == byType)
	{
		byType = GetCPeriExType();
	}

	return byType;
}

/*=============================================================================
    �� �� ���� GetPUType
    ��    �ܣ� ��ȡǰ���豸����, �μ���������4.2
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetPUMType
    ��    �ܣ� ��ȡǰ�����豸�����豸����, �μ���������4.2.1
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetPUPType
    ��    �ܣ� ��ȡǰ�����豸����Χ�豸����, �μ���������4.2.2
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetCPeriExType
    ��    �ܣ� ��ȡƽ̨�豸�����չ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u8 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2008/8/22   1.0			����                  ����
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetCPeriExType( void ) const
{
	return (u8)(StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+
		FIELD_LEN_CDTYPE+FIELD_LEN_CDSN], FIELD_LEN_CDTYPE_EX));
}

/*=============================================================================
    �� �� ���� GetDomainLevel
    ��    �ܣ� ��ȡ���������Ĳ㼶,���8��, [����3A�ڲ�ʹ��]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� s32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetCmuNO
    ��    �ܣ� �õ�������[��/CMUƽ̨]���
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� TKDMNO 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetTopCmuNO
    ��    �ܣ� �õ�����[��/CMUƽ̨]�Ķ���[��/CMUƽ̨]��ţ�����CMU�Ķ���CMU
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� TKDMNO 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/9/13   1.0			½ѩ��                  ����
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
    �� �� ���� GetMCmuNO
    ��    �ܣ� �õ�����[��/CMUƽ̨]�ĸ�[��/CMUƽ̨]��ţ�����CMU���ϼ�CMU
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� TKDMNO 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
			//2006-09-07 ������ҵ���
			memcpy((s8*)&tMCmuId.m_achNO[FIELD_LEN_DOMAIN], 
				   (s8*)&m_achNO[FIELD_LEN_DOMAIN], FIELD_LEN_CMU);
			memcpy((s8*)&tMCmuId.m_achNO[FIELD_LEN_CMS], achBuf, FIELD_LEN_NOTYPE);
		}
	}
	return tMCmuId;
}

/*=============================================================================
    �� �� ���� IsMCmuNO
    ��    �ܣ� �Ƿ�Ϊ��[��/CMUƽ̨]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tMCMUId
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� IsSCmuNO
    ��    �ܣ� �Ƿ�Ϊ��[��/CMUƽ̨]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tSCMUId
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/20   1.0			wdl                     ����
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
    �� �� ���� GetNextCmu
    ��    �ܣ� �õ���һվCMU
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tDstCMUId
    �� �� ֵ�� TKDMNO 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			wdl                     ����
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
    �� �� ���� IsCmuInPath
    ��    �ܣ� �б�CMU�Ƿ���ԴCMU��Ŀ��CMU��·����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMNO &tSrcCMUId
               const TKDMNO &tDstCMUId
    �� �� ֵ�� BO0L32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/13 1.0			������              ����
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
    �� �� ���� GetCustomerNO
    ��    �ܣ� �õ��ͻ��û�/�ͻ��û���/�ͻ��豸�� ������[�ͻ�]���
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� TKDMNO 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� GetSN
    ��    �ܣ� ��ȡ��ŵ���ţ�ע�����豸���͵ĺ궨��ͬ��[����3A�ڲ�ʹ��]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� s32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG s32 TKDMNO::GetSN( void ) const
{
	s32 nSN = 0;
	u8  byNOType = GetNOType();
	switch(byNOType)
	{
	case NOFIELD_TYPE_PLATFORMDEVICE://ƽ̨�豸���
		{
			if (DEVICE_TYPE_INVALID != GetCPeriType())
			{
				nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CDTYPE], 
						            FIELD_LEN_CDSN);
			}
		}
		break;
	case NOFIELD_TYPE_PRECEDINGDEVICE://ǰ���豸���
		{
			if (DEVICE_TYPE_INVALID != GetPUType()) //&& 
			{
				nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_PDTYPE+FIELD_LEN_PDSUBTYPE], 
						            FIELD_LEN_PDSN);
			}
		}
		break;
	case NOFIELD_TYPE_TUSER://�����û����
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
						        FIELD_LEN_TELUSERSN);
		}
		break;
	case NOFIELD_TYPE_DOMAINORCMU://����������
		{
			s32 nLevel = GetDomainLevel();
			if (nLevel > 0)
			{
				nSN = StringToNONum((s8*)&m_achNO[(nLevel-1)*FIELD_LEN_SINGLE_DOMAIN], 
							        FIELD_LEN_SINGLE_DOMAIN);
			}
		}
		break;
	case NOFIELD_TYPE_CUSTOMER://�ͻ����
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE], 
						        FIELD_LEN_CUSTOMERSN);
		}
		break;
	case NOFIELD_TYPE_CUSER://�ͻ��û����
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN], 
						        FIELD_LEN_CUSTOMERUSERSN);
		}
		break;
	case NOFIELD_TYPE_CUGROUP://�ͻ��û������
		{
			nSN = StringToNONum((s8*)&m_achNO[FIELD_LEN_CMS+FIELD_LEN_NOTYPE+FIELD_LEN_CUSTOMERSN], 
						        FIELD_LEN_CUGROUPSN);
		}
		break;
	case NOFIELD_TYPE_CDGROUP://�ͻ��豸�����
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
    �� �� ���� GetDeviceGlobalType
    ��    �ܣ� ��ȡ�豸ͳһ���ͣ�ע�����豸���͵ĺ궨��ͬ��
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
               
    �� �� ֵ�� u8 const
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG u8 TKDMNO::GetDeviceGlobalType( void ) const
{
	u8 byGlobalDeviceType = DEVICE_TYPE_INVALID;
	u8 byNODeviceType = DEVICE_TYPE_INVALID;

	if (NOFIELD_TYPE_DOMAINORCMU == GetNOType())   //CMU...
	{
		byGlobalDeviceType = DEVICE_GLOBAL_TYPE_CMU;
	}
	if (NOFIELD_TYPE_PLATFORMDEVICE == GetNOType())   //ƽ̨...
	{
		byNODeviceType = GetCPeriType();              //ƽ̨�豸����......
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
	if (NOFIELD_TYPE_PRECEDINGDEVICE == GetNOType())  //ǰ��...
	{
		if (PRECEDING_DEVICE_TYPE_MAIN == GetPUType())
		{
			byNODeviceType = GetPUMType();            //ǰ�����豸�����豸����......
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
			byNODeviceType = GetPUPType();            //ǰ�����豸����Χ�豸����......
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
    �� �� ���� SetNOString
    ��    �ܣ� ���ñ���ִ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const s8* pachString
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMNO::SetNOString(const s8* pachString)
{
	if (NULL == pachString)
	{
		return FALSE;
	}
	//wancl
	//ע��:3A�ڱ��ʹ������ͨ���ŵ����,��"99%",�ܳ��Ȳ�һ��Ϊ32
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
    �� �� ���� GetStrIdType
    ��    �ܣ� ��ȡ�豸���������ִ���ע�����豸���͵ĺ궨��ͬ��
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� const s8* 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/20   1.0			wdl                     ����
=============================================================================*/
INLINEFLAG const s8* TKDMNO::GetStrIdType()
{
	switch(GetDeviceGlobalType())
	{
	case DEVICE_GLOBAL_TYPE_CMU:   //CMU
		return "DEVICE_GLOBAL_TYPE_CMU";
	case DEVICE_GLOBAL_TYPE_VTDU:  //����ת��
		return "DEVICE_GLOBAL_TYPE_VTDU";
	case DEVICE_GLOBAL_TYPE_NRU:  //����¼����
		return "DEVICE_GLOBAL_TYPE_NRU";
	case DEVICE_GLOBAL_TYPE_VPU:  //���渴��
		return "DEVICE_GLOBAL_TYPE_VPU";
	case DEVICE_GLOBAL_TYPE_UAS:  //CMU�ĸ��Ի����ݷ�����
		return "DEVICE_GLOBAL_TYPE_UAS";
	case DEVICE_GLOBAL_TYPE_PUI:  //PU����
		return "DEVICE_GLOBAL_TYPE_PUI";
	case DEVICE_GLOBAL_TYPE_CUI:  //CU����
		return "DEVICE_GLOBAL_TYPE_CUI";
	case DEVICE_GLOBAL_TYPE_DVR:          //DVR���洢�ı������������ش洢����Ƶ��������
		return "DEVICE_GLOBAL_TYPE_DVR";
	case DEVICE_GLOBAL_TYPE_ENCODER:      //DV ���������������ش洢����Ƶ��������
		return "DEVICE_GLOBAL_TYPE_ENCODER";
	case DEVICE_GLOBAL_TYPE_IPCAMERA:     //IP-Camera
		return "DEVICE_GLOBAL_TYPE_IPCAMERA";
	case DEVICE_GLOBAL_TYPE_DECODER:      //������
		return "DEVICE_GLOBAL_TYPE_DECODER";
	case DEVICE_GLOBAL_TYPE_VIDICON:      //�����
		return "DEVICE_GLOBAL_TYPE_VIDICON";
	case DEVICE_GLOBAL_TYPE_YT:           //��̨
		return "DEVICE_GLOBAL_TYPE_IOIN";
	case DEVICE_GLOBAL_TYPE_IOIN:         //���������豸��������/ģ����������⡢�̸С��Ŵŵ�
		return "DEVICE_GLOBAL_TYPE_IOIN";
	case DEVICE_GLOBAL_TYPE_INOUT:        //��������豸:�羯�ơ�����
		return "DEVICE_GLOBAL_TYPE_INOUT";
	case DEVICE_GLOBAL_TYPE_AIN:          //���������豸
		return "DEVICE_GLOBAL_TYPE_AIN";
	case DEVICE_GLOBAL_TYPE_AOUT:         //��������豸
		return "DEVICE_GLOBAL_TYPE_AOUT";
	case DEVICE_GLOBAL_TYPE_MATRIX:       //����
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
    �� �� ���� StringToNONum
    ��    �ܣ� ��ָ�����ַ�����ת��Ϊ��Ӧ������, [TKDMNO�ṹ�ڲ�ʹ��]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const s8* pachString
               s32 nStringLen
    �� �� ֵ�� s32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
	nNONum = atoi(achBuf);//�ֽ�������??
	return nNONum;
}

/*=============================================================================
    �� �� ���� NumToNOString
    ��    �ܣ� ��ָ��������ת��Ϊ��Ӧ���ַ�����,���ֳ���С��nStringLen����ͷ������, 
	           [TKDMNO�ṹ�ڲ�ʹ��]
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� s32 nNONum
               s8* pachString
               s32 nStringLen
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
	//ʮ����ת��
	nRealStringLen = sprintf(achBuf, "%d", nNONum);//�ֽ�������??
	if (nRealStringLen > nStringLen)
	{
		return FALSE;
	}
	memcpy((pachString+nStringLen-nRealStringLen), achBuf, nRealStringLen);

	return TRUE;
}


/*=============================================================================
    �� �� ���� TKDMTime
    ��    �ܣ� ȱʡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG TKDMTime::TKDMTime( void )
{
	SetNull();
}

/*=============================================================================
    �� �� ���� TKDMTime
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� time_t tTime
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG TKDMTime::TKDMTime(time_t tTime)
{
	SetTime(&tTime);
}

/*=============================================================================
    �� �� ���� SetNull
    ��    �ܣ� �ÿ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMTime::SetNull()
{
	memset(this, 0, sizeof(TKDMTime));
}

/*=============================================================================
    �� �� ���� IsNull
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� SetTime
    ��    �ܣ� ����ʱ��
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const time_t *ptTime
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
	if (NULL != ptLocalTime) //����ָ���жϣ�chenhongbin
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
    �� �� ���� GetTime
    ��    �ܣ� �õ�ʱ��ṹ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� time_t &tTime
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMTime::GetTime( time_t &tTime ) const
{
	//IsNull ʱ��ʾ���ڲ�������
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
    �� �� ���� GetTime
    ��    �ܣ� �õ�ʱ��ṹ�������ڵ����洢ʱ��������ڻ���ʱ�䣫����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� time_t 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG time_t TKDMTime::GetTime(void) const
{
	//IsNull ʱ��ʾ���ڲ�������
	if (TRUE == IsNull())
	{
		return UNUSED_TIMELINE_NO_LIMIT;
	}
	
	tm  tTmCurTime;

	//chg by linlifen 2010.10.20
	//���뽫�ñ�����ʼ��Ϊ-1��������������ʱ�ĵط���ʱ���п��ܻ����1��Сʱ��ʱ��
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
    �� �� ���� operator ==  
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMTime &tObj
    �� �� ֵ�� BOOL32
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator = 
    ��    �ܣ� ���� ��������
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMTime &tObj
    �� �� ֵ�� void
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator > 
    ��    �ܣ� ���� һ����Чʱ���������ʱ�������Ƚ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMTime &tTimeLine
    �� �� ֵ�� BOOL32
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::operator > ( const TKDMTime &tTimeLine ) const
{
	BOOL32 bRet = FALSE;
	
	//IsNull ʱ��ʾ���ڲ�������
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
    �� �� ���� GetString
    ��    �ܣ� ��ȡʱ���Ӧ�ĸ�ʽ�ִ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� s8* pachString
               s32 nStringLen
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� SetYear
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u16 wYear [1900,2037]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::SetYear( u16 wYear/*������*/ )
{
	if (wYear < 1900)
	{
		return FALSE;
	}
	m_wYear = htons(wYear);
	return TRUE;
}

/*=============================================================================
    �� �� ���� SetMonth
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byMonth [1,12]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
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
    �� �� ���� SetDay
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byDay [1,31]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
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
    �� �� ���� SetHour
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byHour [0,23]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
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
    �� �� ���� SetMinute
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byMinute [0,59]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
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
    �� �� ���� SetSecond
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 bySecond [0,59]
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			����                  ����
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
    �� �� ���� GetDayTime
    ��    �ܣ� 
    �㷨ʵ�֣� ����ʹ��mktime, ֱ�Ӹ���ʱ���������ʱ��
    ȫ�ֱ����� 
    ��    ���� �������
    �� �� ֵ�� void
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			½ѩ��                  ����
    2009/7/7    1.0			��  ��                  �޸�
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
    �� �� ���� GetWeeklyInfo
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� ��ȡ������Ϣ
    �� �� ֵ�� 0~6, (0--monday, 6--sunday)
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			½ѩ��                  ����
=============================================================================*/
INLINEFLAG s32 TKDMTime::GetWeeklyInfo() const
{
	struct tm *newtime;
	time_t ltime = GetTime();	
	newtime = localtime(&ltime);

	return newtime->tm_wday == 0 ? 6 : (newtime->tm_wday - 1);
}

/*=============================================================================
    �� �� ���� GetWeeklyInfo
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� �Ƿ��ڷ�Χ֮��
    �� �� ֵ�� 0~6, (0--monday, 6--sunday)
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			½ѩ��                  ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::IsTimeIn(u32 dwPreTime, u32 dwNextTime) const
{
	u32 dwTime = (u32)GetTime();
	return (dwPreTime < dwTime && dwNextTime > dwTime);
}


/*=============================================================================
    �� �� ���� GetWeeklyInfo
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� �Ƿ��ڷ�Χ֮��
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/5/8    1.0			½ѩ��                  ����
=============================================================================*/
INLINEFLAG BOOL32 TKDMTime::IsTimeIn(const TKDMTime& tPreTime, const TKDMTime& tNextTime) const
{
	//modify by Royan 20121009
	//return IsTimeIn(tPreTime.GetTime(), tNextTime.GetTime());
	return IsTimeIn((u32)tPreTime.GetTime(), (u32)tNextTime.GetTime());
}

/*=============================================================================
    �� �� ���� TKDMVersion
    ��    �ܣ� ȱʡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG TKDMVersion::TKDMVersion( void )
{
	SetNull();
}

/*=============================================================================
    �� �� ���� SetNull
    ��    �ܣ� �ÿ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMVersion::SetNull()
{
	memset(this, 0, sizeof(TKDMVersion));
}

/*=============================================================================
    �� �� ���� SetVersion
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const s8 *pachVersion
    �� �� ֵ�� BOOL32 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator == 
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMVersion &tObj
    �� �� ֵ�� BOOL32
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
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
    �� �� ���� operator = 
    ��    �ܣ� ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TKDMVersion &tObj
    �� �� ֵ�� void
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG void TKDMVersion::operator = ( const TKDMVersion &tObj )
{
	memcpy((s8*)m_achVersion, (s8*)tObj.m_achVersion, MAXLEN_VERSION_STRING);
	m_achVersion[MAXLEN_VERSION_STRING] = '\0';
}






/*=============================================================================
    �� �� ���� CEncryptor
    ��    �ܣ� ȱʡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG CEncryptor::CEncryptor( void )
{
}


/*=============================================================================
    �� �� ���� ~CEncryptor
    ��    �ܣ� ȱʡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG CEncryptor::~CEncryptor( void )
{
}

/*=============================================================================
    �� �� ���� GetBitAt
    ��    �ܣ� ��ȡBitλ��ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� [IN]  pachString : �����ִ�
               [IN]  dwBitPos : ������Bitλ
    �� �� ֵ�� ��λΪ1[TRUE]   ��λΪ0[FLASE]
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/6/9    1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::GetBitAt(const s8* pachString, u32 dwBitPos)
{
    // ����Byte
    u32 dwBypePost= dwBitPos/8;
    dwBitPos = dwBitPos%8;
    // ��������
    u8 byMask = 0x80;
    byMask = (byMask >> dwBitPos);
    // �ж�
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
    �� �� ���� SetBitAt
    ��    �ܣ� ����Bitλ��ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� [IN/OUT]  pachString : �����ִ�
               [IN]  dwBitPos : ������Bitλ
               [IN] bSetToOne : ����Ϊ1��0
    �� �� ֵ�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/6/9    1.0			����                  ����
=============================================================================*/
INLINEFLAG void CEncryptor::SetBitAt(s8* pachString, u32 dwBitPos, BOOL32 bSetToOne)
{
    // ����Byte
    u32 dwBypePost= dwBitPos/8;
    dwBitPos = dwBitPos%8;
    // ��������
    u8 byMask = 0x80;
    byMask = (byMask >> dwBitPos);
    // ����λ
    u8 bySetData;
    if (bSetToOne == TRUE)
    {
        bySetData = byMask;
    }
    else
    {
        bySetData = 0;
    }
    // ��������
    byMask = ~byMask;
    // ����
    pachString[dwBypePost] = (pachString[dwBypePost] & byMask) | bySetData;
}

// ��[pachCipherText+dwCipherSize]ָ��������ת��Ϊ[pachPlainText+dwPlainSize]ָ��������
/*=============================================================================
    �� �� ���� EncryptString
    ��    �ܣ� ����
    �㷨ʵ�֣� ����pachPlainText���ַ�������dwPlainSizeָ����С
               ����pachCipherText�Ƕ�����������dwCipherSizeָ����С��Ŀǰ����ȫ����32λ��
               �����㷨   1���������ַ������油0������32λ���õ���������A��
                          2����Aȡ�����õ�B��
                          3����B����bit�ֽ⡣��32*8=256bit������bit����ż�ֱ������
                              3.1 ż����bit���ƶ�128λ�����0��bit�͵�128��bit��������2��bit�͵�130��bit��������
                              3.2 ������bit��ǰ��Ե������1��bit�͵�255��bit��������3��bit�͵�253��bit��������
    ȫ�ֱ����� 
    ��    ���� [IN]  pachPlainText : ���������ִ�
               [IN]  wPlainSize : ���������ִ��ĳ���(�������ִ�β��'\0')
               [OUT] pachCipherText : ȡ�������ִ��Ļ��棬�����Ժ�pachPlainText���
               [IN/OUT] wCipherSize : ����pachCipherText�Ĵ�С���������ĵ���Ч��С(һ����32)
    �� �� ֵ�� TRUE=�ɹ�; FALSE=ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize)
{
    // ������
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
    // 1���������ַ������油0������32λ���õ���������A��
    memset(pachCipherText, 0, MAXLEN_PWD);
    memcpy(pachCipherText, pachPlainText, wPlainSize);
    // 2����Aȡ�����õ�B��
    s32 nIndex;
    for (nIndex=0; nIndex<MAXLEN_PWD; nIndex++)
    {
        pachCipherText[nIndex] = ~(pachCipherText[nIndex]);
    }
    // 3����B����bit�ֽ⡣��32*8=256bit������bit����ż�ֱ������
    // 3.1 ż����bit���ƶ�128λ�����0��bit�͵�128��bit��������2��bit�͵�130��bit��������
    for (nIndex=0; nIndex<(MAXLEN_PWD*8/2); nIndex+=2)
    {
        BOOL32 bBit1 = GetBitAt(pachCipherText, nIndex);
        BOOL32 bBit2 = GetBitAt(pachCipherText, MAXLEN_PWD*8/2+nIndex);
        SetBitAt(pachCipherText, MAXLEN_PWD*8/2+nIndex, bBit1);
        SetBitAt(pachCipherText, nIndex, bBit2);
    }
    // 3.2 ������bit��ǰ��Ե������1��bit�͵�255��bit��������3��bit�͵�253��bit��������
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
    �� �� ���� DecryptString
    ��    �ܣ� ����
    �㷨ʵ�֣� ͬ����
    ȫ�ֱ����� 
    ��    ���� [IN]  pachCipherText : ���������ִ�
               [IN]  wCipherSize : ���������ִ��ĳ���(�������ִ�β��'\0')
               [OUT] pachPlainText : ȡ�������ִ��Ļ��棬���Ժ�pachCipherText���
               [IN/OUT] wPlainSize : ����pachPlainText�Ĵ�С���������ĵ���Ч��С(�������ִ�β��'\0')
    �� �� ֵ�� TRUE=�ɹ�; FALSE=ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/4/30   1.0			����                  ����
=============================================================================*/
INLINEFLAG BOOL32 CEncryptor::DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize)
{
    // ���ڼ����㷨�ǶԳ��㷨��ֱ�ӵ��ü��ܽӿ�
    EncryptString(pachCipherText, wCipherSize, pachPlainText, wPlainSize);
    // �����������ִ�����˿��Եõ�����
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
