/**
  *****************************************************************************
  * Title                 :   Conversion  Kit
  * Filename              :   re_app_int_rtc.c
  * Origin Date           :   12/02/2020
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
  * Written by Team RACEnergy, Month Year
  *****************************************************************************
  */

/* Includes */
#include "int_rtc/re_app_int_rtc.h"

RTC_HandleTypeDef hRTC;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    if (hrtc->Instance == RTC)
    {
        __HAL_RCC_RTC_ENABLE();
    }
}

RE_StatusTypeDef RE_RTC_Init(void)
{
    hRTC.Instance = RTC;
    hRTC.Init.HourFormat = RTC_HOURFORMAT_24;
    hRTC.Init.AsynchPrediv = 127;
    hRTC.Init.SynchPrediv = 255;
    hRTC.Init.OutPut = RTC_OUTPUT_DISABLE;
    hRTC.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hRTC.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hRTC) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }

    return RE_OK;
}

RE_StatusTypeDef RE_SetTimeStamp(void)
{
    sTime.Hours = 20;
    sTime.Minutes = 0;
    sTime.Seconds = 0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hRTC, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }

    sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
    sDate.Month = RTC_MONTH_FEBRUARY;
    sDate.Date = 13;
    sDate.Year = 2020;
    if (HAL_RTC_SetDate(&hRTC, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        RE_Error_Handler(__FILE__, __LINE__);
    }

    return RE_OK;
}

RE_StatusTypeDef RE_GetTimeStamp(void)
{
    HAL_RTC_GetTime(&hRTC, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hRTC, &sDate, RTC_FORMAT_BIN);
    return RE_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/