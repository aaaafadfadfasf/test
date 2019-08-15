/*****************************************************************************
   模块名      : utility
   文件名      : crontime.h
   相关文件    : 
   文件实现功能: 实现用来表示定时任务的时间表示，类似linux的cron
   作者        : huangzhichun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2012/06/11   1.0        huangzhichun			创建
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
//     int tm_sec; /*秒，正常范围0-59， 但允许至61*/
//     int tm_min; /*分钟，0-59*/
//     int tm_hour; /*小时， 0-23*/
//     int tm_mday; /*日，即一个月中的第几天，1-31*/
//     int tm_mon; /*月， 从一月算起，0-11*/
//     int tm_year; /*年， 从1900至今已经多少年*/
//     int tm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
//     int tm_yday; /*从今年1月1日到目前的天数，范围0-365*/
//     int tm_isdst; /*日光节约时间的旗标*/
// };


/*时间包含5 部分,用空格分开，格式：“分 小时 日 月 星期 :持续时长”
*月、日、星期：支持模糊匹配，“*”表示任意。小时、分：只是精确匹配。
*当“月、日”匹配成功，则“星期”被忽略；如果“星期”匹配成功，则忽略“月、日”。
*“星期”取值范围：0－6
*“持续时长”为*表示无限长
*格式：<min> <hour> <day> <month> <wday> +<timeZone> D<duration>
*例如：“10 10 1 * * UTC+28800 D*”：表示每月1 号10 点10 分启动任务,东8(28800=8*3600)区，持续时间无限长。
*/
class CCronTime
{
#define CRON_ANY 0xFFFFFFFF
#define CRON_INFINITY 0x7FFFFFFF
private:
    int m_minute;
    int m_hour;
    int m_day;                  //日，保持与tm一致，1-31
    int m_month;                //月，1-12，符合字符串直观思维
    int m_wday;                 //星期，0-6 从星期日算起
    int m_timeZone;             //单位：秒

    int m_duration;             //单位：秒 
                                /*
                                备注：这里本来应该是一个unsigned int m_duration;
                                但是由于 int nDiffTime = nCurTime - nStartTime; 比较时会产生负值，与duration
                                只能用signed int
                                所以m_duration也只能定义为int，但是实际上这个值应该是一个unsigned
                                所以它的对应宏 CRON_INFINITY 表示一个无限大的正数
                                */
    //int m_maxTimes;           //执行次数,目前不实现
public:
    /*====================================================================
    函数名      :IsLeapYear
    功能        :判断是否闰年
    算法        :能够被4整除，但不能被100整除 或者 能够被4整除，又能被400整除。
    返回值说明  : 1表示闰年，0表平年
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/11/15	1.0		    黄至春    	  创建
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
    函数名      :GetDaysOfYear
    功能        :获取某天是一年中第几日
    参数        : month: 1-12; mday: 1-31; isLeapYear: 0/1 平年/闰年
    返回值说明  : 天数
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/11/15	1.0		    黄至春    	  创建
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
    //较直观的算法
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
        m_month = CRON_ANY;             //按照用户习惯从1开始 1-12
        m_wday = CRON_ANY;
        m_timeZone = 8*3600;            //默认东8区
        m_duration = CRON_INFINITY;
    }

    bool CheckValid() const
    {
        //校验日期是否合法，大月小月是否匹配
        //                             1月 2月 3月 4月 5月  6月 7月 8月 9月 10月 11月 12月
        const int aMonthDays[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30,  31, 30,  31};
        bool bValid = true;

        do 
        {
            //星期与日月不能同时指定，二选一
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

            //这里还可以判断是否闰年2月
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
    函数名      :IsTimeIn
    功能        :判断给定的时间是否在该CronTime的范围呢内
    返回值说明  :
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/10/25	1.0		    黄至春    	  创建
    ====================================================================*/
    bool IsTimeIn(time_t time)
    {
        time_t timeAdjust = time + m_timeZone;
        tm tTm = *gmtime(&timeAdjust);

        int wday = tTm.tm_wday;
        int year = tTm.tm_year + 1900;
        int month = tTm.tm_mon + 1;     //CronTime中月份从1开始，比较符合人的习惯，方便转为CronString
        int day = tTm.tm_mday;
        int hour = tTm.tm_hour;
        int minute = tTm.tm_min;
		// int second = tTm.tm_sec; 不需要.

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
            //这里也不能简单的24*7，例如周二比周一多一天，但是这种算法会得出多7天的结论
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
                //这里不能简单写死每月30天，而应该用yday计算出两个月份的差值
                //nStartTime += m_month*30*24*60*60;
                //nCurTime += month*30*24*60*60;
                /*
                算法：从第一个月算到当前的上个月，比较两者的累计天数
                例如crontime = 3月，nowtime = 4月，mday是在上一个if中计算，这里不考虑
                则计算1月1号到3月1号，以及1月1号到4月1号累计天数
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
    函数名      :ToString
    功能        :CronTime输出为字符串的表示形式
    返回值说明  :
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/06/11	1.0		    黄至春    	  创建
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
            oss <<"UTC" << (m_timeZone>0 ? "+" : "") << m_timeZone <<" ";   //m_timeZone小于0时会自动附加-号，所以不需要追加。
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
    函数名      :FromString
    功能        :从CronTime的字符串形式解析为结构体
    返回值说明  :
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/06/11	1.0		    黄至春    	  创建
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



