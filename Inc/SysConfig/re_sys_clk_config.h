/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_sys_clk_config.h
  * Origin Date           :   11/02/2020
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
#ifndef _RE_SYS_CLK_CONFIG_H
#define _RE_SYS_CLK_CONFIG_H

/* Includes */
#include "re_std_def.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"

RE_StatusTypeDef RE_SystemClock_Config(void);

#endif
/***************************** END OF FILE ************************************/
