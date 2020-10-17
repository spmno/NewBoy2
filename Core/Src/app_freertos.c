/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "user_task.h"
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "minmea.h"
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId_t gps_task_handle;
const osThreadAttr_t gps_task_attributes = {
  .name = "gps_task",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
void gps_task(void *argument);

osThreadId_t at_task_handle;
const osThreadAttr_t at_task_attributes = {
  .name = "at_task",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};
void at_task(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  gps_task_handle = osThreadNew(gps_task, NULL, &gps_task_attributes);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_Delay(1000);
	printf("hello, log\n");
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
char gps_buffer[GPS_BUFFER_SIZE];
char gprmc_buffer[100];
char gpgga_buffer[100];
void gps_task(void *argument)
{
	while(1) {
		osThreadFlagsWait(GPS_DATA_FLAG, osFlagsWaitAny, osWaitForever);
		strcpy(gps_buffer, usart1_buffer);
		printf("%s\n", gps_buffer);
		const char* gprmc_pointer = strstr(gps_buffer, "$GPRMC");
		if (gprmc_pointer != NULL) {
			const char* gprmc_end_pointer = strstr(gprmc_pointer, "\n");
			int rmc_length = gprmc_end_pointer-gprmc_pointer;
			printf("rmc length %d\n", rmc_length); 
			strncpy(gprmc_buffer, gprmc_pointer, rmc_length);
			struct minmea_sentence_rmc frame;
			if (minmea_parse_rmc(&frame, gprmc_buffer)) {
                    printf("$GPRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                            frame.latitude.value, frame.latitude.scale,
                            frame.longitude.value, frame.longitude.scale,
                            frame.speed.value, frame.speed.scale);
                    printf("$GPRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                            minmea_rescale(&frame.latitude, 1000),
                            minmea_rescale(&frame.longitude, 1000),
                            minmea_rescale(&frame.speed, 1000));
				printf("$GPRMC time: date:%d-%d-%d, time: %d-%d-%d\n",
							frame.date.year, frame.date.month, frame.date.day,
                            frame.time.hours, frame.time.minutes, frame.time.seconds
							);

                }
                else {
                    printf("$xxRMC sentence is not parsed\n");
                }
		}
		const char* gpgga_pointer = strstr(gps_buffer, "$GPGGA");
		if (gpgga_pointer != NULL) {
			const char* gpgga_end_pointer = strstr(gpgga_pointer, "\n");
			int gga_length = gpgga_end_pointer-gpgga_pointer;
			printf("gga length %d\n", gga_length); 
			strncpy(gpgga_buffer, gpgga_pointer, gga_length);
			struct minmea_sentence_gga frame; 	
			if (minmea_parse_gga(&frame, gpgga_buffer)) {
               printf("$xxGGA: fix quality: %d\n", frame.fix_quality);
            }
            else {
               printf("$xxGGA sentence is not parsed\n");
            }
		}
	}

}

void at_task(void *argument) 
{
	while(1) {
		osThreadFlagsWait(GPS_DATA_FLAG, osFlagsWaitAny, osWaitForever);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/