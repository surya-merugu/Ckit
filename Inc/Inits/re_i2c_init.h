/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_i2c_init.h
  * Origin Date           :   08/02/2020
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_I2C_INIT_H
#define _RE_I2C_INIT_H

/* Includes */
#include "re_std_def.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

/* Exported API */
RE_StatusTypeDef RE_I2C1_Init(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);

#endif
/**************************** END OF FILE *************************************/
