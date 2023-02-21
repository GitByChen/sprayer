#ifndef __HX711_H
#define __HX711_H
#include "main.h"

#define  HX711_SCK_L    HAL_GPIO_WritePin(HX711_PIN,HX711_SCK,GPIO_PIN_RESET)
#define  HX711_SCK_H    HAL_GPIO_WritePin(HX711_PIN,HX711_SCK,GPIO_PIN_SET)

#define  HX711_DAT_H    HAL_GPIO_WritePin(HX711_PIN,HX711_DT,GPIO_PIN_SET)
#define  HX711_DAT_READ HAL_GPIO_ReadPin(HX711_PIN,HX711_DT)


typedef struct 
{
    u8 Base_value_Buf[4];
    u16 Write_To_Card_Weight;       //写进卡内的值  将重量*10 写进卡内
    float Weight;                   //转换出来的重量值
    float K;                        //计算出来的线性值
    
    u32 HX711_Weight_Value;               //称重值
    u32 Base_Weight_Value;                //去皮值
}HX711;
extern HX711 HX711_Massage;         
#define HX711_GAIN      128         //增益值
#define HX711_VAVDD     4.3         //满量程输出电压 mV
#define HX711_24BIT     16777216    //2^24
#define HX711_BASE_VDD  4300        //基准电压 mV
#define HX711_BEARING   1           //最大承重 KG
//#define HX711_K_VALUE    1000*1.274 //转换系数 值大了降低系数 反之增加  //5KG的系数为1000*1.000 //1KG 系数1000*1.274
#define HX711_DT_READ GPIO_ReadInputDataBit(HX711_PIN,HX711_DT)
#if(HX711_BEARING==1)
    #define HX711_K_VALUE    1000*1.274 //转换系数 值大了降低系数 反之增加  //5KG的系数为1000*1.000 //1KG 系数1000*1.274
#elif(HX711_BEARING==5)
    #define HX711_K_VALUE    1000*1.000 //转换系数 值大了降低系数 反之增加  //5KG的系数为1000*1.000 //1KG 系数1000*1.274
#endif
void Get_WeightBase(void);
float Get_Weight(void);
void Write_Weight_To_Card(void);

extern uint32_t  HX711_Value;
extern uint32_t  Weight_Offset;
extern float   Weight;
extern u16 new_Weight;	//将重量*10 暂时存放，写进卡内
extern u8 Weight_ValueInit;     //是否初始化完成
void HX711_Task_Init(void);
void HX711_Init(void);
void Sck_Set(u8 PD);
u8* Weight_encryption(u16 weight); //加密
u16 Weight_Decode(u8* weight);      //解密

#endif


