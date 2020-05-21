/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_rev_light_init.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Optocoupler Sensor
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
#include "re_rev_light_init.h"

/**
 * @brief Relay GPIO Initialization
 * This function configures the GPIO pins used by relay
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_RevLight_GpioInit (void)
{
    GPIO_InitTypeDef RevLight_pin;
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*
    * PA0 : Reverse Light Relay Pin
    */

     RevLight_pin.Pin            = GPIO_PIN_5;
     RevLight_pin.Mode           = GPIO_MODE_OUTPUT_PP;
     RevLight_pin.Speed          = GPIO_SPEED_FAST;
     RevLight_pin.Pull           = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOA, &RevLight_pin);

     return RE_OK;
}

