/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "lcd.h"
#include "tim.h"
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
/*APP_LCD Task*/
osThreadId_t APP_LCDhandle;
const osThreadAttr_t APP_LCD_attributs = {
        .name = "APP_LCD",
        .stack_size = 128*4,
        .priority = (osPriority_t)osPriorityNormal
};


/*APP_SR04 Task*/
osThreadId_t APP_SR04Handle;
const osThreadAttr_t APP_SR04_attributs = {
        .name = "APP_HC04",
        .stack_size = 128*4,

};

uint16_t TIM_CounterNum = 0;
/* USER CODE END Variables */
/* Definitions for APP_Main */
osThreadId_t APP_MainHandle;
const osThreadAttr_t APP_Main_attributes = {
  .name = "APP_Main",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void APP_LCDTask(void * param);
void APP_SR04Task(void * param);
/* USER CODE END FunctionPrototypes */

void APPTask_Main(void *argument);

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
  /* creation of APP_Main */
  APP_MainHandle = osThreadNew(APPTask_Main, NULL, &APP_Main_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  APP_LCDhandle = osThreadNew(APP_LCDTask,NULL,&APP_LCD_attributs);
  APP_SR04Handle = osThreadNew(APP_SR04Task,NULL,&APP_SR04_attributs);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_APPTask_Main */
/**
  * @brief  Function implementing the APP_Main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_APPTask_Main */
void APPTask_Main(void *argument)
{
  /* USER CODE BEGIN APPTask_Main */
    //nothing to do and suspend task
    vTaskSuspend(NULL);
  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1000);
//  }
  /* USER CODE END APPTask_Main */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void APP_LCDTask(void * param) {
    /*LCD Init*/
    LCD_Init(WHITE);
    TickType_t Tick = pdMS_TO_TICKS(1000);

    /*HC-SR04 test*/
    LCD_ShowString(35,0,"HC-SR04 Test",BLACK,WHITE,24,1);

    /*TIM Counter test*/
    LCD_ShowString(35,24,"TIM6 Counter:",BLACK,WHITE,24,1);
    while (1) {
        LCD_ShowIntNum(35,24*2,TIM_CounterNum,5,BLACK,WHITE,24);
        vTaskDelay(Tick);
    }
}


void APP_SR04Task(void * param) {
    TickType_t tick = pdMS_TO_TICKS(61);

    HAL_TIM_Base_Start(&htim7);

    for(;;) {
        __HAL_TIM_SET_COUNTER(&htim7,0x0000);
        vTaskDelay(tick);

        taskENTER_CRITICAL();   //protect TIM_CounterNum
        TIM_CounterNum = __HAL_TIM_GET_COUNTER(&htim7);
        taskEXIT_CRITICAL();
    }
}


/* USER CODE END Application */

