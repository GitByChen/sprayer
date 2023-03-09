#ifndef __BC260Y_H
#define __BC260Y_H
#include "stdint.h"


typedef struct 
{
   char BC260_UART_BUF[280];
   char AT_ORDER_BUF[220];
   char BC260_IMEI[20];         //存放IMEI号
   char BC260_SN[20];            //存放SN号
   u8   BC260Y_OPEN_FLAG;     //是否开机
   u8   BC260Y_CONNECT_FLAG;  //是否成功连接服务器
   u8 bindFlag;               //是否绑定设备
   u8  send_error;            //用于在重量不合法时是否需要上报， 0：不需要，1：需要
   int CSG_val;                //网络信号值
}BC260_MASSAGE;
extern BC260_MASSAGE BC260_Massage;

#define AT_Order_Cheak          "AT"           //查询模块是否正常
#define AT_Order_Cheak_MQTT     "ATI"
#define AT_Order_CIMI           "AT+CIMI"      //查询是否有卡
#define AT_Order_CSQ            "AT+CSQ"       //查询是否有信号
#define AT_Order_CGATT          "AT+CGATT?"    //查询注册网络情况
#define AT_Order_CGIMEI         "AT+CGSN=1"    //查询IMEI
#define AT_Order_CGSN           "AT+CGSN=0"     //查询SN
#define AT_Order_REST           "AT+QREST=1"
#define AT_Order_CPSMS           "AT+CPSMS=0"
#define AT_MQTT_QMTOPEN         "AT+QMTOPEN=0,\"120.24.149.179\",1883"   //接入服务器
#define AT_MQTT_QMTCONN         "AT+QMTCONN=0,\"%s\",\"nebulizer\",\"ljrh1234\""           //发送模块ID

#define AT_MQTT_PUB_TIMING_REPORT          "AT+QMTPUB=0,0,0,0,\"ljrh/nebulizer/%s/deviceStatus\"" // 定时广播的主题
#define AT_MQTT_PUB_WEIGHT_REPORT           "AT+QMTPUB=0,0,0,0,\"ljrh/nebulizer/%s/weightIncrease\"" //上报重量不匹配的广播主题
#define AT_MQTT_PUB_TIMING_WAKEUP          "AT+QMTPUB=0,0,0,0,\"%s/BC260Y\"" //我叫什么 在广播

#define AT_MQTT_QMTPUB11          ">" // 广播正常字符
#define TimerTask                  "AT+QMTSUB=0,1,\"ljrh/nebulizer/%s/task\",0"   //工作时间任务
#define TimerClearTask             "AT+QMTSUB=0,1,\"ljrh/nebulizer/%s/clearTask\",0"   //工作时间清零任务
#define RtcTask                     "AT+QMTSUB=0,1,\"ljrh/nebulizer/%s/syncDate\",0"   //实时时间同步任务
#define BindTask                    "AT+QMTSUB=0,1,\"ljrh/nebulizer/%s/bind\",0"        //请求绑定设备
#define AT_MQTT_QMTSUB              "AT+QMTSUB=0,1"         //订阅
#define AT_MQTT_QMTPUB              "AT+QMTPUB=0,0,0,0"     //广播
#define AT_MQTT_QMTUNS              "AT+QMTUNS=0,1"         //退订
#define QOS0         0
#define QOS1         1
#define QOS2         2
/*
*|参数名|必选|类型|说明|
*|deviceSn |是  |string |设备SN号   |
*|deviceImei |是  |string | 设备IMEI号    |
*|bindFlag |是  |int | 设备是否绑定 1:是  0:否    |
*|mainStatus |是  |int | 是否启用 1:是  0:否 |
*|timeStamp |是  |long| 时间戳||
*/
#define JSON_BC260Y_MASSAGE     "{\"sn\":\"%s\",\"imei\":\"%s\",\"bind\":%d,\"status\":%d,\"timestamp\":\"%d000\",\"weight\":%d}"     //定时上报消息
#define HX711_Weight_Report      "{\"Card_ID\":%x,\"Weight\":%d}"    //工作完后重量上报
#define HX711_Weight_Error_Report "{\"sn\":\"%s\",\"timestamp\":\"%d\"}"
#define AT_Order_SLEEP          "AT+CPSMS="     //睡眠模式设置
#define AT_Order_RESP           "OK"            //指令返回
#define AT_Order_ERROE           "ERROR"        //指令发送失败
#define AT_RESP_CIMI            "460"           //查询卡号返回
#define AT_RESP_CGATT           "+CGATT"       //查询注册网络情况返回
#define AT_RESP_CSQ             "AT+CSQ"         //查询网络信号
#define AT_RESP_INIT            "+IP:"          //判断4G模块是否启动
#define AT_ORDER                "AT+"           //AT指令
#define AT_RESP_CGSN             "AT+CGSN="
#define AT_Order_CFG             "AT+QMTCFG=\"session\",0,0"
#define AT_Order_QMTCFG          "AT+QMTCFG=\"keepalive\",0,0"
#define AT_MQTT_RESP_QMTOPEN    "+QMTOPEN: 0,0"     //接入指令返回字符
#define AT_MQTT_RESP_QMTCONN    "+QMTCONN: 0,0,0"   //登录指令返回状态
#define AT_MQTT_RESP_MQTSUB     "+QMTSUB: 0,1"  //   订阅成功
#define AT_MQTT_RESP_QMTUNS     "+QMTUNS: 0,1"    //   退阅成功

#define AT_MQTT_RESP_MQTPUB1          ">" //我叫什么 在广播
#define AT_MQTT_RESP_MQTPUB     "+QMTPUB: 0,0,0"    //发布成功
#define AT_MQTT_RESP_QMTSTAT     "+QMTSTAT:"
#define AT_MQTT_RESP_QMTRECV     "+QMTRECV:"
#define FALL_SALEEP         1
#define EXIT_SALEEP         0
#define AT_CGATT_OPEN           "=1"

extern u8 g_mqtt_init_ok;          //判断是否MQTT初始化完成

void Mqtt_BC260Y_Task_Init(void);
void MQTT_Receive_Data(void);        
u8    BC260Y_Init(void);  //对设备初始化
void    MQTT_Init(void);    //mqtt初始化
u8 MQTT_REST(void);        //MQTT复位
u8 MQTT_QMTOPEN(void);     //连接服务器
u8 MQTT_QMTCONN(void);     //登录服务器
void connect_mqtt(void);

u8 MQTT_Subscribe(char *QMTSUB);  //订阅
u8 MQTT_Publish(const char *str, char *sub);//广播
void TimerSub(void); //定时广播

#endif

