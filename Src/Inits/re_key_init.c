/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_key_init.c
  * Origin Date           :   16/03/2020
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
#include "re_key_init.h"

/**
 * @brief SPEED GPIO Initialization
 * This function configures the GPIO pins used by Speed
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_key_GpioInit(void)
{
    GPIO_InitTypeDef key_pin ;
    __GPIOE_CLK_ENABLE(); /* GPIO Port C Clock Enable */

    key_pin.Pin      = GPIO_PIN_4;
    key_pin.Mode     = GPIO_MODE_IT_RISING_FALLING;
    key_pin.Speed    = GPIO_SPEED_FAST;
    key_pin.Pull     = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE,&key_pin);
//
     HAL_NVIC_SetPriority(EXTI4_IRQn,0,0);
     HAL_NVIC_EnableIRQ(EXTI4_IRQn);


    return RE_OK;
}

