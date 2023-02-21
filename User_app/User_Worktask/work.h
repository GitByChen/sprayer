#ifndef __WORK_H
#define __WORK_H	    
#include	"stm32l4xx_hal_rtc.h"
/*
//ʱ��ṹ��
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//������������
	u16 year;
	u8  month;
	u8  date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��
*/
typedef struct 
{	
	char *week;
	char *time;
	u8 work_time_flag;	//�Ƿ��ڹ���ʱ�����
	u8 working_flag;	//�Ƿ����ڹ���	0:û�й�����1��׼��������2�����ڹ�����3���������
	u8 working_time;	//����ʱ�����
	u8 which_working_time;  //��¼���ĸ��������ڹ���
	u8 working_once;		//���ι�����ɱ�־λ 0��δ��ɡ�1�������
	u16 working_interval_time;	//�������ʱ�����
}_work_time;
extern _work_time work_time; 
	
extern u8 const mon_table[12];	//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��
int32_t time_to_timestamp(void);		/*ʱ������ɣ���λ����*/
char * get_time_ms(void);				/*ʱ������ɣ���λ������*/
#endif


