/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "string.h"
uint8_t UART1_RX_Buffer[UART1_REC_LEN]={0};
//uint8_t UART2_RX_Buffer[UART2_REC_LEN]={0};
uint8_t UART3_RX_Buffer[UART3_REC_LEN]={0};

uint16_t UART1_RX_STA=0;    // 第15bit表示一帧数据接收完成，第14~0位表示接收到的数据量
//uint16_t UART2_RX_STA=0;    // 第15bit表示一帧数据接收完成，第14~0位表示接收到的数据量
uint16_t UART3_RX_STA=0;    // 第15bit表示一帧数据接收完成，第14~0位表示接收到的数据量


uint8_t Res=0;
uint8_t uart2_rxbuf=0;


#pragma import(__use_no_semihosting_swi)
#pragma import(__use_no_semihosting) 

void  _sys_exit(int x) { 
    x = x; 
} 

struct __FILE  { 
    int handle; 
    /* Whatever you require here. If the only file you are using is */ 
    /* standard output using printf() for debugging, no file handling */ 
    /* is required. */ 
}; 
/* FILE is typedef?? d in stdio.h. */ 
FILE __stdout;
/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
 
/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart3, &ch, 1, 0xffff);
  return ch;
}

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	
	//HAL_UART_Receive_IT(&huart1,UART1_RX_Buffer, 1);
  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
	
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);              // 使能接收中断
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);              // 使能空闲中断
//	__HAL_UART_ENABLE_IT(&huart1,UART_IT_ERR);
//		HAL_UART_Receive_IT(&huart1,UART1_RX_Buffer, 1);


  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */
//		__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);              // 使能接收中断
//	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);              // 使能空闲中断
//		__HAL_UART_ENABLE_IT(&huart2,UART_IT_ERR);

//		HAL_UART_Receive_IT(&huart2,UART2_RX_Buffer, 1);

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);              // 使能接收中断
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);              // 使能空闲中断
//		__HAL_UART_ENABLE_IT(&huart3,UART_IT_ORE);
//		HAL_UART_Receive_IT(&huart3,UART3_RX_Buffer, 1);

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void USART1_IRQHandler(void)
{
	u8 Res;
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)  // 空闲中断标记被置位
	{	
   // __HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
	//	if(!(	UART1_RX_STA & 0x8000))
		{
      
			Res=huart1.Instance->RDR;
      if(Res=='+')
      {
        memset(&UART1_RX_Buffer,0,sizeof(UART1_RX_Buffer));				//清除接收
        UART1_RX_STA=0;
      }
			UART1_RX_Buffer[UART1_RX_STA++]=Res;
			__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);              // 使能接收中断
			__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);              // 使能空闲中断
		}
	}
		if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
	{
		
			UART1_RX_STA|=0x8000;
			__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_IDLEF);
			__HAL_UART_DISABLE_IT(&huart1,UART_IT_IDLE);
			
	}
//			if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) != RESET)  // 空闲中断标记被置位
//			{
//				__HAL_UART_CLEAR_OREFLAG(&huart1);
//				__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);              // 使能接收中断

//			}
}

//void USART2_IRQHandler(void)
//{
//	u8 Res;
//	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)  // 空闲中断标记被置位
//	{	
//		if(!(	UART2_RX_STA & 0x8000))
//		{
//			Res=huart2.Instance->RDR;
//			UART2_RX_Buffer[UART2_RX_STA++]=Res;
//			__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);              // 使能接收中断
//			__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);              // 使能空闲中断
//		}
//	}
//		if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
//	{
//			UART2_RX_STA|=0x8000;
//			__HAL_UART_CLEAR_IT(&huart2,UART_CLEAR_IDLEF);
//			__HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);

//	}
//			if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE) != RESET)  // 空闲中断标记被置位
//			{
//				__HAL_UART_CLEAR_OREFLAG(&huart2);
//				__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);              // 使能接收中断

//			}
//}

void USART3_IRQHandler(void)
{
	u8 Res;

	if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET)  // 空闲中断标记被置位
	{	
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);
		if(!(	UART3_RX_STA & 0x8000))
		{
			Res=huart3.Instance->RDR;
			UART3_RX_Buffer[UART3_RX_STA++]=Res;
			__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);              // 使能接收中断
			__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);              // 使能空闲中断
			//HAL_UART_Receive_IT(&huart3,UART3_RX_Buffer, 1);
		}
	}
		if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET)  // 空闲中断标记被置位
	{
			UART3_RX_STA|=0x8000;
			__HAL_UART_CLEAR_IT(&huart3,UART_CLEAR_IDLEF);
			__HAL_UART_DISABLE_IT(&huart3,UART_IT_IDLE);
			

	} 

}
/* USER CODE END 1 */
