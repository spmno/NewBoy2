/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define BUFFER_SIZE 256
#define GPS_BUFFER_SIZE	768
#define AT_BUFFER_SIZE	512
#define MINIMUM_LENGTH	4
//extern uint8_t lpuart1_buffer[BUFFER_SIZE];
extern uint8_t usart1_buffer[GPS_BUFFER_SIZE];
extern uint8_t usart3_buffer[AT_BUFFER_SIZE];
extern uint8_t at_buffer[AT_BUFFER_SIZE];
extern uint8_t uart4_buffer[BUFFER_SIZE];

void gpsinfo_callback(void);
void atinfo_callback(void);

/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
