/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_ext_rtc.c
  * Origin Date           :   08/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   DS1307
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

/* Includes */
#include "ext_rtc/re_app_ext_rtc.h"

I2C_HandleTypeDef hi2c1;
extern timestamp_t time_t;

RE_StatusTypeDef RE_RTC_SetTime(void)
{
    uint8_t cmd = 0x01; /** Write new time to the address : @cmd */
    uint8_t SetTime[8];
    SetTime[0] = 0x00;
    SetTime[1] = 0x00; /* Set time in seconds */
    SetTime[2] = 0x24; /* Set time in minutes */
    SetTime[3] = 0x16; /* Set time in hours */
    SetTime[4] = 0x06; /* Set day of the week */
    SetTime[5] = 0x15; /* Set date */
    SetTime[6] = 0x11; /* Set month */
    SetTime[7] = 0x19; /* Set year*/
    if (HAL_I2C_Master_Transmit(&hi2c1, RTC_WR_ADDR, &cmd, sizeof(cmd), 1000) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    /* Master request Slave for data */
    if (HAL_I2C_Master_Transmit(&hi2c1, RTC_WR_ADDR, SetTime, sizeof(SetTime), 1000) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    return RE_OK;
}

RE_StatusTypeDef RE_RTC_GetTimeStamp(void)
{
    uint8_t rawdata[8];
    if (HAL_I2C_Master_Transmit(&hi2c1, RTC_WR_ADDR, 0x00, sizeof(uint8_t), 1000) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    /* Master request Slave for 8 bytes of data */
    if (HAL_I2C_Master_Receive(&hi2c1, RTC_RD_ADDR, rawdata, 8, 1000) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    return RE_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/