#include <stdio.h>
#include <time.h>
#include "jstime.h"

int main(){
	time_t     now;
	struct tm *t;
	jstm_t t2;
	
	time(&now);
	t = localtime(&now);
	
	t->tm_mday += 365; // 1년 후 시간
	
	t2.tm_year = t->tm_year;
	t2.tm_mon = t->tm_mon;
	t2.tm_mday = t->tm_mday;
	t2.tm_hour = t->tm_hour;
	t2.tm_min = t->tm_min;
	t2.tm_sec = t->tm_sec;
	
	printf("t : %02u-%02u-%02u %02u:%02u:%02u\r\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	printf("mktime(&t)   : %llu\r\n", (unsigned long long)mktime(t)+32400); //기존mktime 은 GMT+0으로 출력합니다. 그래서 +9시간(한국)
	printf("MYmktime(&t) : %llu\r\n", (unsigned long long)jstime_mktime((jstm_t*)&t2));
	return 0;
}
