#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "work.h"
#include "cJSON_user.h"
#include "RC522S.h"
#include "time.h"
#include "ui.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//互斥信号量句柄
extern SemaphoreHandle_t MutexSemaphore;	//互斥信号量

//_calendar_obj calendar;//时钟结构体 
_work_time work_time={0,0,0,0,0,0,0}; 		//存放时间合并后的字符，用于与Cjson数据比较  
extern	RTC_DateTypeDef GetData;    //日期
extern	RTC_TimeTypeDef GetTime;    //时间
/*
*获取秒时间戳
*/
int32_t time_to_timestamp(void)
{
	struct tm stm;
	
	stm.tm_year = GetData.Year + 100;	//RTC_Year rang 0-99,but tm_year since 1900
	stm.tm_mon	= GetData.Month -1;		//RTC_Month rang 1-12,but tm_mon rang 0-11
	stm.tm_mday	=GetData.Date;			//RTC_Date rang 1-31 and tm_mday rang 1-31
	stm.tm_hour	= GetTime.Hours -8;			//RTC_Hours rang 0-23 and tm_hour rang 0-23
	stm.tm_min	= GetTime.Minutes;   //RTC_Minutes rang 0-59 and tm_min rang 0-59
	stm.tm_sec	= GetTime.Seconds;		
	return mktime(&stm);
}
char * get_time_ms(void){

//struct tm tm_new;

char * time_str = NULL,*time_stemp={"%d000"};
u16 time_str_len = 14;

	struct tm stm;
	
	stm.tm_year = GetData.Year + 100;	//RTC_Year rang 0-99,but tm_year since 1900
	stm.tm_mon	= GetData.Month -1;		//RTC_Month rang 1-12,but tm_mon rang 0-11
	stm.tm_mday	=GetData.Date;			//RTC_Date rang 1-31 and tm_mday rang 1-31
	stm.tm_hour	= GetTime.Hours -8;			//RTC_Hours rang 0-23 and tm_hour rang 0-23
	stm.tm_min	= GetTime.Minutes;   //RTC_Minutes rang 0-59 and tm_min rang 0-59
	stm.tm_sec	= GetTime.Seconds;		

time_t now = mktime(&stm);

time_str = malloc(time_str_len);
memset(time_str, 0, time_str_len);

sprintf(time_str,time_stemp,now);

//snprintf(time_str, time_str_len, "%d000\r\n", now);
printf("stm=%d,time_str=%s",now,time_str);
return time_str;

}
/*											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //???????????	  

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的 
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}
*/


/*
*判断今天有没有任务
*/
u8 scan_work_day(u8 Num)
{
	u8 work_i=0,work_t=0;
	work_t=strlen((char*)Cjson_Buf.Cjson_Buffer_Data[Num].work_day);
	for(work_i=0;work_i<work_t;work_i++)
	{
		if(GetData.WeekDay==Cjson_Buf.Cjson_Buffer_Data[Num].work_day[work_i])
		{			
			return 1;
		}
	}
	return 0;
}
void work_task(void const * argument )
{
	static u8 minute_interval_Cheak=0,minute_Cheak=0; //判断在时间范围内，是否已经判断过了
	for( ; ; ) 
	{	
//		xSemaphoreTake(MutexSemaphore,portMAX_DELAY);		//获取互斥信号量
//	printf("%d-%d-%d %d:%d:%d\n",GetData.Year,GetData.Month,GetData.Date,GetTime.Hours,GetTime.Minutes,GetTime.Seconds);//输出闹铃时间
		if(Pcd_Massage_Flag.Have_A_Card==PCD_OK)
		{
			if((work_time.work_time_flag==0 && GetTime.Seconds<=40 && minute_Cheak==0) )	//每天0点检查是否有工作组工作
			{
				minute_Cheak=1;		//每分钟检查一次完成
				work_time.which_working_time=0;//对记录值进行清零
				while(work_time.which_working_time<Cjson_Buf.size && work_time.work_time_flag==0)
				{	                       
					if(scan_work_day(work_time.which_working_time)==1)
					{	
						if((GetTime.Hours*60 +GetTime.Minutes)==
							(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +
							 Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) 
							&& Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1)		//单次任务只判断开始时间
						{
								if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].status==1 && work_time.working_once==0)
								{
									work_time.work_time_flag=1;
									ui_event_cb=run_static;  //触发一次事件
									work_time.working_flag=0;
									printf("在第%d个工作组工作\r\n",work_time.which_working_time);
									break;//符合要求直接退出while
								}	
						}
						else if((GetTime.Hours*60 +GetTime.Minutes)>=
								(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +
								Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) && 
							(GetTime.Hours*60 +GetTime.Minutes)<=
								(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_hour *60 +
								Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_min))
						{
							if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].status==1)
							{
								work_time.work_time_flag=1;
								work_time.working_flag=0;
								ui_event_cb=run_static;  //触发一次事件
								printf("在第%d个工作组工作\r\n",work_time.which_working_time);
								break;//符合要求直接退出while
							}
														
						}
					}
						work_time.work_time_flag=0;
						work_time.working_once=0;
						work_time.which_working_time++;	//记录哪个工作组在工作									
				}
				if(work_time.work_time_flag!=1)
				{
					work_time.which_working_time=0xff;	//没有工作组符合
				}

			}
			else if(GetTime.Seconds>=40 && minute_Cheak==1)
			{
				minute_Cheak=0;
			}
								
			if(work_time.work_time_flag==1 && GetTime.Seconds<40 && minute_interval_Cheak==0)//每分钟判断一次
			{
				minute_interval_Cheak=1;
				//printf("%d-%d-%d %d:%d:%d\n",GetData.Year,GetData.Month,GetData.Date,GetTime.Hours,GetTime.Minutes,GetTime.Seconds);//输出闹铃时间					
					if(scan_work_day(work_time.which_working_time)==1)
					{
						if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1  )
						{	
							if(work_time.working_once==1){
								memset(&work_time,0,sizeof(work_time));//如果时间过了 ，复位重新判断
								ui_event_cb=run_static; //触发一次工作状态事件	
							}												
							
						}
						else if((GetTime.Hours*60 +GetTime.Minutes)<
						(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +
						Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) || 
								(GetTime.Hours*60 +GetTime.Minutes)>
						(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_hour *60 +
						Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_min))
							{
								if(work_time.working_flag==0 ||work_time.working_flag==3)			//如果现在是间隔时间内或者过了时间也要等本次工作完成后再复位
								{
									memset(&work_time,0,sizeof(work_time));//如果时间过了 ，复位重新判断
									ui_event_cb=run_static; //触发一次工作状态事件
								}	
							}
					}
					else
					{
						if(work_time.working_flag==0||work_time.working_flag==3)	//过了时间也要等本次工作完成后再复位
						{
							ui_event_cb=run_static; //触发一次工作状态事件	
							memset(&work_time,0,sizeof(work_time));//如果时间过了 ，复位重新判断
						}	
					}

					if(work_time.working_flag==3 && work_time.working_once==0)		//工作完成计时下一次工作,只有工作完并且不是不是单次任务才计数
					{
						if((work_time.working_interval_time)>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].interval_time)
						{
							work_time.working_flag=0;
							work_time.working_interval_time=0;
							ui_event_cb=NextRunning_Reflash;//触发更新进度条事件
						}
						else
						{
							work_time.working_interval_time++;
							ui_event_cb=NextRunning_Reflash;//触发更新进度条事件
						}
						
					}

					 
			}			
			else if(GetTime.Seconds>=40 && minute_interval_Cheak==1)
			{
				minute_interval_Cheak=0;
			}

			if(work_time.working_flag==0 && work_time.work_time_flag==1)  //在工作时间内，并确认此工作组为启用状态
			{
				work_time.working_flag=1;		//时间段到了，工作标志位置1
				//Pcd_Massage_Flag.Pcd_Read_Flag=0; 			//工作开始，清除读卡标志位，进行读卡
				//Pcd_Massage_Flag.Pcd_Write_Flag=1;				//工作，更新写卡标志位，去写卡
			}
			else if(work_time.working_flag==1 && Pcd_Massage_Flag.Pcd_Write_Flag==3) //要上报重量完成再工作
			{
					work_time.working_flag=2;
					work_time.working_interval_time=0;			//工作开始，同时开始计数间隔时间
					work_time.working_time=0;					//工作时间计数清零，开始工作
					Motor_Working(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].gears);
					HAL_UART_Transmit(&huart3,(u8*)"工作!", strlen("工作!"), 100);    //
			}
			else if(work_time.working_flag==2)
			{
				if(work_time.working_time>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].worktime)
				{	
					work_time.working_flag=3;					//					
					work_time.working_time=0;
										
					Pcd_Massage_Flag.Pcd_Read_Flag=0; 			//工作开始，清除读卡标志位，进行读卡
					Pcd_Massage_Flag.Pcd_Write_Flag=0;				//工作，将写卡标志位清零
					Motor_Working(0);		//
					if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1)	//如果是单次工作，置位标志位为1
					{
						work_time.working_once=1;	
					}
					else
					{
						work_time.working_once=0;	
					}
					HAL_UART_Transmit(&huart3,(u8*)"结束工作!", strlen("结束工作!"), 100);    
				}
				else
				{
					work_time.working_time++;
				}
			}			
		}
		else 
		{
			memset(&work_time,0,sizeof(work_time));//如果没有卡 ，复位重新判断

		}	
//		xSemaphoreGive(MutexSemaphore);					//释放信号量
		vTaskDelay(1000);
	}
	
}













