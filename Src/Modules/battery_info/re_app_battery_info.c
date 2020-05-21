/**
  *****************************************************************************
  * Title                 :   Conversion Kit
  * Filename              :   re_app_battery_info.c
  * Origin Date           :   13/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Battery Pack with CAN
  * Target                :   STN32F407-DISC
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
#include "battery_info/re_app_battery_info.h"

extern CAN_HandleTypeDef hcan1_t;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;

extern LogData_t LogData;
extern uint8_t bat1, bat2, bat3, bat4;

static const LogData_t nodata_t;
uint8_t bat_err_cnt;
uint16_t bat_error;

uint16_t Byte_Swap(uint16_t high_byte, uint16_t low_byte)
{
    uint16_t data;
    data = ((low_byte << 8) & (0xFF00)) | ((high_byte) & (0x00FF));

    return data;
}

/*  Send Bat_Data RTR */
RE_StatusTypeDef RE_Req_BatInfo(void)
{
    uint32_t TxMailbox;
    uint8_t tx_message;
    TxHeader.DLC = 8;
    TxHeader.ExtId = 0x3EA;
    TxHeader.IDE = CAN_ID_EXT;
    TxHeader.RTR = CAN_RTR_REMOTE;

    if (HAL_CAN_AddTxMessage(&hcan1_t, &TxHeader, &tx_message, &TxMailbox) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    return RE_OK;
}

/* Look for missing battery info */
RE_StatusTypeDef RE_GetBatInfoErr(void)
{
    if (bat1 == 0)
    {
        LogData.bat[0] = nodata_t.bat[0];
        bat_err_cnt++;
    }
    if (bat2 == 0)
    {
        LogData.bat[1] = nodata_t.bat[0];
        bat_err_cnt++;
    }
    if (bat3 == 0)
    {
        LogData.bat[2] = nodata_t.bat[0];
        bat_err_cnt++;
    }
    if (bat4 == 0)
    {
        LogData.bat[3] = nodata_t.bat[0];
        bat_err_cnt++;
    }
      if (bat_err_cnt != 0)
        {
            bat_error = bat_err_cnt;
}
        else
            {
              bat_error = 0;
            }
    /* Reset battery error count */
    if (bat_err_cnt != 0)
    {
        bat_err_cnt = 0;
    }
    /* Reset battery count */
    bat1 = bat2 = bat3 = bat4 = 0;
    return RE_OK;
}

// RE_StatusTypeDef RE_BatData
/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/