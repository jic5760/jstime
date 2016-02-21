/*
 * jstime.c
 *
 * Created: 2016-02-20 PM 11:54:05 (GMT+09:00)
 *  Author: jichan (jic5760@naver.com)
 */ 

#include "jstime.h"

/* MSVC(Visual Studio)에서는 inline가 작동하지 않기 때문 */
#ifdef _MSC_VER
#define _JSTIME_INLINE static
#else
#define _JSTIME_INLINE inline
#endif

static const char _jstime_MonDays[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

_JSTIME_INLINE int32_t _jstime_abs_sec(int32_t n)
{
	if(n < 0) return 60+n;
	else return n;
}

_JSTIME_INLINE int32_t _jstime_abs_min(int32_t n)
{
	if(n < 0) return 60+n;
	else return n;
}

_JSTIME_INLINE int32_t _jstime_abs_hour(int32_t n)
{
	if(n < 0) return 24+n;
	else return n;
}

_JSTIME_INLINE void _jstime_monproc(int32_t *pyear, int32_t *pmon)
{
	while(*pmon > 12)
	{
		*pmon -= 12;
		*pyear += 1;
	}
	while(*pmon <= 0)
	{
		*pmon += 12;
		*pyear -= 1;
	}
}

_JSTIME_INLINE char _jstime_isleapyear(int year)
{
	return ((((year%4==0)&&(year%100!=0)) || (year%400==0)) ? 1 : 0);
}

_JSTIME_INLINE char _jstime_isleapyear_since1900(int year)
{
	year += 1900;
	return ((((year%4==0)&&(year%100!=0)) || (year%400==0)) ? 1 : 0);
}

_JSTIME_INLINE void _jstime_dayproc(int32_t *pyear, int32_t *pmonth, int32_t *pday)
{
	char mondays;
	
	if(*pday > 0)
	{
		while(*pday > (mondays = _jstime_MonDays[_jstime_isleapyear_since1900(*pyear)][*pmonth-1]))
		{
			*pmonth += 1;
			_jstime_monproc(pyear, pmonth);
			*pday -= mondays;
		}
	}else{
		while(*pday <= 0)
		{
			*pmonth -= 1;
			_jstime_monproc(pyear, pmonth);
			mondays = _jstime_MonDays[_jstime_isleapyear_since1900(*pyear)][*pmonth-1];
			*pday += mondays;
		}
	}
}

char jstime_isleapyear(int year)
{
	return _jstime_isleapyear(year);
}

char jstime_isleapyear_since1900(int year)
{
	return _jstime_isleapyear_since1900(year);
}

char jstime_mondays(char isleapyear, char month)
{
	if(isleapyear < 0 || isleapyear > 1) return -1;
	if(month > 12 || month < 1) return -1;
	return _jstime_MonDays[isleapyear][month-1];
}

int jstime_getydaysFromDate(int year, int month, int day)
{
	unsigned char i;
	unsigned char j;
	
	int mday = 0;
	
	if(month > 12 || month < 1 || day < 1) return -1;
	
	if(_jstime_isleapyear(year))
	{
		j = 1;
	}else{
		j = 0;
	}
	
	if(_jstime_MonDays[j][month-1] < day) return -1;
	
	for(i=0; i<month-1; i++)
	{
		mday += _jstime_MonDays[j][i];
	}
	
	mday += day;
	
	return mday;
}

int64_t jstime_gettimestamp(jstm_t *tm)
{
	int16_t i, j;
	uint16_t NumberOfleapYears=0;
	uint16_t NumberOfNotleapYears=0;
	uint16_t NumberOfThisYearDates=0;
	int64_t TotalSec=0;
	uint8_t  MonIndex;
	
	int32_t year = tm->tm_year;// + 1900;
	int32_t month = tm->tm_mon + 1;
	int32_t day = tm->tm_mday;
	int32_t hour = tm->tm_hour;
	int32_t min = tm->tm_min;
	int32_t sec = tm->tm_sec;
	
	min  += sec/60;   sec=_jstime_abs_sec(sec%60);
	hour += min/60;   min=_jstime_abs_min(min%60);
	day  += hour/24;  hour=_jstime_abs_hour(hour%24);
	
	_jstime_monproc(&year, &month);
	
	_jstime_dayproc(&year, &month, &day);
	
	j=1900;
	for(i=year-1;i>=0;i--, j++)
	{
		if(_jstime_isleapyear(j)) NumberOfleapYears++;
	}
	NumberOfNotleapYears = year - NumberOfleapYears;
	//365 * 60*60*24 = 365 * 86400 = 31536000
	//366 * 60*60*24 = 366 * 86400 = 31622400
	TotalSec+=31536000LL*NumberOfNotleapYears;
	TotalSec+=31622400LL*NumberOfleapYears;
	
	if(_jstime_isleapyear_since1900(year)){ MonIndex=1; }else{ MonIndex=0; }
	for(i=0;i<(month-1);i++){
		NumberOfThisYearDates+=(uint16_t)_jstime_MonDays[MonIndex][i];
	}
	NumberOfThisYearDates+=(day-1);
	TotalSec+=NumberOfThisYearDates*86400;
	TotalSec+=hour * 3600;
	TotalSec+=min * 60;
	TotalSec+=sec;
	
	/* unix epoch time은 1970년부터 시작하므로 1900->1970년의 70년간 흐른 초를 더해줌 */
	return TotalSec - 2208988800;
}

int jstime_gettm(int64_t timestamp, jstm_t *tm)
{
	int32_t year = 0;
	int32_t month = 0;
	int32_t day = 0;
	int32_t hour = 0;
	int32_t min = 0;
	int32_t sec = 0;
	int32_t ydays = 0;
	char wday;

	int64_t tsdays;
	int32_t tstime;

	int32_t tmp;
	char isleapyear;

	tsdays = timestamp / 86400;
	tstime = timestamp % 86400;

	hour = tstime / 3600;
	tstime = tstime % 3600;
	min = tstime / 60;
	sec = tstime % 60;

	wday = (tsdays + 4) % 7;

	year = 1970;
	tmp = 0;
	do
	{
		tsdays -= tmp;
		if ((isleapyear = _jstime_isleapyear(year)))
		{
			tmp = 366;
		}
		else{
			tmp = 365;
		}
		year++;
	} while (tsdays > tmp);
	year -= 1;

	ydays = 0;
	month = 0;
	tmp = 0;
	do
	{
		tsdays -= tmp;
		ydays += tmp;
		tmp = _jstime_MonDays[isleapyear][month];
		month++;
	} while (tsdays > tmp);

	day = tsdays;
	ydays += day;

	tm->tm_year = year - 1900;
	tm->tm_mon = month - 1;
	tm->tm_mday = day + 1;
	tm->tm_hour = hour;
	tm->tm_min = min;
	tm->tm_sec = sec;
	tm->tm_yday = ydays;
	tm->tm_wday = wday;

	return 0;
}