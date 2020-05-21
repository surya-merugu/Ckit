/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_dock_latch.h
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   OptoCoupler Sensor
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
#ifndef _RE_APP_DOCK_LATCH_H
#define _RE_APP_DOCK_LATCH_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "re_std_def.h"

extern RE_StatusTypeDef RE_OpenDock_Latch(void);
extern RE_StatusTypeDef RE_CloseDock_Latch(void);
extern uint8_t RE_DockLatch_Status(void);


#endif
/**************************** END OF FILE *************************************/
