#include "HX711.h"
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "tim.h"
//#include "key.h"
#include "RC522S.h"
#include "rtc.h"
#include "BC260Y.h"
#include "w25q64.h"
#include "stdio.h"
TaskHandle_t HX711_Handler;		//任务句柄

HX711 HX711_Massage;

#define GapValue 1685.0
#define CPU_FREQUENCY_MHZ    80		// STM32晶振频率

void HX711_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
 //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//

	GPIO_InitStructure.Pin = HX711_SCK; //PE6
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;	//复用推挽输出
	HAL_GPIO_Init(HX711_PIN, &GPIO_InitStructure);//初始化GPIOA5

	GPIO_InitStructure.Pin  = HX711_DT;//PE5
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //下拉输入
 	HAL_GPIO_Init(HX711_PIN, &GPIO_InitStructure);//初始化GPIOA5


}
//设置SCK电平
//PD: 0/1
void Sck_Set(u8 PD)
{
    if(PD)
    {
        HAL_GPIO_WritePin(HX711_PIN,HX711_SCK,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(HX711_PIN,HX711_SCK,GPIO_PIN_RESET);
    }
}
void HX711_Delay_us(__IO uint32_t delay)
{
	int last, curr, val;
	int temp;

	while (delay != 0)
	{
		temp = delay > 900 ? 900 : delay;
		last = SysTick->VAL;
		curr = last - CPU_FREQUENCY_MHZ * temp;
		if (curr >= 0)
		{
			do
			{
				val = SysTick->VAL;
			}
			while ((val < last) && (val >= curr));
		}
		else
		{
			curr += CPU_FREQUENCY_MHZ * 1000;
			do
			{
				val = SysTick->VAL;
			}
			while ((val <= last) || (val > curr));
		}
		delay -= temp;
	}
}

uint32_t HX711_Read(void)
{
	unsigned long value; 
	unsigned char i; 
	
    HX711_DAT_H;
	vTaskDelay(1);
	
	HX711_SCK_L;
	value = 0; 
	while(HX711_DAT_READ); 
	
	for(i=0; i<24; i++)
	{ 
		HX711_SCK_H; 
		value = value<<1;
		HX711_Delay_us(1);
		HX711_SCK_L; 
		if(HX711_DAT_READ)
			value++;
		HX711_Delay_us(1);
	} 
 	HX711_SCK_H; 
	value = value^0x800000;
	HX711_Delay_us(1);
	HX711_SCK_L;
  
	return value;	
}

//数据处理 去极值平均/中值
u32  HX711_Value_Dispose(void)
{
    //u8 i=0;
    //u8 count,j,n=11;
    //u32 temp,value_buf[11];
	u32 sum=0;
	uint32_t   offset;
  	for (int i = 0; i < 10; i++)
    sum += HX711_Read();
	
	offset = (uint32_t)(sum/10);
	return offset;
/*    for(count = 0;count < n;count++)                                //获取采样值
	{
		value_buf[count] = HX711_Read();
	}
    for(j = 0;j<(n-1);j++)
    {
		for(i = 0;i<(n-j);i++)
        {
		    if(value_buf[i]>value_buf[i+1])
		    {
                temp = value_buf[i];
                value_buf[i] = value_buf[i+1];
                value_buf[i+1] = temp;
		    }
        }
    }

    for(count =1;count<n-1;count++)
    {
        sum += value_buf[count];
    }				
   // printf("value_buf=%d \r\n",value_buf[(n-1)/2]);
	//return value_buf[(n-1)/2];        //中值
    return sum/(n-2);           //去极值平均
	*/
}
 
void Get_WeightBase(void)
{
	HX711_Massage.Base_Weight_Value = HX711_Value_Dispose();
	HX711_Massage.Base_value_Buf[0]=(u8)(HX711_Massage.Base_Weight_Value>>24);
	HX711_Massage.Base_value_Buf[1]=(u8)(HX711_Massage.Base_Weight_Value>>16);
	HX711_Massage.Base_value_Buf[2]=(u8)(HX711_Massage.Base_Weight_Value>>8);
	HX711_Massage.Base_value_Buf[3]=(u8)(HX711_Massage.Base_Weight_Value>>0);
	W25QXX_Write(HX711_Massage.Base_value_Buf,HX711_WEIGHT_DATA_FLASH_BASE,HX711_WEIGHT_DATA_SIZE);
	printf("get ok\r\n");
} 
/*
*	称重函数
*/
float Get_Weight(void)
{
	HX711_Massage.Weight=0;
	HX711_Massage.HX711_Weight_Value = HX711_Value_Dispose();
	if(HX711_Massage.HX711_Weight_Value > HX711_Massage.Base_Weight_Value)			
	{
		HX711_Massage.Weight = (HX711_Massage.HX711_Weight_Value-HX711_Massage.Base_Weight_Value)/HX711_Massage.K;
		HX711_Massage.Write_To_Card_Weight=(u16)((HX711_Massage.Weight)+0.5f);		//+0.5 进行四舍五入 
		printf("重量值是：%d\r\n",HX711_Massage.Write_To_Card_Weight);
		 Pcd_Massage_Flag.Pcd_Write_Flag=1;//写标志位置1，将最新的重量写进卡里
	}
	
	return HX711_Massage.Weight;
}
/*
*将重量值写进卡内
*/
void Write_Weight_To_Card(void)
{
		u8  sendbuf[2];		//将重量分成两个字节写进卡内
	unsigned char snr=1;
	sendbuf[0]= HX711_Massage.Write_To_Card_Weight >>8;
	sendbuf[1]=(u8)HX711_Massage.Write_To_Card_Weight;
	printf("重量值是：%d\r\n",HX711_Massage.Write_To_Card_Weight);
	PCD_WriteBlock((snr*4+0), (u8 *)sendbuf);					//写卡操作
	Pcd_Massage_Flag.Pcd_Write_Flag=2;					//写卡标志位置1，在工作时清零

/*	u8  *sendbuf;		//将重量分成两个字节写进卡内
	unsigned char snr=1;
	sendbuf=Weight_encryption(HX711_Massage.Write_To_Card_Weight);
	printf("重量值是：%d\r\n",HX711_Massage.Write_To_Card_Weight);
	PCD_WriteBlock((snr*4+0), sendbuf);					//写卡操作
	Pcd_Massage_Flag.Pcd_Write_Flag=2;					//写卡标志位置1，在工作时清零
	*/
	PCD_Halt();	
}
/*
*重量加密
*/
u8* Weight_encryption(u16 weight)
{
	static u8 verify[4];
	u16 verify_val;
	verify_val=~weight;
	verify[0]=(u8)HX711_Massage.Write_To_Card_Weight;
	verify[1]=(u8)(HX711_Massage.Write_To_Card_Weight>>8);
	verify[2]=(u8)verify_val;
	verify[3]=(u8)(verify_val>>8);
	return verify;
}

/*
*重量解密
*/
u16 Weight_Decode(u8* weight)
{
	static u16 read_weight,read_verify_val;
	read_weight=(u16)weight[0] | (u16)weight[1]<<8;
	read_verify_val=(u16)weight[2] | (u16)weight[3]<<8;
	read_verify_val=~read_verify_val;
	if(read_weight==read_verify_val)
	{
		return read_weight;
	}
	else
	{
		return 1;
	}
}
/*void HX711_task(void* pvParameters )
{
	HX711_Massage.K=((((HX711_VAVDD)/HX711_BEARING)*HX711_GAIN)*HX711_24BIT)/HX711_BASE_VDD;      //计算转换的K值
    HX711_Massage.K/=HX711_K_VALUE;
	if(HX711_Massage.Base_Weight_Value==0)
	{
		Get_WeightBase();
	}
	for( ; ; ) 	
	{
		if(exti_key==1)		//按键清零、去皮
		{
			exti_key=0;
			Get_WeightBase();
			printf("去皮\r\n");
		}
		vTaskDelay(200);
	}
    
}

void HX711_Task_Init(void)
{
	xTaskCreate((	TaskFunction_t) HX711_task,					//任务2函数
						(const char * 	)"HX711_task",			//任务2函数名
						(uint16_t 			)HX711_STK_SIZE,		//任务2堆栈数
						(void * 				)NULL ,			//传入函数参数
						(UBaseType_t 		)HX711_TASK_PRIO,	//任务2优先级
						(TaskHandle_t * )&HX711_Handler );	//取址

}*/
