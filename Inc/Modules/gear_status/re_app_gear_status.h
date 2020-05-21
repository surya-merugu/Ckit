/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_gear_status.h
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Gear Position Sensor
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
#ifndef _RE_APP_GEAR_STATUS_H
#define _RE_APP_GEAR_STATUS_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "re_std_def.h"
#include "stm32f4xx_it.h"

extern uint8_t RE_Gear_Mode (uint8_t Gear_ModeA, uint8_t Gear_ModeB);
extern uint8_t RE_Gear_Direction (uint8_t Gear_directionA, uint8_t Gear_directionB);
RE_StatusTypeDef RE_Read_Gear_Status(void);

#endif
/**************************** END OF FILE *************************************/
