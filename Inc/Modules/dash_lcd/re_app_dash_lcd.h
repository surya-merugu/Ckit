/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_dash_lcd.h
  * Origin Date           :   10/02/2020
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
  * Written by Team RACEnergy, FEB 2020
  *****************************************************************************
  */

/******** Define to prevent recursive inclusion*****************************/
#ifndef _RE_APP_DASH_LCD_H
#define _RE_APP_DASH_LCD_H

/* Includes */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#include "main/main.h"
#include "re_lcd_gpio_init.h"
#include "dash_lcd/re_driver_dash_lcd.h"
#include "battery_info/re_app_battery_info.h"

#include "stdint.h"
#include "stdbool.h"

/*------------------------------    Macros    -------------------------------*/
/* Equivalent hex value to display digit */
#define ZERO 0xBE
#define ONE 0x06
#define TWO 0x7C
#define THREE 0x5E
#define FOUR 0xC6
#define FIVE 0xDA
#define SIX 0xFA
#define SEVEN 0x0E
#define EIGHT 0xFE
#define NINE 0xDE

/* Equivalent hex value to dispaly segment */
#define LOW 0XB0
#define HIGH 0XE6
#define AUTO 0XEE

/* TypeDefs */
typedef struct
{
    uint64_t seg[2];
} CS_seg_t;

/* TN LCD Display Data */
typedef struct
{
    uint16_t err_code;
    uint8_t lock_status;

    uint8_t speed;
    uint8_t speed_graph;

    uint8_t gear;
    uint8_t gear_direction;

    uint8_t motor_temp;
    uint8_t motor_fault_graph;
    uint8_t kit_fault_graph;

    uint8_t bat_temp;
    uint16_t bat_volt;
    uint8_t bat_soc;
    uint8_t bat_soc_graph;
    uint8_t bat_fault_graph;

    uint8_t range;

    uint32_t odo;
    uint8_t odo_graph;
    uint8_t current_in_amp[4];
    uint16_t rpm_eff;
    uint8_t efficiency;
} FSTNLCD_data_t;

/* Exported API's */
extern RE_StatusTypeDef RE_lcd_start_animation(void);
extern RE_StatusTypeDef RE_RefreshScreen(void);
extern RE_StatusTypeDef RE_update_lcd_static_seg(void);

#endif
/**************************** END OF FILE *************************************/
