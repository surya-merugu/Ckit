/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_dock_latch_init.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Optocoupler Relay
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
#include "re_dock_latch_init.h"

/**
 * @brief Dock Latch GPIO Initialization
 * This function configures the GPIO pins used by Dock Latch
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_DockLatch_GpioInit (void)
{
    GPIO_InitTypeDef LatchControl, Latchstatus_pin, LED_pin;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /*
    * PA1 : Latch Relay Pin
    */

     LatchControl.Pin               = GPIO_PIN_10;
     LatchControl.Mode              = GPIO_MODE_OUTPUT_PP;
     LatchControl.Speed             = GPIO_SPEED_FAST;
     LatchControl.Pull              = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOD, &LatchControl);


     Latchstatus_pin.Pin         = GPIO_PIN_8 ;
     Latchstatus_pin.Mode        = GPIO_MODE_INPUT;
     Latchstatus_pin.Speed       = GPIO_SPEED_FAST;
     Latchstatus_pin.Pull        = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOA, &Latchstatus_pin);


     LED_pin.Pin               = GPIO_PIN_12;
     LED_pin.Mode              = GPIO_MODE_OUTPUT_PP;
     LED_pin.Speed             = GPIO_SPEED_FAST;
     LED_pin.Pull              = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOD, &LED_pin);
     return RE_OK;
}

