/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_key_position.c
  * Origin Date           :   16/03/2020
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

/* Includes */
#include "key_position/re_app_key_position.h"
#include "dash_lcd/re_app_dash_lcd.h"

uint8_t key_status;
uint8_t animate;
extern FSTNLCD_data_t LcdDisplay;



/**
 * @brief callback ISR on key positon
 * This function starts the animation when key position is detected 
 * @param  None
 * @retval exit status
 */
RE_StatusTypeDef RE_key_pos_ISR (void)
{
    

    return RE_OK;
}


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/