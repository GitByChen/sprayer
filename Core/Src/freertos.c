/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "tim.h"
#include "usart.h"
#include "stdio.h"
#include "rtc.h"
#include "iwdg.h"
#include "rc522s.h"
#include "w25q64.h"
#include "hx711.h"
#include "cJSON_user.h"
#include "lcd.h"
#include "lcd_init.h"
#include "ui.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "BC260Y.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId startTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
//?????????????
SemaphoreHandle_t MutexSemaphore;	//?????????

osThreadId BC260YTaskHandle;
osThreadId WorkTaskHandle;
osThreadId RC522STaskHandle;
osThreadId GUITaskHandle;

void BC260Y_task(void const * argument );
void work_task(void const * argument );
void RC522Stask(void const * argument);
void GUI_task(void const * argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
		//?????????????
	MutexSemaphore=xSemaphoreCreateMutex();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of startTask */
  osThreadDef(startTask, StartDefaultTask, osPriorityLow, 0, 256);
  startTaskHandle = osThreadCreate(osThread(startTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    osThreadDef( WorkTask, work_task, osPriorityBelowNormal, 0, 256);
   WorkTaskHandle = osThreadCreate(osThread( WorkTask), NULL);

	  osThreadDef(RC522STask, RC522Stask, osPriorityNormal, 0, 256);
  RC522STaskHandle = osThreadCreate(osThread(RC522STask), NULL);



	  osThreadDef( BC260YTask, BC260Y_task, osPriorityHigh, 0, 1024);
   BC260YTaskHandle = osThreadCreate(osThread( BC260YTask), NULL);
	 
	 osThreadDef( GUITask, GUI_task, osPriorityAboveNormal, 0, 4*1024);
   GUITaskHandle = osThreadCreate(osThread( GUITask), NULL);

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the startTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		Uart_task();
//    MQTT_Receive_Data();
//		HAL_IWDG_Refresh(&hiwdg);
		osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
