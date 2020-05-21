/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_MC60_GPRS.c
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
  @verbatim
  ==============================================================================
                     ##### What happens in this driver #####
  ==============================================================================
  * THIS DRIVER SENDS APPROPRIATE COMMANDS THROUGH UART TO MC60 MODULE
  * MC60_GPRS_Init - Initializes and executes the string of serial commands.
  * Each function returns the status of the serial commmand sent.
  *         - OK
  *         - ERROR
  * All functions are exposed in the API for now. 
  * The following is the sequence of commands:
  * Step #1  :   MC60_GPRS_Test_SIM();                    //CPIN
  * Step #2  :   MC60_GPRS_Network_Registration();        //CREG
  * Step #3  :   MC60_GPRS_Network_Registration_Status(); //CGREG
  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gps_gprs/re_app_MC60.h"
#include "gps_gprs/re_app_UART_Ring.h"
#include <string.h>
    
static MC60_GPRS_StatusTypeDef MC60_GPRS_Test_SIM(void);
static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration(void);
static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration_Status(void);

/****************************** GPRS FUNCTIONS ******************************/

MC60_GPRS_StatusTypeDef MC60_GPRS_Init(void)
{
    if (MC60_GPRS_Test_SIM() != MC60_GPRS_OK)
    {
        // ASSERT : MC60_GPRS_Test_SIM() failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GPRS_ERROR;
    }  
    if (MC60_GPRS_Network_Registration() != MC60_GPRS_OK)
    {
        // ASSERT : MC60_GPRS_Network_Registration()failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GPRS_ERROR;
    }
    if (MC60_GPRS_Network_Registration_Status() != MC60_GPRS_OK)
    {
        // ASSERT : MC60_GPRS_Network_Registration_Status() failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_GPRS_ERROR;
    }
    return MC60_GPRS_OK;
}

static MC60_GPRS_StatusTypeDef MC60_GPRS_Test_SIM(void) //CPIN
{
    if (MC60_Send_Command_Check_Response("AT+CPIN?\r", 5000) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_GPRS_ERROR;
    }
    return MC60_GPRS_OK;
}

static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration(void) //CREG
{
    if (MC60_Send_Command_Check_Response("AT+CREG?\r", 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_GPRS_ERROR;
    }
    return MC60_GPRS_OK;
}

static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration_Status(void) //CGREG
{
    if (MC60_Send_Command_Check_Response("AT+CGREG?\r", 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_GPRS_ERROR;
    }
    return MC60_GPRS_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/