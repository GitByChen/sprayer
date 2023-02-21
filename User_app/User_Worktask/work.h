#ifndef __WORK_H
#define __WORK_H	    
#include	"stm32l4xx_hal_rtc.h"
/*
//时间结构体
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//公历日月年周
	u16 year;
	u8  month;
	u8  date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体
*/
typedef struct 
{	
	char *week;
	char *time;
	u8 work_time_flag;	//是否在工作时间段内
	u8 working_flag;	//是否正在工作	0:没有工作；1：准备工作；2：正在工作；3：工作完成
	u8 working_time;	//工作时间计数
	u8 which_working_time;  //记录是哪个工作组在工作
	u8 working_once;		//单次工作完成标志位 0：未完成。1：已完成
	u16 working_interval_time;	//工作间隔时间计数
}_work_time;
extern _work_time work_time; 
	
extern u8 const mon_table[12];	//月份日期数据表
void Disp_Time(u8 x,u8 y,u8 size);//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//在指定位置显示星期
u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间
int32_t time_to_timestamp(void);		/*时间戳生成，单位：秒*/
char * get_time_ms(void);				/*时间戳生成，单位：毫秒*/
#endif


