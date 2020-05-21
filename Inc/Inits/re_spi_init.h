/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_spi_init.h
  * Origin Date           :   20/05/2020
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
  * Written by Team RACEnergy, MAY 2020
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_SPI_INIT_H
#define _RE_SPI_INIT_H

/* Includes */
#include "re_std_def.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"

/* Exported API */
RE_StatusTypeDef RE_SPI_Init(void);

extern SPI_HandleTypeDef hspi1_t;

#endif
/**************************** END OF FILE *************************************/
