/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_speed_init.c
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
#include "re_speed_init.h"

/**
 * @brief SPEED GPIO Initialization
 * This function configures the GPIO pins used by Speed
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_Speed_GpioInit(void)
{
    GPIO_InitTypeDef speed_pin ;
    __GPIOC_CLK_ENABLE(); /* GPIO Port C Clock Enable */
#if 0
    speed_pin.Pin      = GPIO_PIN_2;
    speed_pin.Mode     = GPIO_MODE_IT_FALLING;
    speed_pin.Speed    = GPIO_SPEED_FAST;
    speed_pin.Pull     = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC,&speed_pin);

     HAL_NVIC_SetPriority(EXTI2_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI2_IRQn);
#endif
    speed_pin.Pin      = GPIO_PIN_6;
    speed_pin.Mode     = GPIO_MODE_IT_FALLING;
    speed_pin.Speed    = GPIO_SPEED_FAST;
    speed_pin.Pull     = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC,&speed_pin);

     HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);


    return RE_OK;
}

