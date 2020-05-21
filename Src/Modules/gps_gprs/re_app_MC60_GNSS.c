/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_MC60_GNSS.c
  * Origin Date           :   25/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   MC60
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

/**
  ==============================================================================
                     ##### What happens in this driver #####
  ==============================================================================
  * THIS DRIVER SENDS APPROPRIATE COMMANDS THROUGH UART TO MC60 MODULE
  * MC60_GNSS_Init - Initializes and executes the string of serial commands.
  * Each function returns the status of the serial commmand sent.
  *         - OK
  *         - ERROR
  * The following is the sequence of commands:
  * Step #1  : MC60_GNSS_Time_Sync_Status()
  * Step #2  : MC60_GNSS_Set_EPO(0)
  * Step #3  : MC60_GNSS_Set_EPO(1)
  * Step #4  : MC60_GNSS_Trigger_EPO()
  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gps_gprs/re_app_MC60.h"
#include "gps_gprs/re_app_UART_Ring.h"
#include "gps_gprs/re_app_gps.h"
#include <stdio.h>
#include <string.h>

/* Variables -----------------------------------------------------------------*/
extern gps_t hgps_t;


static MC60_GNSS_StatusTypeDef MC60_GNSS_Poweron(uint8_t mode);
static MC60_GNSS_StatusTypeDef MC60_GNSS_Time_Sync_Status(void);
static MC60_GNSS_StatusTypeDef MC60_GNSS_Set_EPO(void);
static MC60_GNSS_StatusTypeDef MC60_GNSS_Trigger_EPO(void);

/****************************** GNSS FUNCTIONS ******************************/

MC60_GNSS_StatusTypeDef MC60_GNSS_Init(void)
{
    if (MC60_GNSS_Time_Sync_Status() != MC60_GNSS_OK)
    {
        // ASSERT : MC60_GNSS_Time_Sync_Status()failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GNSS_ERROR;
    }
    if (MC60_GNSS_Set_EPO() != MC60_GNSS_OK)
    {
        // ASSERT : MC60_GNSS_Set_EPO(1)failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GNSS_ERROR;
    }
    if (MC60_GNSS_Trigger_EPO() != MC60_GNSS_OK)
    {
        // ASSERT : MC60_GNSS_Trigger_EPO()failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GNSS_ERROR;
    }
    return MC60_GNSS_OK;
}

static MC60_GNSS_StatusTypeDef MC60_GNSS_Poweron(uint8_t mode)
{
    const char *cmd;
    if (mode == 0U)
    {
        cmd = "AT+QGNSSC=0\r";
    }
    else
    {
        cmd = "AT+QGNSSC=1\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_GNSS_ERROR;
    }
    return MC60_GNSS_OK;
}

//QGNSSRD
MC60_GNSS_StatusTypeDef MC60_GNSS_Read_Nav_Info_to_GPS_DATA(void)
{
    UART_Sendstring("AT+QGNSSRD=\"NMEA\/GGA\"\r");
    // UART_Sendstring("AT+QGNSSRD=\"NMEA\/RMC\"\r");
    HAL_Delay(1000);
    char string[140];
    Get_String(string);

    if (Search_for_substring(string, "ERROR"))
    {
        return MC60_GNSS_ERROR;
    }
    if (Search_for_substring(string, "+QGNSSRD"))
    {
        char sub[140] = {0};
        uint8_t i = 0;
        while (i < 140)
        {
            sub[i] = string[34 + i];
            i++;
        }
    char *token = strtok(sub, "\n");
    // printf("%s\n", token);
    gps_process(&hgps_t, token, strlen(token));
    // if (hgps_t.is_valid != 0)
    // {
    //   printf("Valid status: %d\r\n", hgps_t.is_valid);
    //   printf("date: %d\r\n", hgps_t.date);
    //   printf("month: %d\r\n", hgps_t.month);
    //   printf("year: %d\r\n", hgps_t.year);
    //   printf("Latitude: %f degrees\r\n", hgps_t.latitude);
    //   printf("Longitude: %f degrees\r\n", hgps_t.longitude);
    //   printf("Altitude: %f meters\r\n", hgps_t.altitude);
    // }
        return MC60_GNSS_OK;
    }
    return MC60_GNSS_ERROR;
}

//QGNSSCMD - currently on cmdyTpe = 0 is supported
//MC60_GNSS_StatusTypeDef MC60_GNSS_Send_GNSS_CMD(uint8_t cmdType, uint8_t *cmdString)
//{
//}

//QGNSSTS
static MC60_GNSS_StatusTypeDef MC60_GNSS_Time_Sync_Status(void)
{
    if (MC60_Send_Command_Check_Response("AT+QGNSSTS?\r", 300) != MC60_OK)
    {
        static uint8_t counter;
        HAL_Delay(200);
        if (counter < 3)
        {
            (void)MC60_Send_Command_Check_Response("AT+QNTP\r", 300); //this is giving error
            (void)MC60_Send_Command_Check_Response("AT+QGNSSTS?\r", 300);
        }
        else
        {
            return MC60_GNSS_ERROR;
        }
    }
    return MC60_GNSS_OK;
}

//QGNSSEPO
static MC60_GNSS_StatusTypeDef MC60_GNSS_Set_EPO(void)
{
    char data[40] = {0};
    const char *cmd;
    UART_Sendstring("AT+QGNSSEPO?\r");
    HAL_Delay(1000);
    Get_String(data);
    if (Search_for_substring(data, "+QGNSSEPO: 0"))
    {
        cmd = "AT+QGNSSEPO=1\r";
    }
    else
    {
        return MC60_GNSS_OK;
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_GNSS_ERROR;
    }
    return MC60_GNSS_OK;
}

//Set Reference Location Information for QuecFastFix Online
//https://www.embedded.com/floating-point-data-in-embedded-software/
//MC60_GNSS_StatusTypeDef MC60_GNSS_Set_Ref_Location_QFF(float ref_latitude, float ref_longitude)
//{
//}

//QGEPOAID
static MC60_GNSS_StatusTypeDef MC60_GNSS_Trigger_EPO(void)
{
    if (MC60_Send_Command_Check_Response("AT+QGEPOAID\r", 300) != MC60_OK)
    {
        static uint8_t counter;
        MC60_GNSS_Poweron(0);
        MC60_GNSS_Poweron(1);
        MC60_GNSS_Time_Sync_Status();
        MC60_GNSS_Set_EPO();
        ++counter;
        if (counter < 3) //try 3 times
        {
            MC60_GNSS_Trigger_EPO();
            HAL_Delay(300);
        }
        else
        {
            return MC60_GNSS_ERROR;
        }
    }
    return MC60_GNSS_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/