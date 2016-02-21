/*
 * jstime.h
 *
 * Created: 2016-02-20 PM 9:54:22 (GMT+09:00)
 *  Author: jichan (jic5760@naver.com)
 */ 


#ifndef JSTIME_H_
#define JSTIME_H_

#include <stdint.h>

typedef struct _tag_jstm {
	int32_t tm_sec;   /* Seconds */
	int32_t tm_min;   /* Minutes */
	int32_t tm_hour;  /* Hour (0--23) */
	int32_t tm_mday;  /* Day of month (1--31) */
	int32_t tm_mon;   /* Month (0--11) */
	int32_t tm_year;  /* Year (calendar year minus 1900) */
	int32_t tm_wday;  /* Weekday (0--6; Sunday = 0) */
	int32_t tm_yday;  /* Day of year (0--365) */
	int32_t tm_isdst; /* 0 if daylight savings time is not in effect) */
} jstm_t;

/* Leap year 체크 (year : 0년부터 시작한 년도) */
extern char    jstime_isleapyear(int year);

/* Leap year 체크 (year : 1900년부터 시작한 년도) */
extern char    jstime_isleapyear_since1900(int year);

/* 월별 날짜 수 (isleapyear : 0-no leap year / 1-leap year) */
extern char    jstime_mondays(char isleapyear, char month);

/* day of year 구하기 (year : 0년부터 시작한 년도) */
extern int     jstime_getydaysFromDate(int year, int month, int day);

/* epoch time 구하기 (1970년 1월 1일 0시 0분 0초 부터 흐른 초) */
extern int64_t jstime_mktime(jstm_t *tm);

#endif /* JSTIME_H_ */