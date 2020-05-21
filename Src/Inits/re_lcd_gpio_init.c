/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_lcd_gpio_init.c
  * Origin Date           :   10/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   FSTN LCD
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
#include "re_lcd_gpio_init.h"

GPIO_InitTypeDef cs1_pin, cs2_pin, clk_pin, data_pin;

/**
 * @brief LCD GPIO Initialization
 * This function configures the GPIO pins used by Dashboard LCD
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_LCD_GpioInit(void)
{
    __GPIOE_CLK_ENABLE(); /* GPIO Port E Clock Enable */
    /*
    * PE0 : cs1_pin
    * PE1 : cs2_pin
    * PE2 : clk_pin
    * PE3 : data_pin
    */
    cs1_pin.Pin = GPIO_PIN_0; /* Configure Port:E Pin:0 as output */
    cs1_pin.Mode = GPIO_MODE_OUTPUT_PP;
    cs1_pin.Pull = GPIO_PULLDOWN;
    cs1_pin.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOE, &cs1_pin);

    cs2_pin.Pin = GPIO_PIN_1; /* Configure Port:E Pin:1 as output */
    cs2_pin.Mode = GPIO_MODE_OUTPUT_PP;
    cs2_pin.Pull = GPIO_PULLDOWN;
    cs2_pin.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOE, &cs2_pin);

    clk_pin.Pin = GPIO_PIN_2; /* Configure Port:E Pin:2 as output */
    clk_pin.Mode = GPIO_MODE_OUTPUT_PP;
    clk_pin.Pull = GPIO_PULLDOWN;
    clk_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &clk_pin);

    data_pin.Pin = GPIO_PIN_3; /* Configure Port:E Pin:3 as output */
    data_pin.Mode = GPIO_MODE_OUTPUT_PP;
    data_pin.Pull = GPIO_PULLDOWN;
    data_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &data_pin);

    return RE_OK;
}
