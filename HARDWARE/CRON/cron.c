#include "cron.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "pwm.h"
#include "cJSON_user.h"
#define DEBUG_LINE()	printf("FUN:%s LINE:%d TIME:%s\r\n", __FUNCTION__, __LINE__, __TIME__)//

struct data_time_str CalcDataTime = {0};

char *ParseField(char *user, char *ary, int modvalue, int off,
						const char *const *names, char *ptr)
{
	char *base = ptr;
	int n1 = -1;
	int n2 = -1;

	if (base == NULL) {
		return (NULL);
	}

	while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
		int skip = 0;

		/* Handle numeric digit or symbol or '*' */

		if (*ptr == '*' || *ptr == '?') {
			n1 = 0;		/* everything will be filled */
			n2 = modvalue - 1;
			skip = 1;
			++ptr;
		} else if (*ptr >= '0' && *ptr <= '9') {
			if (n1 < 0) {
				n1 = strtol(ptr, &ptr, 10) + off;
			} else {
				n2 = strtol(ptr, &ptr, 10) + off;
			}
			skip = 1;
		} else if (names) {
			int i;

			for (i = 0; names[i]; ++i) {
				if (strncmp(ptr, names[i], strlen(names[i])) == 0) {
					break;
				}
			}
			if (names[i]) {
				ptr += strlen(names[i]);
				if (n1 < 0) {
					n1 = i;
				} else {
					n2 = i;
				}
				skip = 1;
			}
		}

		/* handle optional range '-' */

		if (skip == 0) {
			printf("  failed user %s parsing %s\n", user, base);
			//DEBUG_LINE();
			return (NULL);
		}
		if (*ptr == '-' && n2 < 0) {
			++ptr;
			continue;
		}

		/*
		 * collapse single-value ranges, handle skipmark, and fill
		 * in the character array appropriately.
		 */

		if (n2 < 0) {
			n2 = n1;
		}
		if (*ptr == '/') {
			skip = strtol(ptr + 1, &ptr, 10);
		}
		/*
		 * fill array, using a failsafe is the easiest way to prevent
		 * an endless loop
		 */

		{
			int s0 = 1;
			int failsafe = 1024;

			--n1;
			do {
				n1 = (n1 + 1) % modvalue;

				if (--s0 == 0) {
					ary[n1 % modvalue] = 1;
					s0 = skip;
				}
			}
			while (n1 != n2 && --failsafe);

			if (failsafe == 0) {
				printf(" failed user %s parsing %s\n", user, base);
				//DEBUG_LINE();
				return (NULL);
			}
		}
		if (*ptr != ',') {
			break;
		}
		++ptr;
		n1 = -1;
		n2 = -1;
	}

	if (*ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
		printf(" failed user %s parsing %s\n", user, base);
		//DEBUG_LINE();
		return (NULL);
	}

	while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
		++ptr;
	}

	//if (1) 
	//{
	//	int i;
	//	printf("%s:",user);
	//	for (i = 0; i < modvalue; ++i) {
	//		printf(" %d ", ary[i]);
	//	}
	//	printf("\r\n");
	//}


	return (ptr);
}

uint8_t  GetMaxday(struct data_time_str *p_data)
{
	uint8_t maxday = 0;
	uint16_t year = p_data->year;
	switch(p_data->month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			maxday = 31;  // 1 3 5 7 8 10 12月份
			break;
		case 2:
			if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
			{
				maxday = 29;  // 2月份 闰年
			}
			else
			{
				maxday = 28;  // 2月份 平年
			}			 
			break;
		default :
			 maxday = 30; // 4 6 9 11 月份
			 break;
	}
	return maxday;
}

//基姆拉尔森计算公式
//y:2019 m:8 d:28
//返回为 0 -- 6，beginning sunday 
uint8_t RTCGetWeek(uint32_t y,uint8_t m,uint8_t d)
{
	uint8_t week = 0;
	if(m==1||m==2) 
	{
		m+=12;
		y--;
	}
	week=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400+1)%7;
	return week;
}
//调用一次，加1分钟
void UpdateTimeBuf(struct data_time_str *p_data) 
{
	uint8_t maxday = 0;
	if(p_data->sec == 59)
	{
		p_data->sec = 0;
		if(p_data->min == 59)
		{
			p_data->min = 0;
			if(p_data->hour == 23)
			{
				p_data->hour = 0;
				maxday = GetMaxday(p_data);
				if(p_data->day ==  maxday)
				{
					p_data->day = 1;
					if(p_data->month == 12)
					{
						p_data->month = 1;
						p_data->year++;
					}
					else
					{
						p_data->month++;	
					}
				}
				else
				{
					p_data->day++;	
				}
			}
			else
			{
				p_data->hour++;
			}
		}
		else
		{
			p_data->min++;	
		}	
	}
	else
	{
		p_data->sec++;	
	}
		
		//根据年月日计算出当天是星期几
	p_data->week = RTCGetWeek(p_data->year,p_data->month,p_data->day);
}

//寻找满足crond条件的时间点
uint8_t GetWakeUpTimePoint(CronLine *pline, _calendar_obj *p_data)
{
	if((pline->cl_Secs[p_data->sec] == 1)
        &&(pline->cl_Mins[p_data->min] == 1)
		&& (pline->cl_Hrs[p_data->hour] == 1)
		&& (pline->cl_Days[p_data->date] == 1)
		&& (pline->cl_Mons[p_data->month] == 1)
		&& (pline->cl_Dow[p_data->week] == 1))
	{
		//printf("%04d年%02d月%02d日  %02d:%02d:%02d   星期%d\r\n",
		//		p_data->year,p_data->month,p_data->date,
		//		p_data->hour,p_data->min,p_data->sec,p_data->week);
		return 1;
	}
	return 0;
}
int cron_test()
{
	
	char *ptr;
//	uint32_t cnt = 0; //做个循环限制 
//	uint8_t getcnt = 0; //记录找到唤醒时间的次数 
	
	/*buf：Crontab表达式*/
	//char buf[] = "0 0 1,20 * * "; //0 18 * * * //10 5 */5 * *
	//ptr = ParseField("Mins", line.cl_Mins, 60, 0, NULL, buf);
	//ptr = ParseField("Hrs", line.cl_Hrs, 24, 0, NULL, ptr);
	//ptr = ParseField("Days", line.cl_Days, 32, 0, NULL, ptr);
	//ptr = ParseField("Mons", line.cl_Mons, 12, -1, MonAry, ptr);
	//ptr = ParseField("Week", line.cl_Dow, 7, 0, DowAry, ptr);
        	/*
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	*/
	//先定义个时间初始值
	//struct data_time_str CalcDataTime = {34,9,30,8,2019,4}; 
	CalcDataTime.year = 2019;
	CalcDataTime.month = 8;
	CalcDataTime.day = 30;
	CalcDataTime.hour = 9;
	CalcDataTime.min = 34;

	if (ptr == NULL) 
	{
		printf("It's over\r\n");
	}
//	while((cnt < 0xFFFFFFFF) && (getcnt <= 10))
//	{
//		cnt++;
//		UpdateTimeBuf(&CalcDataTime);
//		if(GetWakeUpTimePoint(&line,&CalcDataTime))
//		{
//			getcnt++;
//		}			
//	}	
    return 0;
}
