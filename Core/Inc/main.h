/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//FLASH pin
#define FLASH_SPI_CS_PIN        GPIO_PIN_4      //CS
#define FLASH_SPI_SCK_PIN				GPIO_PIN_5      //SCK
#define FLASH_SPI_MISO_PIN	    GPIO_PIN_6      //MISO
#define FLASH_SPI_MOSI_PIN	    GPIO_PIN_7      //MOSI
#define	FLASH_SPI_GPIO					GPIOA

//RC552S PIN
#define RC522S_SPI_RST_PIN      GPIO_PIN_11
#define RC552S_SPI_CS_PIN       GPIO_PIN_12
#define RC552S_SPI_SCK_PIN      GPIO_PIN_13
#define RC552S_SPI_MISO_PIN     GPIO_PIN_14
#define RC552S_SPI_MOSI_PIN     GPIO_PIN_15
#define	RC552S_SPI_GPIO					GPIOB

//uart pin
#define USART1_TX               GPIO_PIN_6
#define USART1_RX               GPIO_PIN_7
#define USART1_GPIO							GPIOB

#define USART2_TX               GPIO_PIN_2
#define USART2_RX               GPIO_PIN_3
#define USART2_GPIO							GPIOA

#define USART3_TX								GPIO_PIN_10
#define USART3_RX								GPIO_PIN_11
#define USART3_GPIO							GPIOB

#define RC522_RST_Pin GPIO_PIN_11
#define RC522_RST_GPIO_Port GPIOB
#define RC522_CS_Pin GPIO_PIN_12
#define RC522_CS_GPIO_Port GPIOB
//PWN PIN
#define	PWM_PIN					GPIO_PIN_1					
#define	PWN_GPIO				GPIOA
//HX711
#define HX711_DT                GPIO_PIN_6
#define HX711_SCK               GPIO_PIN_7
#define HX711_PIN               GPIOC

#define BEEP_PIN					GPIO_PIN_8
#define BEEP_GPIO					GPIOA
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
void beep_on(void);
void beep_off(void);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RST_Pin GPIO_PIN_2
#define LCD_RST_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOB
#define SPI_NSS_Pin GPIO_PIN_15
#define SPI_NSS_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_12
#define SPI_MOSI_GPIO_Port GPIOC
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_MISO_Pin GPIO_PIN_4
#define SPI_MISO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
