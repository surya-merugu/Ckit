/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_lcd_gpio_init.h
  * Origin Date           :   10/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   FSTN LCD
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
#ifndef _RE_LCD_GPIO_INIT_H
#define _RE_LCD_GPIO_INIT_H

/* Includes */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "re_std_def.h"

/* Exported API's */
RE_StatusTypeDef RE_LCD_GpioInit(void);

#endif
/**************************** END OF FILE *************************************/
