#ifndef __CRON_H
#define __CRON_H
#include "stm32f10x.h"
#include "rtc.h"

#define NULL    0
#define CronLine_size 		20
//时间 的临时变量，以计算下次唤醒的时间节点 
typedef struct data_time_str
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t week;
}data_time_str;
extern data_time_str CalcDataTime;

typedef struct CronLine
{
    char cl_Secs[60];   /* 0-59                                 */
	char cl_Mins[60];	/* 0-59                                 */
	char cl_Hrs[24];	/* 0-23                                 */
	char cl_Days[32];	/* 0-31                                 */
	char cl_Mons[12];	/* 0-11                                 */
	char cl_Dow[7];		/* 0-6, beginning sunday                */
}CronLine;
extern CronLine line;
static const char *const DowAry[] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",

	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	NULL
};

static const char *const MonAry[] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",

	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	NULL
};


char *ParseField(char *user, char *ary, int modvalue, int off,const char *const *names, char *ptr);
uint8_t GetWakeUpTimePoint(CronLine *pline,_calendar_obj *p_data);

#endif

