/*********************************************************************************************************
*
* File                : ws_W25Qx.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include "w25q64.h" 
#include "spi.h"
#include "usart.h"
#include "work.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "cJSON_user.h"
#include "hx711.h"
#include "BC260Y.h"
/**
  * @brief  Initializes the W25Q128FV interface.
  * @retval None
  */
uint8_t BSP_W25Qx_Init(void)
{ 
	/* Reset W25Qxxx */
	BSP_W25Qx_Reset();
	
	return BSP_W25Qx_GetStatus();
}

/**
  * @brief  This function reset the W25Qx.
  * @retval None
  */
static void	BSP_W25Qx_Reset(void)
{
	uint8_t cmd[2] = {RESET_ENABLE_CMD,RESET_MEMORY_CMD};
	
	W25Qx_Enable();
	/* Send the reset command */
	HAL_SPI_Transmit(&hspi1, cmd, 2, W25Qx_TIMEOUT_VALUE);	
	W25Qx_Disable();

}

/**
  * @brief  Reads current status of the W25Q128FV.
  * @retval W25Q128FV memory status
  */
static uint8_t BSP_W25Qx_GetStatus(void)
{
	 __align(4) uint8_t cmd[] = {READ_STATUS_REG1_CMD};
	uint8_t status;
	
	W25Qx_Enable();
	/* Send the read status command */
	HAL_SPI_Transmit(&hspi1, cmd, 1, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	HAL_SPI_Receive(&hspi1,&status, 1, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
	
	/* Check the value of the register */
	if((status & W25Q128FV_FSR_BUSY) != 0)
	{
		return W25Qx_BUSY;
	}
	return W25Qx_OK;	
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t BSP_W25Qx_WriteEnable(void)
{
	uint8_t cmd[] = {WRITE_ENABLE_CMD};
	uint32_t tickstart = HAL_GetTick();

	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 1, W25Qx_TIMEOUT_VALUE);	
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();
	
	/* Wait the end of Flash writing */
	while(BSP_W25Qx_GetStatus() == W25Qx_BUSY);
	{
		/* Check for the Timeout */
    if((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
    {        
			return W25Qx_TIMEOUT;
    }
	}
	
	return W25Qx_OK;
}

/**
  * @brief  Read Manufacture/Device ID.
	* @param  return value address
  * @retval None
  */
void BSP_W25Qx_Read_ID(uint8_t *ID)
{
	uint8_t cmd[4] = {READ_ID_CMD,0x00,0x00,0x00};
	
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 4, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	HAL_SPI_Receive(&hspi1,ID, 2, W25Qx_TIMEOUT_VALUE);
	W25Qx_Disable();
		
}

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read    
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
	uint8_t cmd[4];

	/* Configure the command */
	cmd[0] = READ_CMD;
	cmd[1] = (uint8_t)(ReadAddr >> 16);
	cmd[2] = (uint8_t)(ReadAddr >> 8);
	cmd[3] = (uint8_t)(ReadAddr);
	
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 4, W25Qx_TIMEOUT_VALUE);	
	/* Reception of the data */
	if (HAL_SPI_Receive(&hspi1, pData,Size,W25Qx_TIMEOUT_VALUE) != HAL_OK)
  {
    return W25Qx_ERROR;
  }
	W25Qx_Disable();
	return W25Qx_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write,No more than 256byte.    
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	uint8_t cmd[4];
	uint32_t end_addr, current_size, current_addr;
	uint32_t tickstart = HAL_GetTick();
	
	/* Calculation of the size between the write address and the end of the page */
  current_addr = 0;

  while (current_addr <= WriteAddr)
  {
    current_addr += W25Q128FV_PAGE_SIZE;
  }
  current_size = current_addr - WriteAddr;

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > Size)
  {
    current_size = Size;
  }

  /* Initialize the adress variables */
  current_addr = WriteAddr;
  end_addr = WriteAddr + Size;
	
  /* Perform the write page by page */
  do
  {
		/* Configure the command */
		cmd[0] = PAGE_PROG_CMD;
		cmd[1] = (uint8_t)(current_addr >> 16);
		cmd[2] = (uint8_t)(current_addr >> 8);
		cmd[3] = (uint8_t)(current_addr);

		/* Enable write operations */
		BSP_W25Qx_WriteEnable();
	
		W25Qx_Enable();
    /* Send the command */
    if (HAL_SPI_Transmit(&hspi1,cmd, 4, W25Qx_TIMEOUT_VALUE) != HAL_OK)
    {
      return W25Qx_ERROR;
    }
    
    /* Transmission of the data */
    if (HAL_SPI_Transmit(&hspi1, pData,current_size, W25Qx_TIMEOUT_VALUE) != HAL_OK)
    {
      return W25Qx_ERROR;
    }
			W25Qx_Disable();
    	/* Wait the end of Flash writing */
		while(BSP_W25Qx_GetStatus() == W25Qx_BUSY)
		{
			/* Check for the Timeout */
			if((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
			{        
				return W25Qx_TIMEOUT;
			}
		}
    
    /* Update the address and size variables for next page programming */
    current_addr += current_size;
    pData += current_size;
    current_size = ((current_addr + W25Q128FV_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q128FV_PAGE_SIZE;
  } while (current_addr < end_addr);

	
	return W25Qx_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory. 
  * @param  BlockAddress: Block address to erase  
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address)
{
	uint8_t cmd[4];
	Address*=4096;
	uint32_t tickstart = HAL_GetTick();
	cmd[0] = SECTOR_ERASE_CMD;
	cmd[1] = (uint8_t)(Address >> 16);
	cmd[2] = (uint8_t)(Address >> 8);
	cmd[3] = (uint8_t)(Address);
	
	/* Enable write operations */
	BSP_W25Qx_WriteEnable();
	
	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 4, W25Qx_TIMEOUT_VALUE);	
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();
	
	/* Wait the end of Flash writing */
	while(BSP_W25Qx_GetStatus() == W25Qx_BUSY);
	{
		/* Check for the Timeout */
    if((HAL_GetTick() - tickstart) > W25Q128FV_SECTOR_ERASE_MAX_TIME)
    {        
			return W25Qx_TIMEOUT;
    }
	}
	return W25Qx_OK;
}

/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Chip(void)
{
	 __align(4) uint8_t cmd[4];
	uint32_t tickstart = HAL_GetTick();
	cmd[0] = SECTOR_ERASE_CMD;
	
	/* Enable write operations */
	BSP_W25Qx_WriteEnable();
	
	/*Select the FLASH: Chip Select low */
	W25Qx_Enable();
	/* Send the read ID command */
	HAL_SPI_Transmit(&hspi1, cmd, 1, W25Qx_TIMEOUT_VALUE);	
	/*Deselect the FLASH: Chip Select high */
	W25Qx_Disable();
	
	/* Wait the end of Flash writing */
	while(BSP_W25Qx_GetStatus() != W25Qx_BUSY);
	{
		/* Check for the Timeout */
		if((HAL_GetTick() - tickstart) > W25Q128FV_BULK_ERASE_MAX_TIME)
		{        
				return W25Qx_TIMEOUT;
		}
	}
	return W25Qx_OK;
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		BSP_W25Qx_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			BSP_W25Qx_Erase_Block(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			BSP_W25Qx_Write(W25QXX_BUF,secpos*4096,4096);//写入整个扇区 
//			break;//写入结束了			
		}else
		BSP_W25Qx_Write(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}

//读取flash内数据进行初始化设置
void Flash_Data_Init(void)
{   
	//printf("size=%d\r\n",CJSON_DATA_SIZE);
	BSP_W25Qx_Read((u8*)&Cjson_Buf,CJSON_DATA_FLASH_BASE,CJSON_DATA_SIZE);
	 if(Cjson_Buf.size>CJSON_BUF_DATA_LEN)
	 {
		memset(&Cjson_Buf,0,sizeof(Cjson_Buf));
	 }	 
	  memset(&work_time,0,sizeof(work_time));//此处初始化结构体为必要操作，否则结构体无法正常初始化

	BSP_W25Qx_Read(HX711_Massage.Base_value_Buf,HX711_WEIGHT_DATA_FLASH_BASE,HX711_WEIGHT_DATA_SIZE);
	HX711_Massage.Base_Weight_Value=(u32)HX711_Massage.Base_value_Buf[0]<<24|
									(u32)HX711_Massage.Base_value_Buf[1]<<16|
									(u32)HX711_Massage.Base_value_Buf[2]<< 8|
									(u32)HX711_Massage.Base_value_Buf[3]<< 0;	
	if(HX711_Massage.Base_Weight_Value<=1000000 || HX711_Massage.Base_Weight_Value==0xffffffff)
	{
		HX711_Massage.Base_Weight_Value=0;
	}
	BSP_W25Qx_Read(&BC260_Massage.bindFlag,BindFlag_DATA_FLASH_BASE,BindFlag_DATA_SIZE);
	if(BC260_Massage.bindFlag>1)
	{
		BC260_Massage.bindFlag=0;
	}
 }

