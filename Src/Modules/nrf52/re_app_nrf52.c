/**
  *****************************************************************************
  * Title                 :   Project Name
  * Filename              :   file_name.c
  * Origin Date           :   dd/mm/yyy
  * Compiler              :   Specify compiler used
  * Hardware              :   Sensors, other hardware used
  * Target                :   Board name
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
  * Written by Team RACEnergy, Month Year
  *****************************************************************************
  */

/* Includes */
#include "nrf52/re_app_nrf52.h"

extern UART_HandleTypeDef huart3_t;
extern uint8_t uart_rcvd_byte;
//extern uint8_t rcvd_data_buffer;
extern uint8_t count;

extern RE_StatusTypeDef RE_OpenDock_Latch(void);

void HAL_UART_RxCpltCallback (UART_HandleTypeDef *huart)
{
//    rcvd_data_buffer = uart_rcvd_byte;
    if (uart_rcvd_byte == 'a')
    {
       RE_OpenDock_Latch();
    }
}


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/