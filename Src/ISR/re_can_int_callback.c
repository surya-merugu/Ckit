/**
  *****************************************************************************
  * Title                 :   Conversion Kit
  * Filename              :   re_can_int_callback.c
  * Origin Date           :   13/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   None
  * Target                :   STM32F407-DISC
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
#include "main/main.h"
#include "re_can_int_callback.h"
#include "battery_info/re_app_battery_info.h"

extern LogData_t LogData;
extern CAN_RxHeaderTypeDef RxHeader;
uint32_t Message_BatID;
uint8_t bat_num;
uint8_t bat1 = 0, bat2 = 0, bat3 = 0, bat4 = 0;

/**
  ==============================================================================
                          ##### Callback functions #####
  ==============================================================================
    [..]
    This subsection provides the following callback functions:
      ( ) HAL_CAN_TxMailbox0CompleteCallback
      ( ) HAL_CAN_TxMailbox1CompleteCallback
      ( ) HAL_CAN_TxMailbox2CompleteCallback
      ( ) HAL_CAN_TxMailbox0AbortCallback
      ( ) HAL_CAN_TxMailbox1AbortCallback
      ( ) HAL_CAN_TxMailbox2AbortCallback
      (+) HAL_CAN_RxFifo0MsgPendingCallback
      ( ) HAL_CAN_RxFifo0FullCallback
      ( ) HAL_CAN_RxFifo1MsgPendingCallback
      ( ) HAL_CAN_RxFifo1FullCallback
      ( ) HAL_CAN_SleepCallback
      ( ) HAL_CAN_WakeUpFromRxMsgCallback
      ( ) HAL_CAN_ErrorCallback
  ==============================================================================
*/

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rcvd_msg[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rcvd_msg) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    if (0x7CF <= RxHeader.ExtId)
    {
        Message_BatID = (RxHeader.ExtId) - (RxHeader.ExtId % 0x64);
        if (Message_BatID == 0X834)
        {
            bat1++;
            bat_num = 0;
        }
        else if (Message_BatID == 0X898)
        {
            bat2++;
            bat_num = 1;
        }
        else if (Message_BatID == 0X8FC)
        {
            bat3++;
            bat_num = 2;
        }
        else if (Message_BatID == 0X960)
        {
            bat4++;
            bat_num = 3;
        }
        switch (RxHeader.ExtId % 0x64)
        {
        case 0:
            /* @ToDo- emergency messsage */
        case 1:
            LogData.bat[bat_num].soc = rcvd_msg[0];
            LogData.bat[bat_num].volt = Byte_Swap(rcvd_msg[1], rcvd_msg[2]) * 2;
            LogData.bat[bat_num].temp = Byte_Swap(rcvd_msg[3], rcvd_msg[4]);
            LogData.bat[bat_num].current = Byte_Swap(rcvd_msg[5], rcvd_msg[6]) * 2;
            break;
        case 2:
            LogData.bat[bat_num].remain_cap = Byte_Swap(rcvd_msg[0], rcvd_msg[1]);
            LogData.bat[bat_num].avail_energy = Byte_Swap(rcvd_msg[2], rcvd_msg[3]);
            break;
        case 3:
            LogData.bat[bat_num].cycle_count = Byte_Swap(rcvd_msg[0], rcvd_msg[1]);
            LogData.bat[bat_num].soh = Byte_Swap(rcvd_msg[2], rcvd_msg[3]);
            LogData.bat[bat_num].full_cap = Byte_Swap(rcvd_msg[4], rcvd_msg[5]);
            break;
        default:
            /* @ToDo  - add default */
            break;
        }
    }
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/
