#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "work.h"
#include "usart.h"
#include "tim.h"	
#include "w25q64.h"
#include "cJSON.h"
#include "cJSON_user.h"
#include "hx711.h"
#include "rtc.h"
#include "RC522S.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"

Cjson Cjson_Buf={0};
/**********USART TASK****************/
void Uart_task(void)
{
	
 	u8 ret;
		
		if(UART3_RX_STA & 0X8000)
		{
            u8 buffer[100]={0};
//			printf("%s\r\n",UART3_RX_Buffer);		
			strcpy((char*)buffer,(char *)UART3_RX_Buffer);
			UART3_RX_STA = 0;
			memset(&UART3_RX_Buffer,0,sizeof(UART3_RX_Buffer));				//清除接收
				if(strstr((const char*)buffer,CHEAK_TASK)!=0)
				{		
					for(ret=0;ret<Cjson_Buf.size;ret++)
					{
						printf("星期几开始:%d,星期几结束:%d,开始时间:%s,结束时间:%s,工作间隔：%d,工作档位：%d,工作时间:%d,单次任务：%d\r\n",Cjson_Buf.Cjson_Buffer_Data[ret].week_start,
										Cjson_Buf.Cjson_Buffer_Data[ret].week_end,
										Cjson_Buf.Cjson_Buffer_Data[ret].timestart,
										Cjson_Buf.Cjson_Buffer_Data[ret].timeend,
										Cjson_Buf.Cjson_Buffer_Data[ret].interval_time,
										Cjson_Buf.Cjson_Buffer_Data[ret].gears,  
										Cjson_Buf.Cjson_Buffer_Data[ret].worktime,
                                        Cjson_Buf.Cjson_Buffer_Data[ret].once_task);
					}
					printf("在第%d个工作组工作\r\n",work_time.which_working_time);
					
				}
				else if(strstr((const char*)buffer,"Get_WeightBase")!=0)
				{
					Get_WeightBase();
				}
                else if(strstr((const char*)buffer,"set_Weight")!=0)
				{
                    int i;
                    sscanf((char*)buffer,"set_Weight=%d",&i);
                    printf("k=%d\r\n",i);
					HX711_Massage.K=((((HX711_VAVDD)/i)*HX711_GAIN)*HX711_24BIT)/HX711_BASE_VDD;      //计算转换的K值
                    HX711_Massage.K/=HX711_K_VALUE;
                    Pcd_Massage_Flag.Pcd_Read_Flag=0;
                    Pcd_Massage_Flag.Pcd_Write_Flag=0;
                    Pcd_Massage_Flag.Card_Modal=0;
                    // memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断
                    //Get_Weight();

				}
                else if(strstr((const char*)buffer,"set_k")!=0)
				{
                    float i;
                    sscanf((char*)buffer,"set_k=%f",&i);
                    printf("k=%f\r\n",i);
                    HX711_Massage.K=((((HX711_VAVDD)/HX711_BEARING)*HX711_GAIN)*HX711_24BIT)/HX711_BASE_VDD;      //计算转换的K值
                    HX711_Massage.K/=i;
                    Get_Weight();
                   //memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断

                }
                else if(strstr((const char*)buffer,"set_card_val")!=0)
				{
                     Pcd_Massage_Flag.Pcd_Read_Flag=0;
                     Pcd_Massage_Flag.Pcd_Write_Flag=1;
                    Pcd_Massage_Flag.Card_Modal=1;
                    memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断
                }                
				else if(strstr((const char*)buffer,"arder")!=0)
				{
					ParseStr((const char *)buffer,createRTCTask);
				}			
		}

	/*	if(Message_Queue_Uart2!=NULL)
		{
			buffer=malloc(USART_REC_LEN);
			memset(buffer,0,USART_REC_LEN);	//清除缓冲区
			err=xQueueReceiveFromISR(Message_Queue_Uart2,buffer,&xTaskWokenByReceive_usart2);//请求消息Message_Queue
			if(err == pdPASS)			//接收到消息
			{
				USART1_SendStr(buffer);				
			}
			free(buffer);		//释放内存
			portYIELD_FROM_ISR(xTaskWokenByReceive_usart2);//如果需要的话进行一次任务切换

		}*/
}
/**********USART TASK****************/

//CJSON数据处理函数
uint8_t ParseStr(const char *JSON,TPYE CJSON_TASK_TPYE)
{
    //JSON = {"arder":"add","weekstart":1,"weekend":2,"starttime":"8:00","endtime":"12:30","interval_time":"30","gears":"3","worktime":10};
    cJSON *str_json,*task_json, *str_arder;
    cJSON *task_flag,*str_no,*str_week,*str_timestart,*str_timeend,*str_intervaltime,*str_gears,*str_worktime, *str_status ,*str_single;
    char *time_end,*time_start,*task_no;
    int start_hour,start_min,end_hour,end_min, week_start,week_end;
    str_json = cJSON_Parse(JSON);   //创建JSON解析对象，返回JSON格式是否正确
    if (!str_json)
    {
        printf("JSON格式错误:%s \r\n", cJSON_GetErrorPtr()); //输出json格式错误信息
		//	HAL_UART_Transmit(&huart3,(u8*)"JSON格式错误 \r\n", strlen("JSON格式错误 \r\n"), 100);    // 发送                
         cJSON_Delete(str_json);   //释放内存
         return 1;

    }
    else
    {
        //printf("JSON格式正确:%s \r\n",cJSON_Print(str_json) );
        switch (CJSON_TASK_TPYE)
        {
        case createTimerTask:
                task_json = cJSON_GetObjectItem(str_json, TASK); //获取task键对应的值的信息
                task_flag=cJSON_GetObjectItem(task_json,FLAG);
                if(task_flag->valueint==1)      //新增任务
                {
                    str_no = cJSON_GetObjectItem(task_json, NO);   //任务编号
                    task_no=str_no->valuestring;

                    str_week = cJSON_GetObjectItem(task_json, WEEK);   //解析周几工作
                    sscanf(str_week->valuestring,"%d-%d",&week_start,&week_end);
                    str_single  = 	cJSON_GetObjectItem(task_json,SINGLE);	        //任务是否单次
                    str_timestart = cJSON_GetObjectItem(task_json, START); 
                    time_start= str_timestart->valuestring;     
                    sscanf( time_start,  "%d:%d",&start_hour,&start_min);	//解析开始时间 
                    if( str_single->valueint==0) {
                        str_timeend = cJSON_GetObjectItem(task_json, END); 
                        time_end =str_timeend->valuestring;
                        sscanf( time_end,  "%d:%d",&end_hour,&end_min);	//解析结束时间 
                    } 
                    
                    str_intervaltime = cJSON_GetObjectItem(task_json, ECHO);    //间隔时间
                    str_gears = cJSON_GetObjectItem(task_json, LEVEL);              //工作等级
                    str_worktime = cJSON_GetObjectItem(task_json,TIME);             //工作时间
                    str_status  = 	cJSON_GetObjectItem(task_json,STATUS);	        //任务状态
                    
                    if(Cjson_Buf.size<CJSON_BUF_DATA_LEN)
                    {
                        strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].timestart,time_start);
                        
                         strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].task_no,task_no);
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].week_start=         week_start; 
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].week_end=           week_end;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].interval_time=      str_intervaltime->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].gears=              str_gears->valueint;                   
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].worktime=           str_worktime->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].status=             str_status->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_start_hour=    start_hour;    
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_start_min=     start_min;
                        if( str_single->valueint==0)        //单次任务没有结束时间
                        {
                            Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_hour=      end_hour;   
                            Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_min=       end_min;
                            strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].timeend,time_end);
                        }
                        else
                        {
                            strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].timeend,"0");
                             Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_hour=0;   
                            Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_min=0;                           
                        }
                          Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].once_task=       str_single->valueint;                  
                        Cjson_Buf.size++;																				
                    W25QXX_Write((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);
					printf("set ok\r\n");	
                    }
                    //else
                    //{
                    //    printf("设置满了，请清除旧设置");
                    //}

                }
                else if(task_flag->valueint==2)     //修改
                {
                    u8 no_num=0;
                    str_no = cJSON_GetObjectItem(task_json, NO);   //解析任务编号
                    task_no=str_no->valuestring;

                    str_week = cJSON_GetObjectItem(task_json, WEEK);   //解析周几工作
                    sscanf(str_week->valuestring,"%d-%d",&week_start,&week_end);

                    str_timestart = cJSON_GetObjectItem(task_json, START); 
                    time_start= str_timestart->valuestring;     
                    sscanf( time_start,  "%d:%d",&start_hour,&start_min);	//解析开始时间  
                    str_timeend = cJSON_GetObjectItem(task_json, END); 
                    time_end =str_timeend->valuestring;
                    sscanf( time_end,  "%d:%d",&end_hour,&end_min);	//解析结束时间  
                    
                    str_intervaltime = cJSON_GetObjectItem(task_json, ECHO);    //间隔时间
                    str_gears = cJSON_GetObjectItem(task_json, LEVEL);              //工作等级
                    str_worktime = cJSON_GetObjectItem(task_json,TIME);             //工作时间
                    str_status  = 	cJSON_GetObjectItem(task_json,STATUS);	        //任务状态	
                    str_single  = 	cJSON_GetObjectItem(task_json,SINGLE);	        //任务状态
                    while(strcmp(task_no,Cjson_Buf.Cjson_Buffer_Data[no_num].task_no)!=0)   //找对应的任务编号进行修改
                    {
                        if(no_num<Cjson_Buf.size)
                            no_num++;
                        else
                        {
                            break;
                        }
                    }
                    if(no_num<Cjson_Buf.size)
                    {
                        strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].timestart,time_start);
                        strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].timeend,time_end);
                        strcpy(Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].task_no,task_no);
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].week_start=         week_start; 
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].week_end=           week_end;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].interval_time=      str_intervaltime->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].gears=              str_gears->valueint;                   
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].worktime=           str_worktime->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].status=             str_status->valueint;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_start_hour=    start_hour;    
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_start_min=     start_min;
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_hour=      end_hour;   
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].time_end_min=       end_min; 
                        Cjson_Buf.Cjson_Buffer_Data[Cjson_Buf.size].once_task=       str_single->valueint; 
						W25QXX_Write((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);
						printf("set ok\r\n");

                    }
                }
                else if(task_flag->valueint==3)     //删除
                {
                    u8 num=0,no_num=0;
                    str_no = cJSON_GetObjectItem(task_json, NO);   //解析任务编号
                    task_no=str_no->valuestring;

                    while(strcmp(task_no,Cjson_Buf.Cjson_Buffer_Data[no_num].task_no)!=0)   //找对应的任务编号进行删除
                    {
                        if(no_num<Cjson_Buf.size)
                            no_num++;
                        else
                        {
                            break;;
                        }
                    }
                                     
                    for(num=no_num;num<Cjson_Buf.size;num++)		//删除对应工作组数据内容
                    {                   
                        Cjson_Buf.Cjson_Buffer_Data[num]= Cjson_Buf.Cjson_Buffer_Data[num+1];
                        memset(&Cjson_Buf.Cjson_Buffer_Data[num+1],0,sizeof(Cjson_Buf.Cjson_Buffer_Data[num+1]));
                    }
                    if(Cjson_Buf.size>0 && no_num<Cjson_Buf.size)       //任务组计数减一
                        Cjson_Buf.size--; 
                    W25QXX_Write((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);
                   // memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断
                    printf("序号:%d \r\n", no_num); 
                }
        break;
        case createRTCTask:
                str_arder = cJSON_GetObjectItem(str_json, "arder"); //获取task键对应的值的信息
            if(strstr(str_arder->valuestring,SET_RTC_TIME)!=0)		//设置RTC时间
           {
                RTC_TimeTypeDef sTime = {0};
                RTC_DateTypeDef sDate = {0};
                int  year,month,date,hour,min,sec,week;
                cJSON *str_date,*str_time,*str_week;
                char *data_date,*data_time;		 		
                str_date = cJSON_GetObjectItem(str_json,"set_date");
                data_date = str_date->valuestring;
                str_time = cJSON_GetObjectItem(str_json,"set_time");
                data_time = str_time->valuestring;
                str_week = cJSON_GetObjectItem(str_json,"week");
                week = str_week->valueint;
                sscanf(data_date, "%x-%x-%x", &year, &month, &date);
                sscanf(data_time, "%x:%x:%x", &hour, &min, &sec);

                sTime.Hours=hour;
                sTime.Minutes=min;
                sTime.Seconds=sec;
                sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                sTime.StoreOperation = RTC_STOREOPERATION_RESET;
                HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD); //设置时间	
                sDate.WeekDay = week;
                sDate.Year=year;
                sDate.Month=month;
                sDate.Date=date;
                HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);//设置时间	
                //HAL_UART_Transmit(&huart3,(u8*)"ok \r\n", strlen("ok \r\n"), 100);    // 发送                
            }
            else if(strstr(str_arder->valuestring,del)!=0)
           {
                    cJSON *str_serial;
                    u8 num;
                    str_serial = cJSON_GetObjectItem(str_json, "serial");   //获取serial键对应的值的信息
                    if(str_serial->type==cJSON_Number)
                    {
                        printf("序号:%d \r\n", str_serial->valueint);                   
                    }        
                    for(num=str_serial->valueint;num<Cjson_Buf.size;num++)		//删除对应工作组数据内容
                    {                   
                        Cjson_Buf.Cjson_Buffer_Data[num]= Cjson_Buf.Cjson_Buffer_Data[num+1];
                        memset(&Cjson_Buf.Cjson_Buffer_Data[num+1],0,sizeof(Cjson_Buf.Cjson_Buffer_Data[num+1]));
                    }
                    if(Cjson_Buf.size>0)
                        Cjson_Buf.size--; 
                    W25QXX_Write((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);
             //       memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断
           }
            else
            {
                RTC_TimeTypeDef sTime = {0};
                RTC_DateTypeDef sDate = {0};
                int  year,month,date,hour,min,sec,week;
                cJSON *str_date,*str_week;
                char *data_date,*data_week;		
                str_date = cJSON_GetObjectItem(str_json,"date");
                data_date = str_date->valuestring;
                str_week = cJSON_GetObjectItem(str_json,"week");
                data_week = str_week->valuestring;
                sscanf(data_date, "%x-%x-%x %x:%x:%x", &year, &month, &date,&hour, &min, &sec);
                sscanf(data_week, "%d", &week);
                sTime.Hours=hour;
                sTime.Minutes=min;
                sTime.Seconds=sec;
                sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
                sTime.StoreOperation = RTC_STOREOPERATION_RESET;
                HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD); //设置时间	
                sDate.WeekDay = week;               
                sDate.Year=(u8)year;
                sDate.Month=month;
                sDate.Date=date;
                HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);//设置时间	
               // HAL_UART_Transmit(&huart3,(u8*)"ok \r\n", strlen("ok \r\n"), 100);    // 发送                
  
            }
        break;
        case createClearTask:
            memset(&Cjson_Buf,0,sizeof(Cjson_Buf));//删除所有任务
            W25QXX_Write((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);    //写flash
        //    memset(&work_time,0,sizeof(work_time));//每次删除操作后都要复位重新判断
        break;
        default:
            break;
        } 
            beep_on();
            vTaskDelay(200);	
            beep_off();       
            cJSON_Delete(str_json);   //释放内存
            return 0;
    }
   
}


