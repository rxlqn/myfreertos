/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
osThreadId PeriodicHandle;
osThreadId HandleHandle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void PeriodicTask(void const * argument);
void HandleTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

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
  /* definition and creation of Periodic */
  osThreadDef(Periodic, PeriodicTask, osPriorityNormal, 0, 128);
  PeriodicHandle = osThreadCreate(osThread(Periodic), NULL);

  /* definition and creation of Handle */
  osThreadDef(Handle, HandleTask, osPriorityAboveNormal, 0, 128);
  HandleHandle = osThreadCreate(osThread(Handle), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_PeriodicTask */
/**
  * @brief  Function implementing the Periodic thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_PeriodicTask */
void PeriodicTask(void const * argument)
{

  /* USER CODE BEGIN PeriodicTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
	printf("Periodic task - About to generate an interrupt.\r\n");
	osSemaphoreRelease(myBinarySem01Handle);
	printf("Periodic task - Interrupt generated.\r\n\r\n\r\n");
  }
  /* USER CODE END PeriodicTask */
}

/* USER CODE BEGIN Header_HandleTask */
/**
* @brief Function implementing the Handle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HandleTask */
void HandleTask(void const * argument)
{
  /* USER CODE BEGIN HandleTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
	osSemaphoreWait(myBinarySem01Handle,osWaitForever);
	printf("Processing event\r\n");
  }
  /* USER CODE END HandleTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

// idle hook must be enable in cube
/* Declare a variable that will be incremented by the hook function. */
u8 ulIdleCycleCount = 0UL;
/* 空闲钩子函数必须命名为vApplicationIdleHook(),无参数也无返回值。 */
void vApplicationIdleHook( void )
{
/* This hook function does nothing but increment a counter. */
	ulIdleCycleCount++;
	printf("ulIdleCycleCount = %d\r\n",ulIdleCycleCount);
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
