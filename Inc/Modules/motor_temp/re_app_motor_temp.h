/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_motor_temp.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_APP_MOTOR_TEMP_H
#define _RE_APP_MOTOR_TEMP_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "re_std_def.h"

extern RE_StatusTypeDef RE_THC_indicator_On(void);
extern RE_StatusTypeDef RE_THC_indicator_Off(void);
extern uint8_t RE_thc_Status(void);


#endif
/**************************** END OF FILE *************************************/
