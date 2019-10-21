#ifndef __CMS_TIME_H__
#define __CMS_TIME_H__

/**	ʱ��ת��. 
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

	inline std::string ToString()const;		    /** ͬ GetXSDDateTime */
	inline void FromString( const std::string& t, bool defaultUseLocalTimeZone = false);	// ͬ SetXSDDateTime

	inline xsd__dateTimer GetXSDDateTime() const;
	inline time_t GetTime() const;          //return GMT
	inline tm GetTM( tm* t = 0 ) const;     //return local time

	inline void SetXSDDateTime( const xsd__dateTimer& t, bool defaultUseLocalTimeZone = false );
	inline void SetTime( time_t t );
	inline void SetTM( tm* t );

	/** ͨ������ʱ�����趨. timeZoneĬ��-1��ʾʹ�ñ���ʱ��. �������ϵͳ�ı���ʱ��. */
	inline void SetLocalTime( int year, int month, int day, int hour, int minute, int second, long timeZone = -1 );
	inline bool GetLocalTime( int &year, int &month, int &day, int &hour, int &minute, int &second, long &timeZone )const;

	/** ��ȡĳ��ָ��ʱ���ĵ���ʱ��. */
	inline bool GetSpecifiedLocalTime( long specifiedTimeZone, int &year, int &month, int &day, int &hour, int &minute, int &second )const;

	/** ��ȡʱ�����ַ���. */
	inline std::string GetTimeZone()const;

	inline bool operator < ( const CCMSTime& another ) const;
	inline bool operator == ( const CCMSTime& another ) const;
	inline time_duration operator - ( const CCMSTime& another ) const;	// ʱ���.
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
			return errno;	// ���ش���ԭ��.
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

	// ����ʱ����.
	long sumerTime = (t.tm_isdst == 1 ? 1 : 0) * 3600;	// ����ʱ�����.
	tz -= sumerTime;

	// ʱ��. ��ʽ "-08:00" "+08:00" "-00:30"
	// ��ʱ����ƫ��ֵ�����෴.
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
// 		assert( 0 );	// ��֧�����ָ�ʽ. ����ȫ.
// 		return;
// 	}
// 	else if (strchr(s, ':'))
// 		t = "%4d%2d%2dT%d:%d:%d%31s";
// 	else /* parse non-XSD-standard alternative ISO 8601 format */		
// 		t = "%4d%2d%2dT%2d%2d%2d%31s";

// 	// ��ȫ�汾( sscanf_s ) �����ַ�����ʽ����Ҫ��Ŀ�껺��������!!
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
	// ֮ǰ��sscanf����ȫ. �ָ�Ϊ��C++�ķ�ʽ����.
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

	// gmt��tm תΪlocal time.
	time_t localTime_t = mktime( &tmTime );

	// �ѱ���ʱ��ȥ��.�������gmt��.
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
	// ȡ��ϵͳ������ʱ.
	// ����û�ûָ��ʱ��, ����ʱ��Ϊ-1,��ϵͳ���ָ��, ����������ж��Ƿ�������ʱ���ټ���һ��.
	// ����û�ָ����ʱ��, ����ʱ����Ϊ��ǰʱ���ֵ, ������ڼ�������������ʱ��/����ʱ��Ӱ��.
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
		// ��������ʱ, �Ե�ʱ��ʱ���Ƿ�������ʱΪ׼���¼���һ��.
		tm dstTM;
		localtime_s( &dstTM, &utcLocalTime );
		localTime.tm_isdst = dstTM.tm_isdst;
		utcLocalTime = mktime( &localTime );
	}

	time_t finalTime = utcLocalTime;
	if ( -1 != timeZone )
	{
		// �û��������Լ���ʱ��, ��ʱ����ʹ��ϵͳ���õ�ʱ��, �������ת��.

		// �ѱ���ʱ��ȥ��.�������gmt��.
		long localTz;
		_get_timezone( &localTz );
		time_t gmtTime = utcLocalTime - localTz;

		// gmt�ټ����û���ʱ��.
		finalTime = gmtTime + timeZone;
	}	

	// ����, ����.
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

	// ����ʱ.
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


