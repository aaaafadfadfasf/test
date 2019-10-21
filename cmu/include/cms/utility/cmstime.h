#ifndef __CMS_TIME_H__
#define __CMS_TIME_H__

/**	时间转换. 
*	time_t <-> xsd__dateTimer ( ISO8601 )
*	+by lizhixing@20111212 
*/

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
	#include <errno.h>


#define _get_timezone get_timezone
#define sprintf_s sprintf
#define sscanf_s sscanf
#endif

typedef std::string xsd__dateTimer;
typedef int time_duration;

class CCMSTime
{
public:
	inline CCMSTime();
	inline CCMSTime( time_t t );
	inline CCMSTime( const xsd__dateTimer& t, bool defaultUseLocalTimeZone = false);
	inline CCMSTime( tm t );

	inline std::string ToString()const;		    /** 同 GetXSDDateTime */
	inline void FromString( const std::string& t, bool defaultUseLocalTimeZone = false);	// 同 SetXSDDateTime

	inline xsd__dateTimer GetXSDDateTime() const;
	inline time_t GetTime() const;          //return GMT
	inline tm GetTM( tm* t = 0 ) const;     //return local time

	inline void SetXSDDateTime( const xsd__dateTimer& t, bool defaultUseLocalTimeZone = false );
	inline void SetTime( time_t t );
	inline void SetTM( tm* t );

	/** 通过本地时间来设定. timeZone默认-1表示使用本地时区. 否则采用系统的本地时区. */
	inline void SetLocalTime( int year, int month, int day, int hour, int minute, int second, long timeZone = -1 );
	inline bool GetLocalTime( int &year, int &month, int &day, int &hour, int &minute, int &second, long &timeZone )const;

	/** 获取某个指定时区的当地时间. */
	inline bool GetSpecifiedLocalTime( long specifiedTimeZone, int &year, int &month, int &day, int &hour, int &minute, int &second )const;

	/** 获取时区的字符串. */
	inline std::string GetTimeZone()const;

	inline bool operator < ( const CCMSTime& another ) const;
	inline bool operator == ( const CCMSTime& another ) const;
	inline time_duration operator - ( const CCMSTime& another ) const;	// 时间差.
	inline CCMSTime& operator += ( const time_duration& td );
	inline CCMSTime operator +( const time_duration& td ) const;

private:
	time_t m_time;
};

#ifndef WIN32
inline int gmtime_s(struct tm * _Tm, const time_t * _Time)
{
	if ( _Tm )
	{
		tm* sysTM = gmtime_r( _Time, _Tm );
		if ( sysTM == 0 )
		{
			return errno;	// 返回错误原因.
		}
// 		if( _Tm && sysTM )
// 		{
// 			*_Tm = *sysTM;
// 		}	
	}
	
	return 0;
}

inline int localtime_s(struct tm * _Tm, const time_t * _Time)
{
	tm * sysLocalTm = localtime_r( _Time, _Tm );
	if ( sysLocalTm == 0 )
	{
		return errno;
	}
// 	if( sysLocalTm && _Tm )
// 	{
// 		*_Tm = *sysLocalTm;
// 	}
	return 0;
}
#endif

inline void get_timezone( long* sec )
{
	time_t nowTime = time(0);
	tm gmtTM; 
	gmtime_s( &gmtTM, &nowTime );
	time_t gmtTime = mktime( &gmtTM );
	if ( sec )
	{
		*sec = (long)( gmtTime - nowTime );
	}
}

CCMSTime::CCMSTime()
	: m_time( time(0) )
{}

CCMSTime::CCMSTime( time_t t )
	: m_time( t )
{}

CCMSTime::CCMSTime( const xsd__dateTimer& t, bool defaultUseLocalTimeZone )
{
	this->SetXSDDateTime( t, defaultUseLocalTimeZone );
}

std::string CCMSTime::ToString()const
{
	return GetXSDDateTime();
}
void CCMSTime::FromString( const std::string& t, bool defaultUseLocalTimeZone )
{
	SetXSDDateTime( t, defaultUseLocalTimeZone );
}

std::string CCMSTime::GetTimeZone() const
{
	tm t = this->GetTM();
	long tz;
	_get_timezone(&tz);

	// 夏令时修正.
	long sumerTime = (t.tm_isdst == 1 ? 1 : 0) * 3600;	// 夏令时算成秒.
	tz -= sumerTime;

	// 时区. 格式 "-08:00" "+08:00" "-00:30"
	// 和时区的偏移值符号相反.
	char szTimezone[100] = { 0 };
	if (tz > 0)
	{
		sprintf_s(szTimezone, "-%02d:%02d", (int)tz / 3600, (int)(tz % 3600) / 60);
	}
	else
	{
		sprintf_s(szTimezone, "+%02d:%02d", (int)abs(tz / 3600), (int)abs(tz % 3600) / 60);
	}
	return szTimezone;
}

xsd__dateTimer CCMSTime::GetXSDDateTime() const
{
	tm t = this->GetTM();
	if ( t.tm_year < 0 || t.tm_mon < 0 )
	{
		assert( false );
		return xsd__dateTimer();
	}

	char szTmp[1024] = {0};
	strftime( szTmp, sizeof(szTmp), "%Y-%m-%dT%H:%M:%S", &t );
	
	xsd__dateTimer xsdTime = szTmp;
	xsdTime += GetTimeZone();
	return xsdTime;
}

time_t CCMSTime::GetTime() const
{
	return m_time;
}
tm CCMSTime::GetTM( tm* t /*= 0 */) const
{
	tm tmTime;
	memset( &tmTime, 0, sizeof( tmTime ) );
	localtime_s( &tmTime, &m_time );
	if ( t )
	{
		*t = tmTime;
	}
	return tmTime;
}

void CCMSTime::SetXSDDateTime( const xsd__dateTimer& xsdDataTime, bool defaultUseLocalTimeZone )
{
	tm tmTime;
	memset( &tmTime, 0, sizeof( tmTime ) );
	tm *a = &tmTime;
	const char * s = xsdDataTime.c_str();

	while ( *s && ( *s < '0' || *s > '9' ) )
	{
		s++;
	}

	std::string zone;
//	const char *t;
// 	if (strchr(s, '-'))
// 		t = "%d-%d-%dT%d:%d:%d%31s";
// 	else
// 	{
// 		assert( 0 );	// 不支持这种格式. 不安全.
// 		return;
// 	}
// 	else if (strchr(s, ':'))
// 		t = "%4d%2d%2dT%d:%d:%d%31s";
// 	else /* parse non-XSD-standard alternative ISO 8601 format */		
// 		t = "%4d%2d%2dT%2d%2d%2d%31s";

// 	// 安全版本( sscanf_s ) 对于字符串格式必须要带目标缓冲区长度!!
// #ifdef WIN32
// 	if (sscanf_s(s, t, &a->tm_year, &a->tm_mon, &a->tm_mday, &a->tm_hour, &a->tm_min, &a->tm_sec, zone, sizeof( zone ) ) < 6)
// #else
// 	if (sscanf(s, t, &a->tm_year, &a->tm_mon, &a->tm_mday, &a->tm_hour, &a->tm_min, &a->tm_sec, zone) < 6)
// #endif
// 	
// 	{
// 		assert( false );
// 	}
	// 2001-10-26T21:32:52, 2001-10-26T21:32:52+02:00, 2001-10-26T19:32:52Z, 2001-10-26T19:32:52+00:00, -2001-10-26T21:32:52, 
	// 之前用sscanf不安全. 现改为用C++的方式解析.
	std::stringstream ssTimeFormat;
	ssTimeFormat << s;
	char tmp;
	ssTimeFormat >> a->tm_year >> tmp >> a->tm_mon >> tmp >> a->tm_mday >> tmp >> a->tm_hour >> tmp >> a->tm_min >> tmp >> a->tm_sec >> zone;
			
	a->tm_wday = -1;
	a->tm_yday = -1;
	if (a->tm_year == 1)
		a->tm_year = 70;
	else
		a->tm_year -= 1900;
	a->tm_mon--;
	if (*zone.c_str() == '.')
	{
		for (s = zone.c_str() + 1; *s; s++)
			if (*s < '0' || *s > '9')
				break;
	}
	else
		s = zone.c_str();

	if ( 0 == *s && defaultUseLocalTimeZone )
	{
		zone = this->GetTimeZone();
		s = zone.c_str();
	}

	if (*s)
	{
		if (*s == '+' || *s == '-')
		{
			int h = 0, m = 0;
			if (s[3] == ':')
			{ /* +hh:mm */
				sscanf_s(s, "%d:%d", &h, &m);
				if (h < 0)
					m = -m;
			}
			else /* +hhmm */
			{
				m = (int)atol(s);
				h = m / 100;
				m = m % 100;
			}
			a->tm_hour -= h;
			a->tm_min -= m;
		}
	}
	else /* if no time zone then convert to internal GMT without considering DST */
	{ 
		int minuteswest;
#if defined(HAVE_GETTIMEOFDAY)
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	minuteswest = tz.tz_minuteswest;
#elif defined(HAVE_FTIME)
	struct timeb tb;
	memset((void*)&tb, 0, sizeof(tb));
#ifdef __BORLAND__
	::ftime(&tb);
#else
	ftime(&tb);
#endif
	minuteswest = tb.timezone;
#else
	/* local timezone unknown */
	minuteswest = 0;
#endif
	a->tm_min += minuteswest;
	}
	/* put hour and min in range */
	a->tm_hour += a->tm_min / 60;
	a->tm_min %= 60;
	if (a->tm_min < 0)
	{
		a->tm_min += 60;
		a->tm_hour--;
	}
	a->tm_mday += a->tm_hour / 24;
	a->tm_hour %= 24;
	if (a->tm_hour < 0)
	{
		a->tm_hour += 24;
		a->tm_mday--;
	}

	// gmt的tm 转为local time.
	time_t localTime_t = mktime( &tmTime );

	// 把本地时区去掉.就是真的gmt了.
	long timeZone;
	_get_timezone( &timeZone );
	time_t gmtTime = localTime_t - timeZone;
	this->SetTime( gmtTime );	
}
void CCMSTime::SetTime( time_t t )
{
	m_time = t;
}
void CCMSTime::SetTM( tm* t )
{
	this->SetTime( mktime( t ) );
}

void CCMSTime::SetLocalTime( int year, int month, int day, int hour, int minute, int second, long timeZone /* = -1 */ )
{
	tm localTime;
	memset( &localTime, 0, sizeof( localTime ) );
	// 取到系统的夏令时.
	// 如果用户没指定时区, 夏令时设为-1,由系统随便指定, 后面会重新判断是否在夏令时里再计算一次.
	// 如果用户指定了时区, 夏令时设置为当前时间的值, 后面会在计算中消除本地时区/夏令时的影响.
	if ( -1 == timeZone )
	{
		localTime.tm_isdst = -1;
	}
	else
	{
		time_t timeNow = time( NULL );
		tm nowTM;
		localtime_s( &nowTM, &timeNow );
		localTime.tm_isdst = nowTM.tm_isdst;
	}

	localTime.tm_year = year;
	localTime.tm_mon = month;
	localTime.tm_mday = day;
	localTime.tm_hour = hour;
	localTime.tm_min = minute;
	localTime.tm_sec = second;

	localTime.tm_wday = -1;
	localTime.tm_yday = -1;
	localTime.tm_year -= 1900;
	localTime.tm_mon--;

	time_t utcLocalTime = mktime( &localTime );

	if ( -1 == timeZone )
	{
		// 修正夏令时, 以当时的时间是否在夏令时为准重新计算一次.
		tm dstTM;
		localtime_s( &dstTM, &utcLocalTime );
		localTime.tm_isdst = dstTM.tm_isdst;
		utcLocalTime = mktime( &localTime );
	}

	time_t finalTime = utcLocalTime;
	if ( -1 != timeZone )
	{
		// 用户传入了自己的时区, 这时不能使用系统配置的时区, 必须进行转换.

		// 把本地时区去掉.就是真的gmt了.
		long localTz;
		_get_timezone( &localTz );
		time_t gmtTime = utcLocalTime - localTz;

		// gmt再加上用户的时区.
		finalTime = gmtTime + timeZone;
	}	

	// 最终, 设置.
	this->SetTime( finalTime );
}
bool CCMSTime::GetLocalTime( int &year, int &month, int &day, int &hour, int &minute, int &second, long &timeZone ) const
{
	_get_timezone( &timeZone );

	tm localTime;
	this->GetTM( &localTime );
	if ( localTime.tm_year < 0 || localTime.tm_mon < 0 )
	{
		return false;
	}

	year = localTime.tm_year + 1900;
	month = localTime.tm_mon + 1;
	day = localTime.tm_mday;
	hour = localTime.tm_hour;
	minute = localTime.tm_min;
	second = localTime.tm_sec;

	// 夏令时.
	if ( localTime.tm_isdst == 1 )
	{
		timeZone -= 3600;
	}
	return true;
}

bool CCMSTime::GetSpecifiedLocalTime( long specifiedTimeZone, int &year, int &month, int &day, int &hour, int &minute, int &second )const
{
	time_t gmtTime = this->GetTime();

	long sysTimeZone = 0;
	_get_timezone( &sysTimeZone );

	time_t specifiedLocalTime_t = gmtTime - specifiedTimeZone + sysTimeZone;

	long tmpTimeZone = 0;
	return CCMSTime(specifiedLocalTime_t).GetLocalTime( year, month, day, hour, minute, second, tmpTimeZone );	
}

bool CCMSTime::operator < ( const CCMSTime& another ) const
{
	return m_time < another.m_time;
}

bool CCMSTime::operator ==( const CCMSTime& another ) const
{
	return this->m_time == another.m_time;
}

time_duration CCMSTime::operator - ( const CCMSTime& another ) const
{
	return (time_duration)(m_time - another.m_time);
}
CCMSTime& CCMSTime::operator += ( const time_duration& td )
{
	m_time += td;
	return *this;
}
CCMSTime CCMSTime::operator +( const time_duration& td ) const
{
	return CCMSTime( m_time + td );
}

#endif  //#ifndef __CMS_TIME_H__


// end of the file


