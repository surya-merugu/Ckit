/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_timer_init_callback.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   None
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

/* Includes */
#include "re_timer_int_callback.h"

extern TIM_HandleTypeDef htim3_t;

extern RE_StatusTypeDef RE_Timer_ISR(void);


/**
 * @brief Timer period elapsed callback
 * this function handles the interupt callbacks of timer3
 * @param htim3_t Timer handle TypeDef
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM3)
    {
        RE_Timer_ISR();
    }
}





/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/