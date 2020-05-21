/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_dash_lcd.h
  * Origin Date           :   10/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :
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

/******** Define to prevent recursive inclusion*****************************/
#ifndef _RE_DRIVER_DASH_LCD_H
#define _RE_DRIVER_DASH_LCD_H

/* Includes */
#include "re_lcd_gpio_init.h"
#include "stdbool.h"

/* Exported API's */
RE_StatusTypeDef RE_lcd_reset(void);
RE_StatusTypeDef RE_lcd_write_data(uint8_t address, uint64_t data, uint16_t cs_pin);

#endif
/**************************** END OF FILE *************************************/
