#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 2 //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#define LCD_W 320
#define LCD_H 240

// SW_SPI or HW_SPI or HW_SPI_DMA  （HW_SPI_DMA暂时还用不了，相关代码还没写）
#define HW_SPI_DMA 1

//==========================================
//定义spi 
//==========================================
#if (HW_SPI_DMA)
#define ST7789_LCD_SPI_DMA hspi3
extern SPI_HandleTypeDef hspi3;
#endif // HW_SP

//-----------------LCD IO口操作---------------- 
#define LCD_CS_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15, GPIO_PIN_SET) //CS		SPI_NSS
#define LCD_CS_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15, GPIO_PIN_RESET)

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4, GPIO_PIN_RESET)//DC SPI_MISO
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4, GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, GPIO_PIN_RESET)//RES LCD_RST
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, GPIO_PIN_RESET)//BLK LCD_BL
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10, GPIO_PIN_SET)

void LCD_WR_DATA_DMA(uint8_t *buf,uint32_t buf_size);//写入一个数组
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




