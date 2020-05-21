/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_main.h
  * Origin Date           :   04/02/2020
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAIN_H
#define _MAIN_H

#include "stdint.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

#include "re_std_def.h"
#include "re_hal_msp.h"

#include "re_sys_clk_config.h"

#include "battery_info/re_app_battery_info.h"
#include "dash_lcd/re_app_dash_lcd.h"
#include "dock_latch/re_app_dock_latch.h"
#include "ext_rtc/re_app_ext_rtc.h"
#include "gear_status/re_app_gear_status.h"
#include "gps_gprs/re_app_gps.h"
#include "gps_gprs/re_app_MC60.h"
#include "gps_gprs/re_app_UART_Ring.h"
#include "int_rtc/re_app_int_rtc.h"
#include "motor_temp/re_app_motor_temp.h"
#include "nrf52/re_app_nrf52.h"
#include "obd/re_app_obd.h"
#include "rev_light/re_app_rev_light.h"
//#include "sdcard/re_app_sdcard.h"
#include "speed_info/re_app_speed_info.h"
#include "timers/re_app_timers.h"




/* System time stamp */
typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hrs;
    uint8_t day;
    uint8_t dd;
    uint8_t mm;
    uint8_t yy;
} timestamp_t;

/* Battery Data */
typedef struct
{
    uint32_t ID;
    /* High frequent data */
    uint8_t soc;
    uint16_t temp;
    uint16_t current;
    uint16_t volt;
    /* Mid frequent data */
    uint16_t avail_energy;
    uint16_t remain_cap;
    /* Low frequent data */
    uint16_t soh;
    uint16_t full_cap;
    uint16_t cycle_count;
} BatteryData_t;

/*Kelly Controller Data */
typedef struct 
{
  uint16_t rpm;
  uint16_t err_code;
}ControllerData_t;

typedef struct
{
  uint8_t  motor_temp;
  uint32_t Odo;
  uint32_t Odo_char;
}DATAI_t;

/* Log Data*/
typedef struct
{
    timestamp_t time_stamp;
    BatteryData_t bat[4];
    uint8_t  file_num;
    char*    pfile_name;
    char     file_name[40];
    char     Dir[20];
    uint16_t err;
    DATAI_t datai;
    ControllerData_t  ctrl;
} LogData_t;

#endif
/***************************** END OF FILE ************************************/
