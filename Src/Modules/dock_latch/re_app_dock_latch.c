/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_dock_latch.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Optocoupler Relay
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
#include "dock_latch/re_app_dock_latch.h"
#include "dash_lcd/re_app_dash_lcd.h"
extern FSTNLCD_data_t LcdDisplay;

uint8_t latch_status;
uint8_t latch_status_flag;


/**
 * @brief RE open dock latch
 * This function opens the dock
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_OpenDock_Latch(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
    latch_status_flag = 1;
    return RE_OK;
}

/**
 * @brief RE close dock latch
 * This function closes the dock
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_CloseDock_Latch(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
    return RE_OK;
}

/**
 * @brief RE Dock Latch Status
 * This function gets the latch status
 * @param  None
 * @retval uint8_t
 */

uint8_t RE_DockLatch_Status(void)
{
    latch_status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
    if (latch_status == 0)
    {
        LcdDisplay.lock_status = 2;  //latch is open
    }
    else
    {
        LcdDisplay.lock_status = 0;  //latch is closed
    }

    return 0;
}



/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/