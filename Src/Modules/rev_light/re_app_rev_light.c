/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_rev_light.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Optocoupler Sensor
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
#include "rev_light/re_app_rev_light.h"

/**
 * @brief RE Turn RevLight On
 * This function turns on the relay in reverse direction
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_Turn_RevLight_On(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    return RE_OK;
}

/**
 * @brief RE Turn RevLight Off
 * This function turns off the relay other than reverse direction
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_Turn_RevLight_Off(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    return RE_OK;
}


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/