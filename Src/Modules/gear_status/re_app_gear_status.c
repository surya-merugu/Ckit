/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_gear_status.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Gear Position sensor
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
#include "gear_status/re_app_gear_status.h"
#include "dash_lcd/re_app_dash_lcd.h"


uint8_t GearMode_statusA,GearMode_statusB,GearDirection_statusA,GearDirection_statusB;
const float high_gear_ratio = 7.19;
const float low_gear_ratio  = 13.9;
float gear_ratio;
extern FSTNLCD_data_t LcdDisplay;

extern RE_StatusTypeDef RE_Turn_RevLight_On(void);
extern RE_StatusTypeDef RE_Turn_RevLight_Off(void);



/**
 * @brief RE_Gear_Mode
 * this function checks for the gear mode
 * @param gear mode pins status
 * @retval Gear Mode
 */
uint8_t RE_Gear_Mode (uint8_t Gear_ModeA, uint8_t Gear_ModeB)
{
    if (Gear_ModeA==0 && Gear_ModeB==1)    //gear low mode
    {
        gear_ratio = low_gear_ratio;
        return LcdDisplay.gear = 1;
    }
    else if (Gear_ModeA==1 && Gear_ModeB==0) //gear auto
    {
        gear_ratio = high_gear_ratio;
        return LcdDisplay.gear = 2;
    }
    else if (Gear_ModeA==0 && Gear_ModeB==0) //gear high
    {
        gear_ratio = high_gear_ratio;
        return LcdDisplay.gear = 3;
    }
    else
    {
        return LcdDisplay.gear = 0;
    }
}


/**
 * @brief RE_Gear_Direction
 * this function checks for the gear direction
 * @param gear direction pins status
 * @retval Gear Direction
 */
uint8_t RE_Gear_Direction (uint8_t Gear_directionA, uint8_t Gear_directionB)
{
    if (Gear_directionA==0 && Gear_directionB==1)       //gear neutral`
    {
        return LcdDisplay.gear_direction = 1;
    }
    else if (Gear_directionA==1 && Gear_directionB==0)  //gear reverse
    {
        return LcdDisplay.gear_direction = 2;
    }
    else if (Gear_directionA==0 && Gear_directionB==0)  //gear drive
    {
        return LcdDisplay.gear_direction = 3;
    }
    else
    {
        return LcdDisplay.gear_direction = 0;
    }
}


/**
 * @brief callback ISR on gear sensor pins
 * This function reads the status of gear
 * @param GPIO Pin number
 * @retval exit status
 */
RE_StatusTypeDef RE_Read_Gear_Status(void)
{

        GearMode_statusA =      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4);
        GearMode_statusB =      HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5);
        GearDirection_statusA = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3);
        GearDirection_statusB = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
        GearDirection_statusB = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
    return RE_OK;
}


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/