/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "util.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef int (*cbFunc)(const char *__restrict, ...);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHOW_SYSTEM_INFO             (0U)
#define SOFTWARE_TIMER_PERIOD_MS     (5000U)
#define QUEUE_SEND_TIMEOUT_MS        (10U)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
#ifdef DEBUG
char pcWriteBuffer[512];
#endif

typedef struct {
    uint32_t counter;
    uint32_t queue_item;
    uint32_t last_tick_ts;
} data_t;

static data_t data;
/* USER CODE END Variables */
/* Definitions for Task01 */
osThreadId_t Task01Handle;
const osThreadAttr_t Task01_attributes = {
  .name = "Task01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task02 */
osThreadId_t Task02Handle;
const osThreadAttr_t Task02_attributes = {
  .name = "Task02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SystemMonitorTa */
#if (SHOW_SYSTEM_INFO)
osThreadId_t SystemMonitorTaHandle;
const osThreadAttr_t SystemMonitorTa_attributes = {
  .name = "SystemMonitorTa",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
#endif
/* Definitions for Queue01 */
osMessageQueueId_t Queue01Handle;
const osMessageQueueAttr_t Queue01_attributes = {
  .name = "Queue01"
};
/* Definitions for Timer01 */
osTimerId_t Timer01Handle;
const osTimerAttr_t Timer01_attributes = {
  .name = "Timer01"
};
/* Definitions for BinarySem01 */
osSemaphoreId_t BinarySem01Handle;
const osSemaphoreAttr_t BinarySem01_attributes = {
  .name = "BinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartTask01(void *argument);
void StartTask02(cbFunc cbLogs);
void StartSystemMonitorTask(void *argument);
void PeriodicTimerCb01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  cbFunc cbLogs = printf;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem01 */
  BinarySem01Handle = osSemaphoreNew(1, 1, &BinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of Timer01 */
  Timer01Handle = osTimerNew(PeriodicTimerCb01, osTimerPeriodic, NULL, &Timer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue01 */
  Queue01Handle = osMessageQueueNew (16, sizeof(uint32_t), &Queue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task01 */
  Task01Handle = osThreadNew(StartTask01, NULL, &Task01_attributes);

  /* creation of Task02 */
  Task02Handle = osThreadNew((osThreadFunc_t)StartTask02, cbLogs, &Task02_attributes);

  /* creation of SystemMonitorTa */
#if (SHOW_SYSTEM_INFO)
  SystemMonitorTaHandle = osThreadNew(StartSystemMonitorTask, NULL, &SystemMonitorTa_attributes);
#endif
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTask01 */
/**
  * @brief  Function implementing the Task01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask01 */
void StartTask01(void *argument)
{
  /* USER CODE BEGIN StartTask01 */
  osTimerStart(Timer01Handle, SOFTWARE_TIMER_PERIOD_MS);
  osSemaphoreAcquire(BinarySem01Handle, osWaitForever);
  data.last_tick_ts = uwTick;
  /* Infinite loop */
  for(;;)
  {
    osSemaphoreAcquire(BinarySem01Handle, osWaitForever);
    data.counter++;
    xQueueSendToBack(Queue01Handle, &data.counter, QUEUE_SEND_TIMEOUT_MS);
    osDelay(1);
  }
  /* USER CODE END StartTask01 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
  * @brief  Function implementing the Task01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask02 */
void StartTask02(cbFunc cbLogs)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    if (xQueueReceive(Queue01Handle, &data.queue_item, 0) == pdTRUE) {
        cbLogs("The Element in the queue is %d, time has been passed %d ms\n",
        (unsigned int)data.queue_item, (unsigned int)UW_TICK_DIFF(data.last_tick_ts, uwTick));

      data.last_tick_ts = uwTick;
    }
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartSystemMonitorTask */
/**
* @brief Function implementing the SystemMonitorTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSystemMonitorTask */
#if (SHOW_SYSTEM_INFO)
void StartSystemMonitorTask(void *argument)
{
  /* USER CODE BEGIN StartSystemMonitorTask */
  /* Infinite loop */
  for(;;)
  {
#ifdef DEBUG
    osDelay(2000);
    vTaskList(pcWriteBuffer);
    DBG("%s\n", pcWriteBuffer);
#endif
    osDelay(1);
  }
  /* USER CODE END StartSystemMonitorTask */
}
#endif

/* PeriodicTimerCb01 function */
void PeriodicTimerCb01(void *argument)
{
  /* USER CODE BEGIN PeriodicTimerCb01 */
  osSemaphoreRelease(BinarySem01Handle);
  /* USER CODE END PeriodicTimerCb01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
