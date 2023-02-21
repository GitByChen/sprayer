#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "tim.h"	
#include "w25q64.h"
#include "cJSON.h"
#include "cJSON_user.h"
#include "BC260Y.h"
#include "RC522S.h"
#include "HX711.h"
#include "work.h"
//#include "cron.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"

//互斥信号量句柄
extern SemaphoreHandle_t MutexSemaphore;	//互斥信号量

BC260_MASSAGE BC260_Massage; 

char *strx;
char JSON_MASSAGE[200]={0}; //存放定时上报的数据
char t_json[70];
void MQTT_SendStr(char* pData)
{
	char *data=pData;
	u16 len;
	len=strlen((char* )pData);
		HAL_UART_Transmit(&huart1, (u8 *)data,len, 10000);    // 发送回去
		HAL_UART_Transmit(&huart1, (u8 *)"\r\n",strlen("\r\n"), 1000);    // 发送回去

}

u8 Clear_Buffer(const char *resp ,const char *order)     //清楚缓存
{
    u8 i=0;
    MQTT_Receive_Data();
    while (strstr((const char*)BC260_Massage.BC260_UART_BUF,resp)==0)
    {
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
            return 1;
        }
        else if(i>=10)
        {
            return 1;
        }
        else
        {
            i++;
					MQTT_SendStr((char*) order);	//发送
           // USART3_SendStr((u8 *)order);
            vTaskDelay(300);
            MQTT_Receive_Data();
            //strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,resp);
        }
				 vTaskDelay(300);  
    }

     return 0;
}


u8  BC260Y_Init(void)//对设备初始化
{
    //char *temp;
    MQTT_SendStr(AT_Order_CPSMS); //推出休眠模式
    vTaskDelay(300);  
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CPSMS))
    {
        return 1;
    }
    MQTT_SendStr(AT_Order_CFG); //发送”session“设置，获取掉电期间服务器下发的消息
    vTaskDelay(300);  
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CFG))
    {
        return 1;
    }
   /* USART3_SendStr(AT_Order_CIMI);    //查询是否有卡
    vTaskDelay(300);
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CIMI))
    {
         return 1;
    } 

    USART3_SendStr(AT_Order_CGATT);    //查询是否有卡
    vTaskDelay(300);
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CIMI))
    {
         return 1;
    }
    sscanf((const char*)BC260_Massage.BC260_UART_BUF,"%*[^:]:%d",&ret);  
    if(ret==0)
    {
        strcpy(temp,AT_Order_Cheak);
        strcpy(temp,AT_RESP_CGATT);
        strcpy(temp,AT_CGATT_OPEN);   
        USART3_SendStr((u8 *)temp);           //发送激活网络指令
        vTaskDelay(300);
        Clear_Buffer(AT_Order_RESP,temp);   //如果有卡，直接输出   
    }*/
	MQTT_SendStr(AT_Order_CGIMEI);	//发送
    vTaskDelay(300);
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CGIMEI)==1)
    {
        return 1;
    }
    sscanf(BC260_Massage.BC260_UART_BUF, "%*[^: ]: %[^\r]",BC260_Massage.BC260_IMEI);
		 printf("imei=%s\r\n",BC260_Massage.BC260_IMEI);
    if(BC260_Massage.BC260_IMEI==0)
    {
        return 1;
    }
    memset(BC260_Massage.BC260_UART_BUF,0,UART1_REC_LEN);	//清除缓冲区
    vTaskDelay(300);
		
		MQTT_SendStr(AT_Order_CGSN);	//发送
    vTaskDelay(300);
    if(Clear_Buffer(AT_Order_RESP,AT_Order_CGSN)==1)
    {
        return 1;
    }
    sscanf(BC260_Massage.BC260_UART_BUF, "%*[^M]%[^\r]\r",BC260_Massage.BC260_SN);
    printf("sn=%s\r\n",BC260_Massage.BC260_SN);
    if(strstr(BC260_Massage.BC260_SN,"MPN")==0)
     {
        return 1;
     }    
    vTaskDelay(300);
    return 0;
}
u8 MQTT_REST(void)
{
		MQTT_SendStr(AT_Order_REST);	//发送
    //USART3_SendStr(AT_Order_REST); //发送”AT“
    vTaskDelay(300);  
    if(Clear_Buffer(AT_Order_RESP,AT_Order_REST))
    {
        return 1;
    }
     vTaskDelay(300);  
    return 0;
   
}
u8 MQTT_QMTOPEN(void)
{
     u8 i=0;
		MQTT_SendStr( AT_MQTT_QMTOPEN);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();
    strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTOPEN);//确认返回值正确
    while(strx==NULL)
    {
        MQTT_Receive_Data();
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0){
					BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        if(i>=30){
			//BC260_Massage.BC260Y_CONNECT_FLAG=0;
            //return 1;
            break;
        }else{
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTOPEN);//确认返回值正确
            i++;
        }
        vTaskDelay(300);
    }
    vTaskDelay(100);
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}
u8 MQTT_QMTCONN(void)
{
    u8 i=0;
    char t_json[100];
    //printf("sn=%s\r\n",BC260_Massage.BC260_SN);         //使用SN码作为设备名
    sprintf(t_json, AT_MQTT_QMTCONN,BC260_Massage.BC260_SN);
		MQTT_SendStr(t_json);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();
    strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTCONN);//确认返回值正确
     while(strx==NULL)
    {
        MQTT_Receive_Data();
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0 || strstr(BC260_Massage.BC260_UART_BUF,AT_MQTT_RESP_QMTSTAT)!=0)
        {
						BC260_Massage.BC260Y_CONNECT_FLAG=0;
					 return 1;
        }
        else if(i>=30 ){
			//			BC260_Massage.BC260Y_CONNECT_FLAG=0;
            //return 1;
            break;
        }
        else{
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTCONN);//确认返回值正确
            i++;
        }
         vTaskDelay(300);
    }
    vTaskDelay(100); 
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}
/*
* 订阅
*QMTSUB:订阅主题
*QOS:QOS等级
*/
u8 MQTT_Subscribe(char *QMTSUB,u8 QOS)
{
    u8 i=0;
    char Subscribe[]={"%s,\"%s\",%d"},save_sub[70];

    sprintf(save_sub,Subscribe,AT_MQTT_QMTSUB,QMTSUB,QOS);
    //printf("sn=%s\r\n",BC260_Massage.BC260_SN);         //使用SN码作为设备名
	MQTT_SendStr(save_sub);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();
    strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTSUB);//确认返回值正确
     while(strx==NULL)
    {
        MQTT_Receive_Data();
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
					BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        else if(i>=30)
        {
			//BC260_Massage.BC260Y_CONNECT_FLAG=0;
           // return 1;
            break;
        }
        else
        {
            i++;
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTSUB);//确认返回值正确
        }
        vTaskDelay(300);
    }
    vTaskDelay(100);
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
   return 0;
}

u8 MQTT_UnSubscribe(char *QMTSUB,u8 QOS)      //退订
{
    char Subscribe[]={"%s,\"%s\",%d"},save_sub[70];
    sprintf(save_sub,Subscribe,AT_MQTT_QMTSUB,QMTSUB,QOS);
	MQTT_SendStr(QMTSUB);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();
    strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTUNS);//确认返回值正确
     while(strx==NULL)
    {
        MQTT_Receive_Data();
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
			BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        else
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_QMTUNS);//确认返回值正确
    }
   vTaskDelay(300);
   return 0;
}

/*
* 广播
*str:广播主题
* sub:广播内容
*/
u8 MQTT_Publish(const char *str, char *sub)
{
    u8 i=0;
    u8 t=26;
	u16 len;
    char Subscribe[]={"%s,\"%s\""},save_sub[70];
    sprintf(save_sub,Subscribe,AT_MQTT_QMTPUB,str);
	MQTT_SendStr((char *)save_sub);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();
   /* strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTPUB1);//看下返回状态
    while(strx==NULL)
    {
        MQTT_Receive_Data();
         if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
			BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        if(i>=5)        //第一次发了后没反应，1.5S后再出去发送一次
        {			
			break;
        }
        else{
            i++;
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTPUB1);//看下返回状态
        }
        vTaskDelay(300);
    }
    i=0;*/			
		len=strlen(sub);
		HAL_UART_Transmit(&huart1, (u8* )sub,len, 10000);    // 发送		
		HAL_UART_Transmit(&huart1, &t,1, 10000);    // 发送
    vTaskDelay(300);
    MQTT_Receive_Data();
    strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTPUB);//看下返回状态
    while(strx==NULL)
    {
        MQTT_Receive_Data();
         if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
					BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        if(i>=5)
        {
			break;
        }
        else
        {
            i++;
            strx=strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_MQTT_RESP_MQTPUB);//看下返回状态
        }
         vTaskDelay(300);
    }
    BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}

/*
*订阅主题、广播主题拼接函数
*/
void MQTT_joint(void)
{
    //printf("sn=%s\r\n",BC260_Massage.BC260_SN);         //使用SN码作为设备名
    sprintf(BC260_Massage.AT_MQTT_SUBJECT, TimerTask,BC260_Massage.BC260_SN);      //订阅用的任务设置主题
    sprintf(BC260_Massage.AT_MQTT_SUB_CLEAR_TASK, TimerClearTask,BC260_Massage.BC260_SN);//订阅用的任务清零设置 主题
    sprintf(BC260_Massage.AT_MATT_SUB_RTC_TASK, RtcTask,BC260_Massage.BC260_SN); //接收时间同步 主题
    sprintf(BC260_Massage.AT_MQTT_SUB_BIND_TASK, BindTask,BC260_Massage.BC260_SN); //请求绑定设备 主题

    sprintf(BC260_Massage.AT_MQTT_PUB, AT_MQTT_PUB_TIMING_REPORT,BC260_Massage.BC260_SN);//广播用的主题
    sprintf(BC260_Massage.AT_MQTT_PUB_WEIGHT, AT_MQTT_PUB_WEIGHT_REPORT,BC260_Massage.BC260_SN); //广播上报重量异常 主题

   // printf("sub=%d,pub=%d,sub2=%d\r\n",strlen(BC260_Massage.AT_MQTT_SUBJECT),strlen(BC260_Massage.AT_MQTT_PUB),strlen(BC260_Massage.AT_MQTT_SUB_CLEAR_TASK));
}
/*
*MQTT接收成功返回函数
*/
void MQTT_reply(char *subject,char *resp)
{
    u8 num=0;
    while(MQTT_Publish(subject,resp)==1)
    {   
        if(num>=2)
        {
            MQTT_QMTOPEN();   //对设备初始化								
            MQTT_QMTCONN();
            MQTT_Subscribe(BC260_Massage.AT_MQTT_SUBJECT,QOS0);
            MQTT_Subscribe(BC260_Massage.AT_MQTT_SUB_CLEAR_TASK,QOS0);
            break;
        }
        num++;
    }
    BC260_Massage.BC260Y_CONNECT_FLAG=1;
    Send_timing_wakeup=0;   

}
/*定时发送函数*/
void TimerSub(void)
{
    Send_Timeout=0;     //计数清零
    Send_timing_wakeup=0; 
    memset(&JSON_MASSAGE,0,sizeof(JSON_MASSAGE));   
    sprintf(JSON_MASSAGE,JSON_BC260Y_MASSAGE,BC260_Massage.BC260_SN,BC260_Massage.BC260_IMEI, BC260_Massage.bindFlag ,1, time_to_timestamp(),HX711_Massage.Write_To_Card_Weight);//time_to_timestamp():获取秒时间戳；get_time_ms()：毫秒时间戳             
    // printf("size=%d\r\n",strlen(JSON_MASSAGE));
    if(MQTT_Publish(BC260_Massage.AT_MQTT_PUB,JSON_MASSAGE)==1)
    {
        // BC260Y_Init();//对设备初始化                       
        connect_mqtt();
    }
}
/*定时发送函数*/

/*
*串口1接收函数处理
*/
void MQTT_Receive_Data(void)
{
 
  if(UART1_RX_STA &0x8000)
	{   
           
		memset(&BC260_Massage.BC260_UART_BUF,0,sizeof(BC260_Massage.BC260_UART_BUF));
		strcpy(BC260_Massage.BC260_UART_BUF,(char*)UART1_RX_Buffer);
		memset(&UART1_RX_Buffer,0,sizeof(UART1_RX_Buffer));				//清除接收
		UART1_RX_STA = 0;
			HAL_UART_Transmit(&huart3, (u8 *)BC260_Massage.BC260_UART_BUF,strlen((char* )BC260_Massage.BC260_UART_BUF), 10000);    // 串口3打印
	//	memset(&UART1_RX_Buffer,0,sizeof(UART1_RX_Buffer));
		if(strstr(BC260_Massage.BC260_UART_BUF,AT_MQTT_RESP_QMTSTAT)!=0  )   //看下返回状态
        {
                BC260_Massage.BC260Y_CONNECT_FLAG=0;
        }
        else if(strstr(BC260_Massage.BC260_UART_BUF,AT_RESP_INIT)!=0 )   //看下返回状态
        {
                BC260_Massage.BC260Y_OPEN_FLAG=1;                              
        }
        else if(strstr(BC260_Massage.BC260_UART_BUF,AT_MQTT_RESP_QMTRECV)!=0 )   //看下返回状态
        {
            char *Cjson_s="{%s}}",*json_order_buf;  
            sscanf(BC260_Massage.BC260_UART_BUF, "%*[^{]{%[^}]",BC260_Massage.AT_ORDER_BUF);
            json_order_buf=malloc(strlen(BC260_Massage.AT_ORDER_BUF)+10);       //加回"{}"所占的内存
            sprintf(json_order_buf,Cjson_s,BC260_Massage.AT_ORDER_BUF);        //对数据加回“{}”，再进行数据处理
            //printf("%s\r\n",BC260_Massage.BC260_UART_BUF);
            if(strstr(BC260_Massage.BC260_UART_BUF,BC260_Massage.AT_MQTT_SUBJECT)!=0 ) //任务设置
            {           
                ParseStr(json_order_buf,createTimerTask);    //处理Cjson格式数据                           
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,BC260_Massage.AT_MQTT_SUB_CLEAR_TASK)!=0)       //清空任务
            {
                ParseStr(json_order_buf,createClearTask);     //处理Cjson格式数据
                TimerSub();    //立即发送状态
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,BC260_Massage.AT_MATT_SUB_RTC_TASK)!=0 )       //时间设置任务
            {
                ParseStr(json_order_buf,createRTCTask);    //处理Cjson格式数据
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,BC260_Massage.AT_MQTT_SUB_BIND_TASK)!=0)
            {
                     ParseStr(json_order_buf,createBindTask);    //处理Cjson格式数据
                     TimerSub();    //立即发送状态           
            }
            free(json_order_buf);
        }       
	}

}
/*
*连接服务器操作
*/
void connect_mqtt(void)
{
    if(MQTT_QMTCONN()==0)
    {
    MQTT_Subscribe(BC260_Massage.AT_MQTT_SUBJECT,QOS0);
    MQTT_Subscribe(BC260_Massage.AT_MQTT_SUB_CLEAR_TASK,QOS0);
    MQTT_Subscribe(BC260_Massage.AT_MATT_SUB_RTC_TASK,QOS0);
     MQTT_Subscribe(BC260_Massage.AT_MQTT_SUB_BIND_TASK,QOS0);
    }
}

void BC260Y_task(void const * argument )
{
   BC260_Massage.bindFlag=1;    //默认
    for( ; ; ) 
    {      
//       xSemaphoreTake(MutexSemaphore,portMAX_DELAY);	//获取互斥信号量
        MQTT_Receive_Data();
        if((BC260_Massage.BC260Y_CONNECT_FLAG ==0 && Send_Timeout>=20) ||(BC260_Massage.BC260Y_CONNECT_FLAG ==0 &&BC260_Massage.BC260Y_OPEN_FLAG==1)) //初始化4G模块
        {
				BC260_Massage.BC260Y_CONNECT_FLAG=1; 
               if(BC260Y_Init()==1)  //对设备初始化 
                {
                    BC260_Massage.BC260Y_CONNECT_FLAG=0;
                }
                else
                {
                    MQTT_joint();
                    if(MQTT_QMTOPEN()==0)
                    {
                        connect_mqtt();
                        if(BC260_Massage.BC260Y_CONNECT_FLAG==1)
                        {
                            Send_Timeout=0;
                            Send_timing_wakeup=0;
                            beep_on();
                            vTaskDelay(500);	
                            beep_off();
                        }
                    }
                }
                          
        }
        else if(BC260_Massage.BC260Y_CONNECT_FLAG==1)
        {                       
			if( Send_Timeout>=120 )//超过15S对广播一次 定时器500Ms加一
            {
				TimerSub();               
            }
            else if(Pcd_Massage_Flag.Pcd_Read_Flag==1 && Pcd_Massage_Flag.Pcd_Write_Flag==2 )       //Pcd_Write_Flag 在完成写卡后置2
            {
                Send_timing_wakeup=0;
                 memset(&t_json,0,sizeof(t_json));
                //char json[]="{\"Card_ID\":%x,\"Weight\":%d}";                                
                sprintf(t_json, HX711_Weight_Report, Pcd_Massage_Flag.Pcd_Card_ID , HX711_Massage.Write_To_Card_Weight);
                if(MQTT_Publish(AT_MQTT_PUB_TIMING_WAKEUP,t_json)==1)
                {                          
                      //  BC260Y_Init();//对设备初始化	
                        connect_mqtt();
                }               
                Pcd_Massage_Flag.Pcd_Write_Flag=3;
                
            }
			else if(Send_timing_wakeup>=20)              //定时发送，维持模块唤醒
            {
                Send_timing_wakeup=0;  
                char res[]="OK"; 
                if(MQTT_Publish(AT_MQTT_PUB_TIMING_WAKEUP,res)==1)
                {   
                   // BC260Y_Init();//对设备初始化	
                    connect_mqtt();                                       
                }                    
            }
            else if(Pcd_Massage_Flag.Pcd_Legal_Flag==2 &&  BC260_Massage.send_error==1 && BC260_Massage.bindFlag==1)  //如果重量不合法，上报
            {
                memset(&t_json,0,sizeof(t_json));
                //char PUB_WEIGHT[]="{\"sn\":\"%s\"}";                                
                sprintf(t_json, HX711_Weight_Error_Report, BC260_Massage.BC260_SN,time_to_timestamp());//time_to_timestamp():获取秒时间戳；get_time_ms()：毫秒时间戳 
                if(MQTT_Publish(BC260_Massage.AT_MQTT_PUB_WEIGHT,t_json)==1)
                {   
                  //  BC260Y_Init();//对设备初始化	
                    connect_mqtt();                                       
                }
                 BC260_Massage.send_error=0;
            }
            if(Send_timing_wakeup>40)       //时间异常，重新连接
            {
                BC260_Massage.BC260Y_CONNECT_FLAG=0;
            }
        } 

//        xSemaphoreGive(MutexSemaphore);					//释放信号量 
//more  = uxTaskGetStackHighWaterMark(NULL);            //查看任务使用堆栈数    
        vTaskDelay(500);
    }
    

}



