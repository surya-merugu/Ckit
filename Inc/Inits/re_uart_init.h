/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_uart_init.h
  * Origin Date           :   08/02/2020
  * Compiler              :   iccarm
  * Hardware              :   mc60
  * Target                :   STM32F407-DISCOVERY
  * Notes                 :   None
  *****************************************************************************
  * @attention
  *
  * <h2><center>&copy;Copyright (C) RACEnergy, Inc - All Rights Reserved.
  * </center></h2>
  *
  * Unauthorized copying of this file, via any medium is strictly prohibited
  * Proprietary and confidential
  *
  * Written by Team RACEnergy, FEB 2020
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_UART_INIT_H
#define _RE_UART_INIT_H

/* Includes */
#include "re_std_def.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

extern UART_HandleTypeDef huart2_t;
extern UART_HandleTypeDef huart3_t;

/* Exported API */
RE_StatusTypeDef RE_USART2_UART_Init(void);
RE_StatusTypeDef RE_USART3_UART_Init(void);

#endif
/**************************** END OF FILE *************************************/
