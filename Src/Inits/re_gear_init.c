/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_gear_init.c
  * Origin Date           :   15/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   Gear Position Sensor
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
#include "re_gear_init.h"

/**
 * @brief GEAR STATUS GPIO Initialization
 * This function configures the GPIO pins used by gear position sensor
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_Gear_GpioInit (void)
{
    GPIO_InitTypeDef GMode_pinA, GMode_pinB, GDirection_pinA, GDirection_pinB ;
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* GPIO Port C Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*
    * PA6 : Gear Mode PinA
    * PA7 : Gear Mode PinB
    * PC4 : Gear Direction PinA
    * PC5 : Gear Direction PinB
    */

     GMode_pinA.Pin            = GPIO_PIN_7;
     GMode_pinA.Mode           = GPIO_MODE_INPUT;
     GMode_pinA.Speed          = GPIO_SPEED_FAST;
     GMode_pinA.Pull           = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOA,&GMode_pinA);

     GMode_pinB.Pin            = GPIO_PIN_3;
     GMode_pinB.Mode           = GPIO_MODE_INPUT;
     GMode_pinB.Speed          = GPIO_SPEED_FAST;
     GMode_pinB.Pull           = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOC,&GMode_pinB);

     GDirection_pinA.Pin       = GPIO_PIN_4;
     GDirection_pinA.Mode      = GPIO_MODE_INPUT;
     GDirection_pinA.Speed     = GPIO_SPEED_FAST;
     GDirection_pinA.Pull      = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOC,&GDirection_pinA);

     GDirection_pinB.Pin       = GPIO_PIN_5;
     GDirection_pinB.Mode      = GPIO_MODE_INPUT;
     GDirection_pinB.Speed     = GPIO_SPEED_FAST;
     GDirection_pinB.Pull      = GPIO_PULLDOWN;
     HAL_GPIO_Init(GPIOC,&GDirection_pinB);
#if 0
     HAL_NVIC_SetPriority(EXTI0_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI0_IRQn);

     HAL_NVIC_SetPriority(EXTI1_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI1_IRQn);

     HAL_NVIC_SetPriority(EXTI2_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI2_IRQn);

     HAL_NVIC_SetPriority(EXTI3_IRQn,0,1);
     HAL_NVIC_EnableIRQ(EXTI3_IRQn);
#endif
     return RE_OK;
}

