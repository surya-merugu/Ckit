/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_thc_off_init.c
  * Origin Date           :   16/03/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Thermal Cutoff
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
#include "re_thc_off_init.h"

/**
 * @brief Thermal cutoff GPIO Initialization
 * This function configures the GPIO pins used by thermal cutoff
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_thc_off_GpioInit (void)
{
    GPIO_InitTypeDef Thc_Control, Thc_status;
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /*
    * PD7  : thermal cutoff status Pin
    * PD11 : thermal cutoff control Pin
    */

     Thc_Control.Pin               = GPIO_PIN_11;
     Thc_Control.Mode              = GPIO_MODE_OUTPUT_PP;
     Thc_Control.Speed             = GPIO_SPEED_FAST;
     Thc_Control.Pull              = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOD, &Thc_Control);


     Thc_status.Pin         = GPIO_PIN_7;
     Thc_status.Mode        = GPIO_MODE_INPUT;
     Thc_status.Speed       = GPIO_SPEED_FAST;
     Thc_status.Pull        = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOD, &Thc_status);

     return RE_OK;
}

