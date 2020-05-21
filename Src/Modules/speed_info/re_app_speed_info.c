/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_speed_info.c
  * Origin Date           :   15/02/2020
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
#include "speed_info/re_app_speed_info.h"
#include "dash_lcd/re_app_dash_lcd.h"

uint32_t kmph;
uint32_t RpmCnt;
double odo;
uint8_t save_odo;

extern float gear_ratio;
extern FSTNLCD_data_t LcdDisplay;


/**
 * @brief RE_speed
 * This function calculates the speed of the motor
 * @param  None
 * @retval kmph
 */
uint32_t RE_speed(void)
{

 //   kmph = RpmCnt*15*50/653/gear_ratio;
    kmph = RpmCnt*28.18/gear_ratio/653*50/4*0.865; /* Divided by factor 4 */
//    kmph = RpmCnt/gear_ratio*2.432432/8;
    LcdDisplay.rpm_eff = RpmCnt;
    RpmCnt = 0;
    return LcdDisplay.speed = kmph;
}


uint32_t RE_distance_covered(void)
{
    
    odo += (double) kmph/7200;
    if (odo >= 1) 
    {
        save_odo = 1;
        odo = 0;
    }
    return 0;
}

#if 0
/**
 * @brief callback ISR on speed pin
 * This function increments RPM count whenever inkoved
 * @param  None
 * @retval exit status
 */
RE_StatusTypeDef RE_Speed_ISR (void)
{
    RpmCnt++;
    return RE_OK;
}
#endif

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/