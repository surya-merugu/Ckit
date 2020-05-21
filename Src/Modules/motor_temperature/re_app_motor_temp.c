/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_motor_temp.c
  * Origin Date           :   16/03/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   thermal cutoff
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
#include "motor_temp/re_app_motor_temp.h"

uint8_t thc_status;


/**
 * @brief RE thc indicator on
 * This function turns on the thc indicator
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_THC_indicator_On(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    return RE_OK;
}

/**
 * @brief RE thc indicator off
 * This function turns on the thc indicator
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_THC_indicator_Off(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    return RE_OK;
}

/**
 * @brief RE Dock Latch Status
 * This function gets the latch status
 * @param  None
 * @retval uint8_t
 */

uint8_t RE_thc_Status(void)
{
    thc_status = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7);

}



/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/