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

#define SN          "sn"            //设备编号
#define IMEI        "imei"          //设备IMEI号
#define NO          "no"            //任务编号
#define TIMESTAMP   "timestemp"     //时间戳
#define TASK        "task"          //任务
#define WEEK        "week"          //周
#define START       "start"         //开始时间 mm:dd
#define END         "end"           //结束时间 mm:dd
#define ECHO        "echo"          //重复时间  单位：min
#define LEVEL       "level"         //工作等级
#define TIME        "time"          //工作时间
#define STATUS      "status"        //任务状态 (是否启用)
#define FLAG        "flag"          //标志位 ("标识位 1.新增;2.修改;3.删除;")
#define SINGLE      "single"        //单次任务

typedef enum 
{
    createTimerTask=1,      //任务设置
    createRTCTask,          //RTC时间更新设置
    createClearTask,         //任务清空设置
}TPYE;

//extern enum TPYE CJSON_TASK_TPYE;
typedef struct{
    
    char Task_Buf[200];
    char task_no[15];           //存放任务编号
    char timestart[10];        //开始时间字符串
    char timeend[10];          //结束时间字符串
    u8 week_start;          //星期几开始
    u8 week_end;            //星期几结束
    u8 time_start_hour;     //解析出来的开始时间
    u8 time_start_min;
    u8 time_end_hour;       //解析出来的结束时间
    u8 time_end_min;
    u8 status;           //是否启用
    u8 once_task;       //是否单次任务
    u8 interval_time;   //工作间隔 单位：min
    u8 gears;           //工作档位 单位：min
    u16 worktime;       //工作时间 单位：s

}Cjson_Buffer;

#define CJSON_BUF_LEN           200
#define CJSON_BUF_DATA_LEN      20 
typedef struct {
    u8 size;        //工作组计数
    Cjson_Buffer Cjson_Buffer_Data[CJSON_BUF_DATA_LEN];
}Cjson;
extern Cjson Cjson_Buf;

void Uart_task(void);

uint8_t ParseStr(const char *JSON,TPYE CJSON_TASK_TPYE);    //JSON:需要解析的内容，type：解析类型

#endif

