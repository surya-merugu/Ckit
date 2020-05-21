/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_MC60_TCPIP.c
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
  * The following is the sequence of commands:
  * Step #0  : MC60_TCPIP_Deact();                                     Send "AT+QIDEACT\r" 
  * Step #1  : MC60_TCPIP_Select_Transfer_mode(0);                     Send "AT+QIMODE=0\r" 
  * Step #2  : MC60_TCPIP_Set_GPRS(1);                                 Send "AT+QICSGP=1,\"www\"\r"
  * Step #3  : MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password();  Send "AT+QIREGAPP\r"
  * Step #4  : MC60_TCPIP_Activate_GPRS_Context(); 
  * Step #5  : MC60_TCPIP_Get_LocalIP(); 
  * Step #6  : MC60_Send_Command_Check_Response("ATV1\r", 300);
  * Step #7  : MC60_TCPIP_IPheader_in_data(1);
  * Step #8  : MC60_TCPIP_Connect_with_IP_DNS(1);
  * Step #9  : MC60_TCPIP_Context_Set_To_Foreground(1);
  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gps_gprs/re_app_MC60.h"
#include "gps_gprs/re_app_UART_Ring.h"
#include <string.h>
    
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Deact(void);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Select_Transfer_mode(uint8_t mode);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Set_GPRS(uint8_t mode);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password(void);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Get_LocalIP(void);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_IPheader_in_data(uint8_t mode);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Activate_GPRS_Context(void);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Context_Set_To_Foreground(uint8_t mode);
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Connect_with_IP_DNS(uint8_t mode);


/****************************** TCPIP FUNCTIONS ******************************/

MC60_TCPIP_StatusTypeDef MC60_TCPIP_Init(void)
{
    if (MC60_TCPIP_Deact() != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Select_Transfer_mode(0) != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Set_GPRS(1) != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password() != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Activate_GPRS_Context() != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Get_LocalIP() != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }

    (void)MC60_Send_Command_Check_Response("ATV1\r", 300);

    if (MC60_TCPIP_IPheader_in_data(1) != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Connect_with_IP_DNS(1) != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    if (MC60_TCPIP_Context_Set_To_Foreground(0) != MC60_TCPIP_OK)
    {
        // ASSERT failed.
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }

    return MC60_TCPIP_OK;
}

//QIOPEN - maximum response time : 75 seconds depending on network
//TO-DO: Error Handling
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Open(void)
{
    uint32_t tickstart = 0U;
    uint32_t Timeout = 75000U;

    UART_Sendstring("AT+QIOPEN=\"TCP\",\"api.thingspeak.com\",\"80\"\r");
    HAL_Delay(1000);

    tickstart = HAL_GetTick();

    while (!((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))) // wait for response for given time.
    {
        char response[140] = {0};
        Get_String(response);
        if (Search_for_substring(response, "CONNECT OK"))
        {
            //    printf("CONNECTED\n");
            return MC60_TCPIP_OK;
        }
        else if (Search_for_substring(response, "ALREADY CONNECT"))
        {
            //    printf("ALREADY CONNECTED\n");
            return MC60_TCPIP_OK;
        }
        else if (Search_for_substring(response, "CONNECT FAIL"))
        {
            UART_Sendstring("AT+QISTAT\r");
            HAL_Delay(500);
            char sub_response[140] = {0};
            Get_String(sub_response);
            if (Search_for_substring(sub_response, "TCP CONNECTING"))
            {
                MC60_TCPIP_Close();
                HAL_Delay(500);
                // MC60_TCPIP_Init();
            }
            else
            {
                MC60_TCPIP_Deact();
                HAL_Delay(500);
                // MC60_TCPIP_Init();
            }
            RE_Error_Handler(__FILE__, __LINE__);
            return MC60_TCPIP_ERROR;
        }
        else if (Search_for_substring(response, "ERROR"))
        {
            UART_Sendstring("AT+QIMUX?\r");
            char sub_response[140] = {0};
            Get_String(sub_response);
            if (Search_for_substring(sub_response, "QIMUX: 1"))
            {
                UART_Sendstring("AT+QIMUX=0\r");
                // MC60_TCPIP_Init();
            }
            else
            {
                UART_Sendstring("AT+QISTAT\r");
                char sub_sub_response[140] = {0};
                Get_String(sub_sub_response);
                if (Search_for_substring(sub_response, "TCP CONNECTING"))
                {
                    MC60_TCPIP_Close();
                    RE_Error_Handler(__FILE__, __LINE__);
                    return MC60_TCPIP_ERROR;
                    // MC60_TCPIP_Init();
                }
            }
        }
    }
    RE_Error_Handler(__FILE__, __LINE__);
    return MC60_TCPIP_TIMEOUT;
}

//QICLOSE - maximum response time : 300ms
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Close(void)
{
    if (MC60_Send_Command_Check_Response("AT+QICLOSE\r", 300) != MC60_OK)
    {
        HAL_Delay(200);
        RE_Error_Handler(__FILE__, __LINE__);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QISEND - maximum response time : 300ms
//TO-DO: Read response and Error Handling
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Send(char *s)
{
    UART_Sendstring("AT+QISEND\r");
    HAL_Delay(3000); //this delay is not acceptable and should be changed to OS_delay once switched to RTOS
    UART_Sendstring(s);
    HAL_Delay(10000);
    return MC60_TCPIP_OK;
    volatile int response = Get_Position_First_Encounter("AT+QISEND\r");
    if (response != -1)
    {
        volatile int write_cmd_status = Get_Position_First_Encounter(">");
        while (write_cmd_status == -1) // wait till ">" appears. But interrupt might be triggers before that?
        {
            write_cmd_status = Get_Position_First_Encounter(">");
        }
        UART_Sendstring(s);
        HAL_Delay(5000);
        char string[100] = {0};
        Get_String(string);
        uint8_t ok_status = Search_for_substring(string, "OK");
        uint8_t error_status = Search_for_substring(string, "ERROR");
        uint8_t fail_status = Search_for_substring(string, "FAIL");
        HAL_Delay(10);
        if (ok_status)
        {
            return MC60_TCPIP_OK;
        }
        else if (error_status)
        {
            RE_Error_Handler(__FILE__, __LINE__);
            return MC60_TCPIP_ERROR;
        }
        else if (fail_status)
        {
            return MC60_TCPIP_OK;
        }
    }
    RE_Error_Handler(__FILE__, __LINE__);
    return MC60_TCPIP_ERROR;
}

//QIDEACT - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Deact(void)
{
    if (MC60_Send_Command_Check_Response("AT+QIDEACT\r", 40000) == MC60_TIMEOUT)
    {
        //    UART_Sendstring("AT+QISTAT\r");
        //    char sub_response[140] = {0};
        //    Get_String(sub_response);
        //    if (Search_for_substring(sub_response, "TCP CONNECTING"))
        //    {
        (void)MC60_TCPIP_Close();
        //    }
        //    else
        //    {
        //      (void) MC60_TCPIP_Deact();
        //    }
        HAL_Delay(200);
    }
    return MC60_TCPIP_OK;
}

//QIMODE - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Select_Transfer_mode(uint8_t mode)
{
    const char *cmd;
    //  UART_Sendstring("AT+QIMODE?\r");
    //	HAL_Delay(300);
    if (mode == 0U)
    {
        cmd = "AT+QIMODE=0\r";
    }
    else
    {
        cmd = "AT+QIMODE=1\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QICSGP - maximum response time : 300ms AT+QICSGP=1,\"www\"
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Set_GPRS(uint8_t mode)
{
    const char *cmd;
    if (mode == 0U)
    {
        // cmd = "AT+QICSGP=0,\"www\"\r";
        cmd = "AT+QICSGP=0,\"airtelgprs.com\"\r";
    }
    else
    {
        // cmd = "AT+QICSGP=1,\"www\"\r";
        cmd = "AT+QICSGP=1,\"airtelgprs.com\"\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QIREGAPP - maximum response time : 300ms AT+QIREGAPP
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password(void)
{
    if (MC60_Send_Command_Check_Response("AT+QIREGAPP\r", 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QIACT - maximum response time : 150 seconds depending on network AT+QIACT
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Activate_GPRS_Context(void)
{
    if (MC60_Send_Command_Check_Response("AT+QIACT\r", 150000) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QILOCIP - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Get_LocalIP(void)
{
    static uint8_t counter;
    if (MC60_Send_Command_Check_Response("AT+QILOCIP\r", 300) == MC60_ERROR)
    {
        HAL_Delay(200);
        (void)MC60_TCPIP_Deact();                                    //QIDEACT
        (void)MC60_TCPIP_Select_Transfer_mode(0);                    //QIMODE
        (void)MC60_TCPIP_Set_GPRS(1);                                //AT+QICSGP=1,\"www\"
        (void)MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password(); //QIREGAPP
        (void)MC60_TCPIP_Activate_GPRS_Context();                    //QIACT
        if (counter < 3U)
        {
            (void)MC60_TCPIP_Get_LocalIP(); //QILOCIP
        }
        else
        {
            counter = 0U;
            return MC60_TCPIP_ERROR;
        }
        HAL_Delay(500);
    }
    return MC60_TCPIP_OK;
}

//QIHEAD - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_IPheader_in_data(uint8_t mode)
{
    const char *cmd;
    if (mode == 0U)
    {
        cmd = "AT+QIHEAD=0\r";
    }
    else
    {
        cmd = "AT+QIHEAD=1\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QIDNSIP - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Connect_with_IP_DNS(uint8_t mode)
{
    const char *cmd;
    if (mode == 0U)
    {
        cmd = "AT+QIDNSIP=0\r";
    }
    else
    {
        cmd = "AT+QIDNSIP=1\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}

//QIFGCNT - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Context_Set_To_Foreground(uint8_t mode)
{
    const char *cmd;
    if (mode == 0U)
    {
        cmd = "AT+QIFGCNT=0\r";
    }
    else
    {
        cmd = "AT+QIFGCNT=1\r";
    }
    if (MC60_Send_Command_Check_Response(cmd, 300) != MC60_OK)
    {
        HAL_Delay(200);
        return MC60_TCPIP_ERROR;
    }
    return MC60_TCPIP_OK;
}
/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/