/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_battery_info.h
  * Origin Date           :   13//02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Battery Pack with CAN
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
#ifndef _RE_APP_BATTERY_INFO_H
#define _RE_APP_BATTERY_INFO_H

#include "main/main.h"
#include "re_can_init.h"

uint16_t Byte_Swap(uint16_t high_byte, uint16_t low_byte);
RE_StatusTypeDef RE_Req_BatInfo(void);
RE_StatusTypeDef RE_GetBatInfoErr(void);

#endif
/**************************** END OF FILE *************************************/
