/*****************************************************************************
   ģ����      : utility
   �ļ���      : crontime.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ʵ��������ʾ��ʱ�����ʱ���ʾ������linux��cron
   ����        : huangzhichun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���				�޸�����
   2012/06/11   1.0        huangzhichun			����
******************************************************************************/

#ifndef __CRON_TIME_H__
#define __CRON_TIME_H__

#include <string>
#include <sstream>

#ifdef WIN32
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#elif _LINUX_
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define _get_timezone get_timezone
#define sprintf_s sprintf
#define sscanf_s sscanf
#endif


#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
#define STRTOK_SAFE(strToken, strDelimit, pstrContext) strtok_s(strToken, strDelimit, pstrContext)

#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define STRTOK_SAFE(strToken, strDelimit, pstrContext) strtok(strToken, strDelimit)

#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define STRTOK_SAFE(strToken, strDelimit, pstrContext) strtok_r(strToken, strDelimit, pstrContext)
#endif

//char *strtok_safe( char *strToken, const char *strDelimit )
//{
//    for(char* pHead=pTail=lpszCronTime; pTail != NULL; pTail++)
//    {
//        if(pTail)
//            if(pHead)
//    }
//}


//char* token = STRTOK_SAFE( lpszCronTime, seps, &pPos);
//while( token != NULL )
//{
//    //printf( " %s\n", token );
//    szPart[] = token;

//    token = STRTOK_SAFE( NULL, seps, &pPos);
//}

// struct tm
// {
//     int tm_sec; /*�룬������Χ0-59�� ��������61*/
//     int tm_min; /*���ӣ�0-59*/
//     int tm_hour; /*Сʱ�� 0-23*/
//     int tm_mday; /*�գ���һ�����еĵڼ��죬1-31*/
//     int tm_mon; /*�£� ��һ������0-11*/
//     int tm_year; /*�꣬ ��1900�����Ѿ�������*/
//     int tm_wday; /*���ڣ�һ���еĵڼ��죬 ������������0-6*/
//     int tm_yday; /*�ӽ���1��1�յ�Ŀǰ����������Χ0-365*/
//     int tm_isdst; /*�չ��Լʱ������*/
// };


/*ʱ�����5 ����,�ÿո�ֿ�����ʽ������ Сʱ �� �� ���� :����ʱ����
*�¡��ա����ڣ�֧��ģ��ƥ�䣬��*����ʾ���⡣Сʱ���֣�ֻ�Ǿ�ȷƥ�䡣
*�����¡��ա�ƥ��ɹ��������ڡ������ԣ���������ڡ�ƥ��ɹ�������ԡ��¡��ա���
*�����ڡ�ȡֵ��Χ��0��6
*������ʱ����Ϊ*��ʾ���޳�
*��ʽ��<min> <hour> <day> <month> <wday> +<timeZone> D<duration>
*���磺��10 10 1 * * UTC+28800 D*������ʾÿ��1 ��10 ��10 ����������,��8(28800=8*3600)��������ʱ�����޳���
*/
class CCronTime
{
#define CRON_ANY 0xFFFFFFFF
#define CRON_INFINITY 0x7FFFFFFF
private:
    int m_minute;
    int m_hour;
    int m_day;                  //�գ�������tmһ�£�1-31
    int m_month;                //�£�1-12�������ַ���ֱ��˼ά
    int m_wday;                 //���ڣ�0-6 ������������
    int m_timeZone;             //��λ����

    int m_duration;             //��λ���� 
                                /*
                                ��ע�����ﱾ��Ӧ����һ��unsigned int m_duration;
                                �������� int nDiffTime = nCurTime - nStartTime; �Ƚ�ʱ�������ֵ����duration
                                ֻ����signed int
                                ����m_durationҲֻ�ܶ���Ϊint������ʵ�������ֵӦ����һ��unsigned
                                �������Ķ�Ӧ�� CRON_INFINITY ��ʾһ�����޴������
                                */
    //int m_maxTimes;           //ִ�д���,Ŀǰ��ʵ��
public:
    /*====================================================================
    ������      :IsLeapYear
    ����        :�ж��Ƿ�����
    �㷨        :�ܹ���4�����������ܱ�100���� ���� �ܹ���4���������ܱ�400������
    ����ֵ˵��  : 1��ʾ���꣬0��ƽ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/11/15	1.0		    ������    	  ����
    ====================================================================*/
    static int IsLeapYear(int year)
    {
        if(( (year%4==0) && (year%100!=0) ) || (year%400==0) )
        {
            return 1;
        }
        return 0;
    }
    /*====================================================================
    ������      :GetDaysOfYear
    ����        :��ȡĳ����һ���еڼ���
    ����        : month: 1-12; mday: 1-31; isLeapYear: 0/1 ƽ��/����
    ����ֵ˵��  : ����
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/11/15	1.0		    ������    	  ����
    ====================================================================*/
    static int GetDaysOfYear(int month, int mday, int isLeapYear)
    {
        int yday = 0;
        if( month <= 8 )
        {
            yday = month/2*31 + (month-1-month/2)*30+mday;
        }
        else
        {
            yday = (month+1)/2*31 + (month-1-(month+1)/2)*30+mday;
        }

        if ( month > 2 )
        {
            if(isLeapYear==1)
            {
                yday -= 1;
            }
            else
            {
                yday -= 2;
            }
        }
        
        return yday;
    }
    //��ֱ�۵��㷨
//     static int GetDaysOfYear1(int month, int mday, int isLeapYear)
//     {
//         int num=0;
//         int a[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
//         int i=0;
// 
//         if ((month>2)&&isLeapYear)
//         {
//             a[0]=1;
//         }
//         for (i=0;i<month;i++)
//         {
//             num += a[i];
//         }
//         num+=mday;
//         return num;
//     }
public:
    CCronTime()
    {
        Clear();
    }

    CCronTime(int minute, int hour, int day, int month, int wday, int timeZone, int duration)
    {
        m_minute = minute;
        m_hour = hour;
        m_day = day;
        m_month = month;
        m_wday = wday;
        m_timeZone = timeZone;
        m_duration = duration;
        CheckValid();
    }

    void Clear()
    {
        m_minute = CRON_ANY;
        m_hour = CRON_ANY;
        m_day = CRON_ANY;
        m_month = CRON_ANY;             //�����û�ϰ�ߴ�1��ʼ 1-12
        m_wday = CRON_ANY;
        m_timeZone = 8*3600;            //Ĭ�϶�8��
        m_duration = CRON_INFINITY;
    }

    bool CheckValid() const
    {
        //У�������Ƿ�Ϸ�������С���Ƿ�ƥ��
        //                             1�� 2�� 3�� 4�� 5��  6�� 7�� 8�� 9�� 10�� 11�� 12��
        const int aMonthDays[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30,  31, 30,  31};
        bool bValid = true;

        do 
        {
            //���������²���ͬʱָ������ѡһ
            if (CRON_ANY != (unsigned int)m_wday && 
                (CRON_ANY != (unsigned int)m_day || CRON_ANY != (unsigned int)m_month))
            {
                bValid = false;
                break;
            }

            if ( CRON_ANY != (unsigned int)m_minute && m_minute > 59 )
            {
                bValid = false;
                break;
            }

            if ( CRON_ANY != (unsigned int)m_hour && m_hour > 23 )
            {
                bValid = false;
                break;
            }

            if ( CRON_ANY != (unsigned int)m_month && m_month > 12 )
            {
                bValid = false;
                break;
            }

            //���ﻹ�����ж��Ƿ�����2��
            if ( CRON_ANY != (unsigned int)m_day )
            {
                int nMaxDay = 31;
                if ( CRON_ANY != (unsigned int)m_month )
                {
                    nMaxDay = aMonthDays[m_month];
                }
                if ( m_day > nMaxDay )
                {
                    bValid = false;
                    break;
                }
            }

            if ( CRON_ANY != (unsigned int)m_wday && m_wday > 6 )
            {
                bValid = false;
                break;
            }
		} while (0);

        assert( bValid );
        return bValid;
    }

    /*====================================================================
    ������      :IsTimeIn
    ����        :�жϸ�����ʱ���Ƿ��ڸ�CronTime�ķ�Χ����
    ����ֵ˵��  :
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/10/25	1.0		    ������    	  ����
    ====================================================================*/
    bool IsTimeIn(time_t time)
    {
        time_t timeAdjust = time + m_timeZone;
        tm tTm = *gmtime(&timeAdjust);

        int wday = tTm.tm_wday;
        int year = tTm.tm_year + 1900;
        int month = tTm.tm_mon + 1;     //CronTime���·ݴ�1��ʼ���ȽϷ����˵�ϰ�ߣ�����תΪCronString
        int day = tTm.tm_mday;
        int hour = tTm.tm_hour;
        int minute = tTm.tm_min;
		// int second = tTm.tm_sec; ����Ҫ.

        int nStartTime = 0;
        int nCurTime = 0;
        if(CRON_ANY != (unsigned int)m_minute)
        {
            nStartTime += m_minute*60;
            nCurTime += minute*60;
        }
        if(CRON_ANY != (unsigned int)m_hour)
        {
            nStartTime += m_hour*60*60;
            nCurTime += hour*60*60;
        }

        if(CRON_ANY != (unsigned int)m_wday)
        {
            //����Ҳ���ܼ򵥵�24*7�������ܶ�����һ��һ�죬���������㷨��ó���7��Ľ���
            //nStartTime += m_wday*7*24*60*60;
            //nCurTime += wday*7*24*60*60;

            nStartTime += m_wday*24*60*60;
            nCurTime += wday*24*60*60;
        }
        else
        {
            if(CRON_ANY != (unsigned int)m_day)
            {
                nStartTime += m_day*24*60*60;
                nCurTime += day*24*60*60;
            }
            if(CRON_ANY != (unsigned int)m_month)
            {
                //���ﲻ�ܼ�д��ÿ��30�죬��Ӧ����yday����������·ݵĲ�ֵ
                //nStartTime += m_month*30*24*60*60;
                //nCurTime += month*30*24*60*60;
                /*
                �㷨���ӵ�һ�����㵽��ǰ���ϸ��£��Ƚ����ߵ��ۼ�����
                ����crontime = 3�£�nowtime = 4�£�mday������һ��if�м��㣬���ﲻ����
                �����1��1�ŵ�3��1�ţ��Լ�1��1�ŵ�4��1���ۼ�����
                */
                int nIsLeapYear = IsLeapYear(year);
                int nStartDay = GetDaysOfYear(m_month, 1, nIsLeapYear);
                int nCurDay = GetDaysOfYear(month, 1, nIsLeapYear);
                nStartTime += nStartDay*24*60*60;
                nCurTime += nCurDay*24*60*60;

            }
        }

        int nDiffTime = nCurTime - nStartTime;
        if ( nDiffTime < 0 || nDiffTime > m_duration )
        {
            return false;
        }

        return true;
    }

    /*====================================================================
    ������      :ToString
    ����        :CronTime���Ϊ�ַ����ı�ʾ��ʽ
    ����ֵ˵��  :
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/06/11	1.0		    ������    	  ����
    ====================================================================*/
	std::string ToString()const
    {
        if (!CheckValid())
        {
            return "";
        }

		std::ostringstream oss;
		if(CRON_ANY == (unsigned int)m_minute)
		{
			oss << "* ";
		}
		else
		{
			oss << m_minute <<" ";
		}

		if(CRON_ANY == (unsigned int)m_hour)
		{
			oss << "* ";
		}
		else
		{
			oss << m_hour <<" ";
		}

		if(CRON_ANY == (unsigned int)m_day)
		{
			oss << "* ";
		}
		else
		{
			oss << m_day <<" ";
		}

        if(CRON_ANY == (unsigned int)m_month)
        {
            oss << "* ";
        }
        else
        {
            oss << m_month <<" ";
        }

		if(CRON_ANY == (unsigned int)m_wday)
		{
			oss << "* ";
		}
		else
		{
			oss << m_wday <<" ";
		}

        if (CRON_ANY == (unsigned int)m_timeZone)
        {
            oss << "UTC* ";
        } 
        else
        {
            oss <<"UTC" << (m_timeZone>0 ? "+" : "") << m_timeZone <<" ";   //m_timeZoneС��0ʱ���Զ�����-�ţ����Բ���Ҫ׷�ӡ�
        }
        

        if(CRON_INFINITY == (unsigned int)m_duration)
        {
            oss << "D*";
        }
        else
        {
            oss << "D"<< m_duration;
        }

        return oss.str();
    }

    /*====================================================================
    ������      :FromString
    ����        :��CronTime���ַ�����ʽ����Ϊ�ṹ��
    ����ֵ˵��  :
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/06/11	1.0		    ������    	  ����
    ====================================================================*/
	bool FromString( const char* lpszCronTime )
    {
        if(NULL == lpszCronTime)
        {
            return false;
        }

        Clear();

        //enum ECronPart{ CT_MIN, CT_HOR, CT_DAY, CT_MON, CT_WDAY, CT_DUR, CT_NUM };
        //char* szPart[CT_NUM]={0};

        char achCronTime[32]={0};
        strcpy(achCronTime, lpszCronTime);

        char seps[]   = " \t :";
        char* pPos = NULL;

        //minute
        char* token = STRTOK_SAFE( achCronTime, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        if(strcmp(token, "*") !=0 )
        {
            m_minute = atoi(token);
        }

        //hour
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        if(strcmp(token, "*") !=0 )
        {
            m_hour = atoi(token);
        }

        //day
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        if(strcmp(token, "*") !=0 )
        {
            m_day = atoi(token);
        }

        //month
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        if(strcmp(token, "*") !=0 )
        {
            m_month = atoi(token);
        }

        //wday
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        if(strcmp(token, "*") !=0 )
        {
            m_wday = atoi(token);
        }

        //timeZone
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        char* pszUTC = strstr(token, "UTC");
        if( pszUTC != 0 )
        {
            token = pszUTC+3;
            if(strcmp(token, "*") !=0 )
            {
                m_timeZone = atoi(token);
            }
        }

        //duration
        token = STRTOK_SAFE( NULL, seps, &pPos);
        if(NULL == token)
        {
            return false;
        }
        char* pszDur = strstr(token, "D");
        if( pszDur != 0 )
        {
            token = pszDur+1;
            if(strcmp(token, "*") !=0 )
            {
                m_duration = atoi(token);
            }
        }

        if (!CheckValid())
        {
            return false;
        }

        return true;
    }

public:
    //minute
    int GetMinute() const
    {
        return m_minute;
    }
    void SetMinute(int var)
    {
        m_minute = var;
    }

    //hour
    int GetHour() const
    {
        return m_hour;
    }
    void SetHour(int var)
    {
        m_hour = var;
    }

    //day 1-31
    int GetDay() const
    {
        return m_day;
    }
    void SetDay(int var)
    {
        m_day = var;
    }

    //month 1-12
    int GetMonth() const
    {
        return m_month;
    }
    void SetMonth(int var)
    {
        m_month = var;
    }

    //wday
    int GetWday() const
    {
        return m_wday;
    }
    void SetWday(int var)
    {
        m_wday = var;
    }

    //timeZone
    int GetTimeZone() const
    {
        return m_timeZone;
    }
    void SetTimeZone(int var)
    {
        m_timeZone = var;
    }

    //duration
    int GetDuration() const
    {
        return m_duration;
    }
    void SetDuration(int var)
    {
        m_duration = var;
    }
};

#endif  //#ifndef __CRON_TIME_H__



