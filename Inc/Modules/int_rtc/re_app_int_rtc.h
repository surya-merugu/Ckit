/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_int_rtc.h
  * Origin Date           :   12/02/2020
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
#ifndef _RE_APP_INT_RTC_H
#define _RE_APP_INT_RTC_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rtc.h"
#include "re_std_def.h"

RE_StatusTypeDef RE_RTC_Init(void);
RE_StatusTypeDef RE_GetTimeStamp(void);
RE_StatusTypeDef RE_SetTimeStamp(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);

#endif
/**************************** END OF FILE *************************************/
