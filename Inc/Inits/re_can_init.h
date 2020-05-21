/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_can_init.h
  * Origin Date           :   13/02/2020
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
#ifndef _RE_CAN_INIT_H
#define _RE_CAN_INIT_H

/* Includes */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_gpio.h"

#include "re_std_def.h"

RE_StatusTypeDef RE_CAN1_Init(void);
RE_StatusTypeDef RE_CAN1_Filter_Config(void);
RE_StatusTypeDef RE_CAN1_Start_Interrupt(void);
#endif
/**************************** END OF FILE *************************************/