#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
#include "usart.h"
#include "tim.h"	
#include "w25q64.h"
#include "cJSON.h"
#include "cJSON_user.h"
#include "BC260Y.h"
#include "RC522S.h"
#include "HX711.h"
#include "work.h"
#include "ui.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"

//互斥信号量句柄
extern SemaphoreHandle_t MutexSemaphore;	//互斥信号量

BC260_MASSAGE BC260_Massage; 

char t_json[70];
void MQTT_SendStr(char* pData)
{
	char *data=pData;
	u16 len;
	len=strlen((char* )pData);
		HAL_UART_Transmit(&huart1, (u8 *)data,len, 10000);    // 发送回去
		HAL_UART_Transmit(&huart1, (u8 *)"\r\n",strlen("\r\n"), 1000);    // 发送回去

}
/*
*判断是否已经发送成功并回传
*resp:需要判断的内容
*timeout：超时时间
*/
u8 Clear_Buffer(const char *resp,u8 timeout)     
{
    u8 i=0;
    MQTT_Receive_Data();
    while (strstr((const char*)BC260_Massage.BC260_UART_BUF,resp)==0)
    {
        
        if(strstr((const char*)BC260_Massage.BC260_UART_BUF,(const char*)AT_Order_ERROE)!=0)
        {
             BC260_Massage.BC260Y_CONNECT_FLAG=0;
            return 1;
        }
        else if(i>=timeout)
        {
            return 0;
        }
        else
        {
            i++;         
        }
		vTaskDelay(100);  
        MQTT_Receive_Data();
    }

     return 0;
}


u8  BC260Y_Init(void)//对设备初始化
{
    char temp[5];
    MQTT_SendStr(AT_Order_CPSMS); //退出休眠模式
    vTaskDelay(50);  
    if(Clear_Buffer(AT_Order_RESP,10))
    {
        return 1;
    }
    MQTT_SendStr(AT_Order_ECHOMODE); //设置不回显输入数据 
    vTaskDelay(50);  
    if(Clear_Buffer(AT_Order_RESP,10))
    {
        return 1;
    }
    MQTT_SendStr(AT_Order_CFG); //发送”session“设置，获取掉电期间服务器下发的消息
    vTaskDelay(50);  
    if(Clear_Buffer(AT_Order_RESP,10))
    {
        return 1;
    }
    MQTT_SendStr(AT_Order_QMTCFG); //设置客户端保活时间。范围0-3600 （单位：秒），0：不断开连接
    vTaskDelay(50);  
    if(Clear_Buffer(AT_Order_RESP,30))
    {
        return 1;
    } 

    MQTT_SendStr(AT_RESP_CSQ);  //查询网络信号
    vTaskDelay(50);  
    if(Clear_Buffer(AT_Order_RESP,30))
    {
        return 1;
    }
    sscanf(BC260_Massage.BC260_UART_BUF,"%*[^:]: %[^,]",temp);
    sscanf(temp,"%d",&BC260_Massage.CSG_val);
	MQTT_SendStr(AT_Order_CGIMEI);	//发送
    vTaskDelay(50);
    if(Clear_Buffer(AT_Order_RESP,30)==1)
    {
        return 1;
    }
    sscanf(BC260_Massage.BC260_UART_BUF, "%*[^: ]: %[^\r]",BC260_Massage.BC260_IMEI);
//	printf("imei=%s\r\n",BC260_Massage.BC260_IMEI);
    if(BC260_Massage.BC260_IMEI==0)
    {
        return 1;
    }
    vTaskDelay(50);
	if(strstr(BC260_Massage.BC260_SN,"MPN")==0)	
    {
        MQTT_SendStr(AT_Order_CGSN);	//发送
        vTaskDelay(50);
        if(Clear_Buffer(AT_Order_RESP,30)==1)
        {
            return 1;
        }
        sscanf(BC260_Massage.BC260_UART_BUF, "%*[^M]%[^\r]\r",BC260_Massage.BC260_SN);
//        printf("sn=%s\r\n",BC260_Massage.BC260_SN);
        if(strstr(BC260_Massage.BC260_SN,"MPN")==0)
        {
            return 1;
        }
        W25QXX_Write((u8*)&BC260_Massage.BC260_SN,MQTT_SN_DATA_FLASH_BASE,MQTT_SN_DATA_SIZE);    
        //vTaskDelay(300);
        
    }
    return 0;
}

/*
*关闭服务器
*/
u8 MQTT_QMTCLOSE(void)
{
	MQTT_SendStr( AT_MQTT_QMTCLOSE);	//发送
    vTaskDelay(100);
    if(Clear_Buffer(AT_MQTT_RESP_QMTCLOSE,90))
    {
        return 1;
    }
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;   
}

/*
*连接服务器
*/
u8 MQTT_QMTOPEN(void)
{
	MQTT_SendStr( AT_MQTT_QMTOPEN);	//发送
    vTaskDelay(100);
    if(Clear_Buffer(AT_MQTT_RESP_QMTOPEN,90))
    {
        return 1;
    }
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}
/*
*登录服务器
*/
u8 MQTT_QMTCONN(void)
{
    memset(&t_json,0,sizeof(t_json));
    //printf("sn=%s\r\n",BC260_Massage.BC260_SN);         //使用SN码作为设备名
    sprintf(t_json, AT_MQTT_QMTCONN,BC260_Massage.BC260_SN);
	MQTT_SendStr(t_json);	//发送
    vTaskDelay(100);
    if(Clear_Buffer(AT_MQTT_RESP_QMTCONN,90))
    {
        return 1;
    }
    vTaskDelay(1000); 
    MQTT_Receive_Data();        //此位置用于接收服务器存储的内容
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}
/*
* 订阅
*QMTSUB:订阅主题
*QOS:QOS等级
*/
u8 MQTT_Subscribe(char *QMTSUB)
{
    memset(&t_json,0,sizeof(t_json));	
    sprintf(t_json,QMTSUB,BC260_Massage.BC260_SN);
    //printf("sn=%s\r\n",BC260_Massage.BC260_SN);         //使用SN码作为设备名
	MQTT_SendStr(t_json);	//发送
    vTaskDelay(300);
    if(Clear_Buffer(AT_MQTT_RESP_MQTSUB,90))
    {
        return 1;
    }
	BC260_Massage.BC260Y_CONNECT_FLAG=1;
   return 0;
}

/*
* 广播
*str:广播主题
* sub:广播内容
*/
u8 MQTT_Publish(const char *str, char *sub)
{
    u8 t=26;
	u16 len;   
    memset(&t_json,0,sizeof(t_json));	
    sprintf(t_json,str,BC260_Massage.BC260_SN);
	MQTT_SendStr((char *)t_json);	//发送
    vTaskDelay(300);
    MQTT_Receive_Data();		
    len=strlen(sub);
    HAL_UART_Transmit(&huart1, (u8* )sub,len, 10000);    // 发送		
    HAL_UART_Transmit(&huart1, &t,1, 10000);    // 发送
    vTaskDelay(300);
    if(Clear_Buffer(AT_MQTT_RESP_MQTPUB,90))
    {
        return 1;
    }
    BC260_Massage.BC260Y_CONNECT_FLAG=1;
    return 0;
}

char JSON_MASSAGE[200]; //存放定时上报的数据
/*定时发送函数*/
void TimerSub(void)
{

    Send_Timeout=0;     //计数清零
    Send_timing_wakeup=0; 
    memset(&JSON_MASSAGE,0,sizeof(JSON_MASSAGE));   
    sprintf(JSON_MASSAGE,JSON_BC260Y_MASSAGE,BC260_Massage.BC260_SN,BC260_Massage.BC260_IMEI, BC260_Massage.bindFlag ,1, time_to_timestamp(),HX711_Massage.Write_To_Card_Weight);//time_to_timestamp():获取秒时间戳；get_time_ms()：毫秒时间戳             
    // printf("size=%d\r\n",strlen(JSON_MASSAGE));
    if(MQTT_Publish(AT_MQTT_PUB_TIMING_REPORT,JSON_MASSAGE)==1)
    {
        MQTT_QMTCONN(); 
        mqtt_Sub();
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
		if(strstr(BC260_Massage.BC260_UART_BUF,AT_MQTT_RESP_QMTSTAT)!=0  )   //看下返回状态
        {
                BC260_Massage.BC260Y_CONNECT_FLAG=0;
                MQTT_QMTCLOSE();
        }
        else if(strstr(BC260_Massage.BC260_UART_BUF,AT_RESP_INIT)!=0 )   //看下返回状态
        {
                BC260_Massage.BC260Y_OPEN_FLAG=1;                              
        }
        else if(strstr(BC260_Massage.BC260_UART_BUF,AT_MQTT_RESP_QMTRECV)!=0 )   //看下返回状态
        {
            char json_order_buf[220];
            char *Cjson_s="{%s}}"; 
            sscanf(BC260_Massage.BC260_UART_BUF, "%*[^{]{%[^}]",BC260_Massage.AT_ORDER_BUF);
            memset(&json_order_buf,0,sizeof(json_order_buf));	
            //json_order_buf=malloc(strlen(BC260_Massage.AT_ORDER_BUF));       //加回"{}"所占的内存
            sprintf(json_order_buf,Cjson_s,BC260_Massage.AT_ORDER_BUF);        //对数据加回“{}”，再进行数据处理
            
            if(strstr(BC260_Massage.BC260_UART_BUF,"task")!=0 ) //任务设置
            {           
                ParseStr(json_order_buf,createTimerTask);    //处理Cjson格式数据                           
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,"clearTask")!=0)       //清空任务
            {
                ParseStr(json_order_buf,createClearTask);     //处理Cjson格式数据
                TimerSub();    //立即发送状态
                ui_event_cb=bind;
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,"syncDate")!=0 )       //时间设置任务
            {
                ParseStr(json_order_buf,createRTCTask);    //处理Cjson格式数据
            }
            else if(strstr(BC260_Massage.BC260_UART_BUF,"bind")!=0)
            {
                    ParseStr(json_order_buf,createBindTask);    //处理Cjson格式数据
                    TimerSub();    //立即发送状态  
                    ui_event_cb=bind;         
            }
        }       
	}

}
/*
*订阅主题
*/
void mqtt_Sub(void)
{  
    if(MQTT_Subscribe(TimerTask)==0)
    {
        if(MQTT_Subscribe(TimerClearTask)==0)
        {
           if( MQTT_Subscribe(RtcTask)==0)
           {
                MQTT_Subscribe(BindTask);
           } 
        }
    }
}

void BC260Y_task(void const * argument )
{
	
    for( ; ; ) 
    {      
//       xSemaphoreTake(MutexSemaphore,portMAX_DELAY);	//获取互斥信号量
        MQTT_Receive_Data();
//			  xSemaphoreGive(MutexSemaphore);					//释放信号量 
        if((BC260_Massage.BC260Y_CONNECT_FLAG ==0 && Send_Timeout>=30) ||(BC260_Massage.BC260Y_CONNECT_FLAG ==0 &&BC260_Massage.BC260Y_OPEN_FLAG==1)) //初始化4G模块
        {
				BC260_Massage.BC260Y_CONNECT_FLAG=1; 
               if(BC260Y_Init()==1)  //对设备初始化 
                {
                    BC260_Massage.BC260Y_CONNECT_FLAG=0;
                }
                else
                {
                   
                    MQTT_QMTOPEN(); //d
                    MQTT_QMTCONN();                    
                    mqtt_Sub();
                    if(BC260_Massage.BC260Y_CONNECT_FLAG==1)
                    {
                        Send_Timeout=0;
                        Send_timing_wakeup=0;
                        BC260_Massage.BC260Y_CONNECT_FLAG=1;
                        ui_event_cb=signal;     //触发一次事件
//                            beep_on();
//                            vTaskDelay(500);	
//                            beep_off();
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
                char massage_pcd[35];
                Send_timing_wakeup=0;
                 memset(&massage_pcd,0,sizeof(massage_pcd));
                sprintf(massage_pcd, HX711_Weight_Report, Pcd_Massage_Flag.Pcd_Card_ID , HX711_Massage.Write_To_Card_Weight);
                //printf("size=%d,pcd=%s",strlen(massage_pcd),massage_pcd);
                if(MQTT_Publish(AT_MQTT_PUB_TIMING_WAKEUP,massage_pcd)==1)
                {                          
                       MQTT_QMTCONN();	
                        mqtt_Sub();
                }
                Pcd_Massage_Flag.Pcd_Write_Flag=3;
                
            }
			else if(Send_timing_wakeup>=20)              //定时发送，维持模块唤醒
            {
                Send_timing_wakeup=0;  
                char *res="OK"; 
                if(MQTT_Publish(AT_MQTT_PUB_TIMING_WAKEUP,res)==1)
                {   
                    MQTT_QMTCONN(); 
                    mqtt_Sub();                                       
                }                    
            }
            else if(Pcd_Massage_Flag.Pcd_Legal_Flag==2 &&  BC260_Massage.send_error==1 && BC260_Massage.bindFlag==1)  //如果重量不合法，上报
            {
                memset(&t_json,0,sizeof(t_json));
                sprintf(t_json, HX711_Weight_Error_Report, BC260_Massage.BC260_SN,time_to_timestamp());//time_to_timestamp():获取秒时间戳；get_time_ms()：毫秒时间戳 
                if(MQTT_Publish(AT_MQTT_PUB_WEIGHT_REPORT,t_json)==1)
                {   
                    MQTT_QMTCONN(); 
                    mqtt_Sub();                                        
                }
                 BC260_Massage.send_error=0;
            }
        } 

//        xSemaphoreGive(MutexSemaphore);					//释放信号量 
//more  = uxTaskGetStackHighWaterMark(NULL);            //查看任务使用堆栈数    
        vTaskDelay(500);
    }
    

}



