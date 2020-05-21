/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_MC60.c
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
  * MC60_Init() - Initializes and executes the string of serial commands.
  * Each function returns the status of the serial commmand sent.
  *         - OK
  *         - ERROR
  *         - TIMEOUT / NO RESPONSE
  * The commands are separated into:
  *         - GENERAL
  *         - GPRSS
  *         - TCPIP
  *         - GNSS
  * All functions are exposed in the API for now. 
  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gps_gprs/re_app_MC60.h"
#include "gps_gprs/re_app_UART_Ring.h"
#include "gps_gprs/re_app_gps.h"
#include <string.h>
#include <stdio.h>

extern gps_t hgps_t;

static MC60_StatusTypeDef MC60_Test(void);

/* MC60_ Initialization, de-initialization and test Functions */
MC60_StatusTypeDef MC60_Init(void)
{
    /* Return function status */
    if (MC60_Test() != MC60_OK)
    {
        //ASSERT what went wrong
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_ERROR;
    }
    if (MC60_GPRS_Init() != MC60_GPRS_OK)
    {
        //ASSERT what went wrong
        return MC60_ERROR;
    }
    if (MC60_GNSS_Init() != MC60_GNSS_OK)
    {
        //ASSERT what went wrong
        return MC60_ERROR;
    }
  //  timer_get_GPS = 3000;
  // once gnss initialization is successful have an interrupt periodically get GPS data.
  //  while(MC60_TCPIP_Init() != MC60_TCPIP_OK) //try till TCPIP is initialized successfully.
  //  {
  //    // get GPS Data here?
  //    MC60_Read_Send_GPS_Data_on_TCPIP();
  //  }
  //  if(MC60_TCPIP_Init() != MC60_TCPIP_OK)
  //  {
  //    //ASSERT what went wrong
  //    return MC60_ERROR;
  //  }
    return MC60_OK;
}

MC60_StatusTypeDef MC60_Send_Command_Check_Response(const char *s, uint32_t Timeout)
{
    uint32_t tickstart = 0U;
    char string[200] = {0};

    UART_Sendstring(s);
    HAL_Delay(1000); //this delay is not acceptable and should be changed to OS_delay once switched to RTOS

    tickstart = HAL_GetTick();

    while (!((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))) // wait for response for given time.
    {
        // Be here till the response is OK or ERROR!
        Get_String(string);
        uint8_t ok_status = Search_for_substring(string, "OK");
        uint8_t error_status = Search_for_substring(string, "ERROR");
        HAL_Delay(10);
        if (ok_status)
        {
            return MC60_OK;
        }
        else if (error_status)
        {
            return MC60_ERROR;
        }
    }
    return MC60_TIMEOUT;
}

// Function contains both TCPIP and GNSS function. Therefore it is placed in general.
// TO-DO: error handling and make it general
MC60_StatusTypeDef MC60_Read_Send_GPS_Data_on_TCPIP()
{
    if (MC60_GNSS_Read_Nav_Info_to_GPS_DATA() != MC60_GNSS_OK)
    {
        //Assert
        //Error Handler
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_ERROR;
    }
    if (MC60_TCPIP_Open() != MC60_TCPIP_OK)
    {
        //ASSERT ERROR HANDLER
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_ERROR;
    }

    char whole_string[150] = {0};
    char sub[5] = {0};
    sprintf(sub, "%d", hgps_t.date);
  // uint8_t i = 0;
  // //edit the -GPS_DATA string to smaller string -sub
  // while (i < 40)
  // {
  //   sub[i] = GPS_DATA[35 + i];
  //   i++;
  // }
    strcpy(whole_string, "GET https://api.thingspeak.com/update?api_key=48NXHGFM8U1V6SZX&field1=");
//  strcat(whole_string, sub);
    strcat(whole_string, "hello kitty");
    strcat(whole_string, "\r\n\x1a");

    if (MC60_TCPIP_Send(whole_string) != MC60_TCPIP_OK)
    {
        //ASSERT ERROR HANDLER
        return MC60_ERROR;
    }
  //    ++success_counter;
    return MC60_OK;
}

static MC60_StatusTypeDef MC60_Test(void)
{
    return MC60_Send_Command_Check_Response("AT\r", 300);
}

// Search_for_substring function definition
uint8_t Search_for_substring(char *str, char *sub)
{
    uint8_t flag = 0U;
    uint8_t i = 0U, len1 = 0U, len2 = 0U;
    len1 = (uint8_t)strlen(str);
    len2 = (uint8_t)strlen(sub);
    while (*(str + i) != '\0')
    {
        if (*(str + i) == *(sub))
        {
            if ((i + len2) > len1)
            {
                break;
            }
            if (strncmp(str + i, sub, len2) == 0)
            {
                flag = 1;
                break;
            }
        }
    i++;
    }
    return flag;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/