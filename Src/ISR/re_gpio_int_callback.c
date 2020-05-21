/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_gpio_int_callback.c
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
#include "re_gpio_int_callback.h"

extern RE_StatusTypeDef RE_Speed_ISR (void);
extern RE_StatusTypeDef RE_Gear_sensor_ISR(uint16_t GPIO_Pin);
extern RE_StatusTypeDef RE_key_pos_ISR (void);
extern uint8_t key_status;
extern uint8_t animate;
/**
 * @brief GPIO External interrupt Callback
 * This function is being called by the HAL layer whenever GPIO interrupt occurs
 * @param  GPIO Pin Number
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_6)
    {
      /*Increment rpm count */
       RpmCnt++;
    }
    else if(GPIO_Pin == GPIO_PIN_4)
    {     
        if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) != key_status)
        {
          key_status = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
          if(key_status == 1)
          {
            animate = 1;
            /* Enable speed interrupt */
            HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
          }
          else
          {
            animate = 0;
            /* Disable speed interrupt */
            HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
          }
    }
      animate = 1;
    }
//    else 
//    {
//        __NOP();
//    }
}


/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/