/**
  *****************************************************************************
  * Title                 :   Conversion Kit
  * Filename              :   re_can_init.c
  * Origin Date           :   13/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   TJA1050
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
#include "re_can_init.h"

CAN_HandleTypeDef hcan1_t;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

static RE_StatusTypeDef RE_CAN_GpioInit(CAN_HandleTypeDef *hcan1)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hcan1->Instance == CAN1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**CAN1 GPIO Configuration
         * PD0     ------> CAN1_RX
         * PD1     ------> CAN1_TX
         */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* CAN1 interrupt Init */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
        HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    }
    return RE_OK;
}

static RE_StatusTypeDef RE_CAN_GpioDeInit(CAN_HandleTypeDef *hcan1)
{
    if (hcan1->Instance == CAN1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN1 GPIO Configuration
         * PD0     ------> CAN1_RX
         * PD1     ------> CAN1_TX
         */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1);

        /* CAN1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
    }
    return RE_OK;
}

RE_StatusTypeDef RE_CAN1_Init(void)
{
    hcan1_t.Instance = CAN1;
    RE_CAN_GpioInit(&hcan1_t);

    hcan1_t.Init.AutoBusOff = DISABLE;
    hcan1_t.Init.AutoRetransmission = DISABLE;
    hcan1_t.Init.AutoWakeUp = DISABLE;
    hcan1_t.Init.Mode = CAN_MODE_NORMAL;
    hcan1_t.Init.ReceiveFifoLocked = DISABLE;
    /** For APB1 clock speed : 42 Mhz
   * Prescaler : 6
   * Time_Seg1 : 11
   * Time_Seg2 : 2
   * Sync_Jump : 1
   * Sample_point : 85.7%
   */
    hcan1_t.Init.Prescaler = 6;
    hcan1_t.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1_t.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan1_t.Init.TimeSeg2 = CAN_BS2_2TQ;

    hcan1_t.Init.TimeTriggeredMode = DISABLE;
    hcan1_t.Init.TransmitFifoPriority = DISABLE;
    if (HAL_CAN_Init(&hcan1_t) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    return RE_OK;
}

/**
  * @brief RE_CAN1_Filter_Config
  *  This function configures CAN1 filter banks
  * @param None
  * @retval Exit Status
  */
RE_StatusTypeDef RE_CAN1_Filter_Config(void)
{
    CAN_FilterTypeDef CAN1_Filter_t;

    /* Accept all messages */
    CAN1_Filter_t.FilterActivation = ENABLE; /* Enable (or)Disable Filtering */
    CAN1_Filter_t.FilterBank = 0;            /* FilterBank:0 */
    CAN1_Filter_t.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN1_Filter_t.FilterIdHigh = 0x0000;
    CAN1_Filter_t.FilterIdLow = 0x0000;
    CAN1_Filter_t.FilterMaskIdHigh = 0x0000;
    CAN1_Filter_t.FilterMaskIdLow = 0x0000;
    CAN1_Filter_t.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN1_Filter_t.FilterScale = CAN_FILTERSCALE_32BIT;
    if (HAL_CAN_ConfigFilter(&hcan1_t, &CAN1_Filter_t) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }

/* Configure filter banks */
#if 0
    CAN1_Filter_t.FilterActivation = DISABLE; /* Enable (or)Disable Filtering */
    CAN1_Filter_t.FilterBank = 0;             /* FilterBank:0 */
    CAN1_Filter_t.FilterFIFOAssignment = CAN_RX_FIFO0;
    CAN1_Filter_t.FilterIdHigh = 0x0000;
    CAN1_Filter_t.FilterIdLow = 0x0000;
    CAN1_Filter_t.FilterMaskIdHigh = 0xFFFF;
    CAN1_Filter_t.FilterMaskIdLow = 0x8000;
    CAN1_Filter_t.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN1_Filter_t.FilterScale = CAN_FILTERSCALE_32BIT;
    if (HAL_CAN_ConfigFilter(&hcan1_t, &CAN1_Filter_t) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }

    CAN1_Filter_t.FilterActivation = DISABLE;
    CAN1_Filter_t.FilterBank = 1;
    CAN1_Filter_t.FilterFIFOAssignment = CAN_RX_FIFO1;
    CAN1_Filter_t.FilterIdHigh = 0x0000;
    CAN1_Filter_t.FilterIdLow = 0x8000;
    CAN1_Filter_t.FilterMaskIdHigh = 0xFFFF;
    CAN1_Filter_t.FilterMaskIdLow = 0xFFF8;
    CAN1_Filter_t.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN1_Filter_t.FilterScale = CAN_FILTERSCALE_32BIT;
    if (HAL_CAN_ConfigFilter(&hcan1_t, &CAN1_Filter_t) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
#endif

    return RE_OK;
}

RE_StatusTypeDef RE_CAN1_Start_Interrupt(void)
{
    if (HAL_CAN_ActivateNotification(&hcan1_t, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING |
                                                   CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
    /* CAN enter normal operation mode*/
    if (HAL_CAN_Start(&hcan1_t) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }
}
/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/