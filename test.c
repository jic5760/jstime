#include <stdio.h>
#include <time.h>
#include "jstime.h"

int main(){
	time_t     now;
	struct tm *t;
	jstm_t t2;
	jstm_t t3;

	int64_t ts;

	time(&now);
	t = localtime(&now);

	t->tm_mday += 0; // 1년 후 시간

	t2.tm_year = t->tm_year;
	t2.tm_mon = t->tm_mon;
	t2.tm_mday = t->tm_mday;
	t2.tm_hour = t->tm_hour;
	t2.tm_min = t->tm_min;
	t2.tm_sec = t->tm_sec;

	ts = jstime_gettimestamp((jstm_t*)&t2);

	printf("t : %02u-%02u-%02u %d %d %02u:%02u:%02u\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_wday, t->tm_yday, t->tm_hour, t->tm_min, t->tm_sec);
	printf("mktime(&t)              : %lld\r\n", mktime(t) + 32400); //기존mktime 은 GMT+0으로 출력합니다. 그래서 +9시간(한국)
	printf("jstime_gettimestamp(&t) : %lld\r\n", ts);

	now = ts;
	t = gmtime(&now);
	printf("localtime    : %02u-%02u-%02u %d %d %02u:%02u:%02u\r\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_wday, t->tm_yday, t->tm_hour, t->tm_min, t->tm_sec);

	jstime_gettm(ts, &t3);
	printf("jstime_gettm : %02u-%02u-%02u %d %d %02u:%02u:%02u\r\n", t3.tm_year + 1900, t3.tm_mon + 1, t3.tm_mday, t3.tm_wday, t3.tm_yday, t3.tm_hour, t3.tm_min, t3.tm_sec);

	return 0;
}
