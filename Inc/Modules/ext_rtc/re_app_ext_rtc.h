/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_ext_rtc.h
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
#ifndef _RE_APP_EXT_RTC_H
#define _RE_APP_EXT_RTC_H

/* Includes */
#include "main/main.h"
#include "re_i2c_init.h"

/* Macros */
#define RTC_WR_ADDR 0xD0 /* RTC Slave address - write */
#define RTC_RD_ADDR 0xD1 /* RTC Slave address - read  */

/* Exported API's */
RE_StatusTypeDef RE_RTC_GetTimeStamp(void);
RE_StatusTypeDef RE_RTC_SetTime(void);

#endif
/**************************** END OF FILE *************************************/
