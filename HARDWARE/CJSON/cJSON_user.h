#ifndef __CJSON_USER_H
#define __CJSON_USER_H
#include "stdio.h"
#include "usart.h"	
#include "main.h"


#define pwm_size                "pwm_size="
#define add                     "add"
#define del                     "del"
#define CHEAK_TASK              "cheak_task"
#define SET_RTC_TIME            "set_rtc_time"

#define SN          "sn"            //�豸���
#define IMEI        "imei"          //�豸IMEI��
#define NO          "no"            //������
#define TIMESTAMP   "timestemp"     //ʱ���
#define TASK        "task"          //����
#define WEEK        "week"          //��
#define START       "start"         //��ʼʱ�� mm:dd
#define END         "end"           //����ʱ�� mm:dd
#define ECHO        "echo"          //�ظ�ʱ��  ��λ��min
#define LEVEL       "level"         //�����ȼ�
#define TIME        "time"          //����ʱ��
#define STATUS      "status"        //����״̬ (�Ƿ�����)
#define FLAG        "flag"          //��־λ ("��ʶλ 1.����;2.�޸�;3.ɾ��;")
#define SINGLE      "single"        //��������

typedef enum 
{
    createTimerTask=1,      //��������
    createRTCTask,          //RTCʱ���������
    createClearTask,         //�����������
}TPYE;

//extern enum TPYE CJSON_TASK_TPYE;
typedef struct{
    
    char Task_Buf[200];
    char task_no[15];           //���������
    char timestart[10];        //��ʼʱ���ַ���
    char timeend[10];          //����ʱ���ַ���
    u8 week_start;          //���ڼ���ʼ
    u8 week_end;            //���ڼ�����
    u8 time_start_hour;     //���������Ŀ�ʼʱ��
    u8 time_start_min;
    u8 time_end_hour;       //���������Ľ���ʱ��
    u8 time_end_min;
    u8 status;           //�Ƿ�����
    u8 once_task;       //�Ƿ񵥴�����
    u8 interval_time;   //������� ��λ��min
    u8 gears;           //������λ ��λ��min
    u16 worktime;       //����ʱ�� ��λ��s

}Cjson_Buffer;

#define CJSON_BUF_LEN           200
#define CJSON_BUF_DATA_LEN      20 
typedef struct {
    u8 size;        //���������
    Cjson_Buffer Cjson_Buffer_Data[CJSON_BUF_DATA_LEN];
}Cjson;
extern Cjson Cjson_Buf;

void Uart_task(void);

uint8_t ParseStr(const char *JSON,TPYE CJSON_TASK_TPYE);    //JSON:��Ҫ���������ݣ�type����������

#endif

