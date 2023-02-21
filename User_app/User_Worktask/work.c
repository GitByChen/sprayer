#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "rtc.h"
#include "work.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "cJSON_user.h"
#include "RC522S.h"
#include "time.h"
//�����ź������
extern SemaphoreHandle_t MutexSemaphore;	//�����ź���

//_calendar_obj calendar;//ʱ�ӽṹ�� 
_work_time work_time={0,0,0,0,0,0,0}; 		//���ʱ��ϲ�����ַ���������Cjson���ݱȽ�  
extern	RTC_DateTypeDef GetData;    //����
extern	RTC_TimeTypeDef GetTime;    //ʱ��

/*ʱ������ɣ���λ����*/
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

/*get_time_ms:�˷����ϲ��������ַ���,����ʱ����˻��������*/
char * get_time_ms(void){

//struct tm tm_new;

char * time_str = NULL,*time_stemp={"%d%03d"};
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

sprintf(time_str,time_stemp,now,0);

//snprintf(time_str, time_str_len, "%d000\r\n", now);
printf("stm=%d,time_str=%s",now,time_str);		//�˷����ϲ��������ַ���ʱ����˻��������
return time_str;

}
/*											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //???????????	  

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮��� 
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}
*/
void work_task(void const * argument )
{
	static u8 minute_interval_Cheak=0,minute_Cheak=0;
	for( ; ; ) 
	{	
	//	xSemaphoreTake(MutexSemaphore,portMAX_DELAY);		//��ȡ�����ź���
	//HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);	//��ȡʱ��
    //HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);	//��ȡ����
	//		calendar.week=RTC_Get_Week(GetData.Year+2000,GetData.Month,GetData.Date);//��ȡ����
	//	printf("week=%d\r\n",GetData.WeekDay);
//	printf("%d-%d-%d %d:%d:%d\n",GetData.Year,GetData.Month,GetData.Date,GetTime.Hours,GetTime.Minutes,GetTime.Seconds);//�������ʱ��
		if(Pcd_Massage_Flag.Have_A_Card==PCD_OK)
		{
			if((work_time.work_time_flag==0 && GetTime.Seconds<=30 && minute_Cheak==0) )	//ÿ��0�����Ƿ��й����鹤��
			{
				minute_Cheak=1;		//ÿ���Ӽ��һ�����
				work_time.which_working_time=0;//�Լ�¼ֵ��������
				while(work_time.which_working_time<Cjson_Buf.size)
				{	
					if(GetData.WeekDay>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].week_start 
						&& GetData.WeekDay<=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].week_end)
					{
						
						if((GetTime.Hours*60 +GetTime.Minutes)==(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) 
							&& Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1)
						{
							
								if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].status==1 && work_time.working_once==0)
								{
									work_time.work_time_flag=1;
									work_time.working_flag=0;
									printf("�ڵ�%d�������鹤��\r\n",work_time.which_working_time);
									break;
								}
								else
								{
									work_time.work_time_flag=0;
									work_time.working_once=0;
									work_time.which_working_time++;	//��¼�ĸ��������ڹ���
								}														
							
						}
						else if((GetTime.Hours*60 +GetTime.Minutes)>=(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) && 
							(GetTime.Hours*60 +GetTime.Minutes)<=(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_hour *60 +Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_min))
						{
							if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].status==1)
							{
								work_time.work_time_flag=1;
								work_time.working_flag=0;
								printf("�ڵ�%d�������鹤��\r\n",work_time.which_working_time);
								break;
							}
							else
							{
								work_time.work_time_flag=0;
								work_time.working_once=0;
								work_time.which_working_time++;	//��¼�ĸ��������ڹ���
							}														
						}
						else
						{
							work_time.work_time_flag=0;
							work_time.working_once=0;
							work_time.which_working_time++;	//��¼�ĸ��������ڹ���
						}
					}
					else
					{
						work_time.work_time_flag=0;
						work_time.working_once=0;
						work_time.which_working_time++;	//��¼�ĸ��������ڹ���
					}

				}

				if(work_time.work_time_flag!=1)
				{
					work_time.which_working_time=0xff;	//û�й��������
				}

			}
			else if(GetTime.Seconds>=40 && minute_Cheak==1)
			{
				minute_Cheak=0;
			}
								
			if(work_time.work_time_flag==1 && GetTime.Seconds<40 && minute_interval_Cheak==0)//ÿ�����ж�һ��
			{
				minute_interval_Cheak=1;
				//printf("%d-%d-%d %d:%d:%d\n",GetData.Year,GetData.Month,GetData.Date,GetTime.Hours,GetTime.Minutes,GetTime.Seconds);//�������ʱ��					
					if(GetData.WeekDay>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].week_start 
						&& GetData.WeekDay<=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].week_end)
					{
						if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1  )
						{	
							if(work_time.working_once==1){
								memset(&work_time,0,sizeof(work_time));//���ʱ����� ����λ�����ж�						
							}												
							
						}
						else if((GetTime.Hours*60 +GetTime.Minutes)<(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_hour*60 +Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_start_min) || 
								(GetTime.Hours*60 +GetTime.Minutes)>(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_hour *60 +Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].time_end_min))
							{
								if(work_time.working_flag==0 ||work_time.working_flag==3)			//��������Ǽ��ʱ���ڻ��߹���ʱ��ҲҪ�ȱ��ι�����ɺ��ٸ�λ
									memset(&work_time,0,sizeof(work_time));//���ʱ����� ����λ�����ж�
							}
					}
					else
					{
						if(work_time.working_flag==0||work_time.working_flag==3)	//����ʱ��ҲҪ�ȱ��ι�����ɺ��ٸ�λ
							memset(&work_time,0,sizeof(work_time));//ʱ����� ����λ�����ж�
					}

				if(work_time.work_time_flag==1)
				{
					if(work_time.working_flag==3 && work_time.working_once==0)		//������ɼ�ʱ��һ�ι���,ֻ�й����겢�Ҳ��ǲ��ǵ�������ż���
					{
						if((work_time.working_interval_time)>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].interval_time)
						{
							work_time.working_flag=0;
							work_time.working_interval_time=0;
						}
						else
						{
							work_time.working_interval_time++;
						}
						
					}

					if(work_time.working_flag==0 )  //�ڹ���ʱ���ڣ���ȷ�ϴ˹�����Ϊ����״̬
					{
						work_time.working_flag=1;		//ʱ��ε��ˣ�������־λ��1
						//Pcd_Massage_Flag.Pcd_Read_Flag=0; 			//������ʼ�����������־λ�����ж���
						//Pcd_Massage_Flag.Pcd_Write_Flag=1;				//����������д����־λ��ȥд��
					} 
				}
			}			
			else if(GetTime.Seconds>=40 && minute_interval_Cheak==1)
			{
				minute_interval_Cheak=0;
			}
			if(work_time.working_flag==1 &&Pcd_Massage_Flag.Pcd_Write_Flag==3) //Ҫ�ϱ���������ٹ���
			{
					work_time.working_flag=2;
					work_time.working_interval_time=1;			//������ʼ��ͬʱ��ʼ�������ʱ��
					work_time.working_time=0;					//����ʱ��������㣬��ʼ����
					Motor_Working(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].gears);
				HAL_UART_Transmit(&huart3,(u8*)"����!", strlen("����!"), 100);    //
			}
			else if(work_time.working_flag==2)
			{
				if(work_time.working_time>=Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].worktime)
				{	
					work_time.working_flag=3;					//					
					work_time.working_time=0;
										
					Pcd_Massage_Flag.Pcd_Read_Flag=0; 			//������ʼ�����������־λ�����ж���
					Pcd_Massage_Flag.Pcd_Write_Flag=0;				//��������д����־λ����
					Motor_Working(0);		//
					if(Cjson_Buf.Cjson_Buffer_Data[work_time.which_working_time].once_task==1)	//����ǵ��ι�������λ��־λΪ1
					{
						work_time.working_once=1;	
					}
					else
					{
						work_time.working_once=0;	
					}
					HAL_UART_Transmit(&huart3,(u8*)"��������!", strlen("��������!"), 100);    
				}
				else
				{
					work_time.working_time++;
				}
			}			
		}
		else 
		{
			memset(&work_time,0,sizeof(work_time));//���û�п� ����λ�����ж�

		}
		/*switch (calendar.week)
		{
		case 1:
			work_time.week="����һ";
			break;
		case 2:
			work_time.week="���ڶ�";
			break;
		case 3:
			work_time.week="������";
			break;
		case 4:
			work_time.week="������";
			break;
		case 5:
			work_time.week="������";
			break;
		case 6:
			work_time.week="������";
			break;
		case 7:
			work_time.week="������";
			break;
		default:
			break;
		}*/		
//		xSemaphoreGive(MutexSemaphore);					//�ͷ��ź���
		vTaskDelay(1000);
	}
	
}












