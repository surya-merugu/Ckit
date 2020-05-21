/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_MC60.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RE_APP_MC60_H
#define _RE_APP_MC60_H

#include <stdint.h>
#include "main.h"
//#include "cmsis_os.h"

#define OK "OK\r\n"
#define ERROR_MC60 "ERROR"

  // General Functions
uint8_t Search_for_substring(char *str, char *sub);

/** 
 * @brief  MC60 Status structures definition  
 */
typedef enum
{
    MC60_OK = 0x00U,
    MC60_ERROR = 0x01U,
    MC60_TIMEOUT = 0x02U,
    MC60_OTHER = 0xFF
}MC60_StatusTypeDef;

typedef enum
{
    MC60_GPRS_OK = 0x03U,
    MC60_GPRS_ERROR = 0x04U
} MC60_GPRS_StatusTypeDef;

typedef enum
{
    MC60_TCPIP_OK = 0x05U,
    MC60_TCPIP_ERROR = 0x06U,
    MC60_TCPIP_TIMEOUT = 0x07U
} MC60_TCPIP_StatusTypeDef;

typedef enum
{
    MC60_GNSS_OK = 0x08U,
    MC60_GNSS_ERROR = 0x0AU
} MC60_GNSS_StatusTypeDef;

// MC60 functions - Module level functions.
MC60_StatusTypeDef MC60_Init(void);
MC60_StatusTypeDef MC60_Deinit(void);
static MC60_StatusTypeDef MC60_Test(void); //AT - maximum response time : 300ms
inline MC60_StatusTypeDef MC60_Send_Command_Check_Response(const char *s, uint32_t Timeout);
MC60_StatusTypeDef MC60_Send_Command_Get_String(const char *s);

// GPRS functions
MC60_GPRS_StatusTypeDef MC60_GPRS_Init(void);
MC60_GPRS_StatusTypeDef MC60_GPRS_Deinit(void);
static MC60_GPRS_StatusTypeDef MC60_GPRS_Test_SIM(void);                    //CPIN - maximum response time : 5 seconds
static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration(void);        //CREG - maximum response time : 300ms
static MC60_GPRS_StatusTypeDef MC60_GPRS_Network_Registration_Status(void); //CGREG - maximum response time : 300ms

 // TCPIP functions
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Init(void);
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Deinit(void);
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Open(void);                                            //QIOPEN - maximum response time : 75 seconds depending on network
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Close(void);                                           //QICLOSE - maximum response time : 300ms
MC60_TCPIP_StatusTypeDef MC60_TCPIP_Send(char *s);                                         //QISEND - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Deact(void);                                    //QIDEACT - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Select_Transfer_mode(uint8_t mode);             //QIMODE - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Set_GPRS(uint8_t mode);                         //QICSGP - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Start_Task_and_Set_APN_UserName_Password(void); //QIREGAPP - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Activate_GPRS_Context(void);                    //QIACT - maximum response time : 150 seconds depending on network
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Get_LocalIP(void);                              //QILOCIP - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Context_Set_To_Foreground(uint8_t mode);        //QIFGCNT - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_IPheader_in_data(uint8_t mode);                 //QIHEAD - maximum response time : 300ms
static MC60_TCPIP_StatusTypeDef MC60_TCPIP_Connect_with_IP_DNS(uint8_t mode);              //QIDNSIP - maximum response time : 300ms

// GNSS functions
MC60_GNSS_StatusTypeDef MC60_GNSS_Init(void);
static MC60_GNSS_StatusTypeDef MC60_GNSS_Poweron(uint8_t mode);
MC60_GNSS_StatusTypeDef MC60_GNSS_Read_Nav_Info_to_GPS_DATA(void);                                      //QGNSSRD
static MC60_GNSS_StatusTypeDef MC60_GNSS_Send_GNSS_CMD(uint8_t cmdType, uint8_t *cmdString);            //QGNSSCMD - currently on cmdyTpe = 0 is supported
static MC60_GNSS_StatusTypeDef MC60_GNSS_Time_Sync_Status(void);                                        //QGNSSTS
static MC60_GNSS_StatusTypeDef MC60_GNSS_Set_EPO(void);                                                 //QGNSSEPO
static MC60_GNSS_StatusTypeDef MC60_GNSS_Set_Ref_Location_QFF(float ref_latitude, float ref_longitude); //Set Reference Location Information for QuecFastFix Online
//https://www.embedded.com/floating-point-data-in-embedded-software/
static MC60_GNSS_StatusTypeDef MC60_GNSS_Trigger_EPO(void); //QGEPOAID

// SPECIAL functions
MC60_StatusTypeDef MC60_Read_Send_GPS_Data_on_TCPIP(void);

#endif
/**************************** END OF FILE *************************************/
