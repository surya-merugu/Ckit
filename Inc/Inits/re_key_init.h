/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_key_init.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_KEY_INIT_H
#define _RE_KEY_INIT_H

/* Includes */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_it.h"
#include "re_std_def.h"

/* Exported API's */
RE_StatusTypeDef RE_key_GpioInit(void);

#endif
/**************************** END OF FILE *************************************/