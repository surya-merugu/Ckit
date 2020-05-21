/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_driver_dash_lcd.c
  * Origin Date           :   11/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   FSTN LCD
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
#include "dash_lcd/re_driver_dash_lcd.h"

#define CMDOFF 0x00
#define CMDON 0x01
#define CMDLCDOFF 0x02
#define CMDLCDON 0x03
#define CMDB3C4 0x29

extern GPIO_InitTypeDef cs1_pin, cs2_pin, clk_pin, data_pin;

static RE_StatusTypeDef RE_lcd_write_bit(bool n);
static RE_StatusTypeDef RE_lcd_write_cmdcode(uint8_t cmdcode);

/**
 * @brief RE_lcd_write_bit
 * This function bit bang the the lcd data to be sent onto GPIO
 * @param n bit: [1] or [0]
 * @retval Exit status\
 */
static RE_StatusTypeDef RE_lcd_write_bit(bool n)
{
    HAL_GPIO_WritePin(GPIOE, data_pin.Pin, (GPIO_PinState)n);
    __NOP();
    __NOP();
    HAL_GPIO_WritePin(GPIOE, clk_pin.Pin, GPIO_PIN_SET); /* Write bit '1' to GPIO */
    __NOP();
    __NOP();
    HAL_GPIO_WritePin(GPIOE, clk_pin.Pin, GPIO_PIN_RESET); /* Write bit '0' to GPIO */
    __NOP();
    __NOP();
    HAL_GPIO_WritePin(GPIOE, clk_pin.Pin, GPIO_PIN_SET); /* Write bit '1' to GPIO */

    return RE_OK;
}

/**
 * @brief RE_lcd_write_data
 * Send 6-bit address + 8-bit data to <address>
 * @param address Lcd memory address
 * @param data Data to be written onto LCD memory
 * @param cs_pin Chip select pin for lcd memory
 * @retval Exit status
 */
RE_StatusTypeDef RE_lcd_write_data(uint8_t address, uint64_t data, uint16_t cs_pin)
{
    uint8_t i = 0;
    HAL_GPIO_WritePin(GPIOE, cs_pin, GPIO_PIN_SET);   /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs_pin, GPIO_PIN_RESET); /* Write bit '1' to GPIO */
    RE_lcd_write_bit(1);
    RE_lcd_write_bit(0);
    RE_lcd_write_bit(1);
    address <<= 2;
    for (i = 0; i < 6; i++)
    {
        if ((address & 0x80) == 0x80)
            RE_lcd_write_bit(1);
        else
            RE_lcd_write_bit(0);
        address <<= 1;
    }
    for (i = 0; i < 64; i++)
    {
        if ((data & 0x01) == 0x01)
            RE_lcd_write_bit(1);
        else
            RE_lcd_write_bit(0);
        data >>= 1;
    }
    HAL_GPIO_WritePin(GPIOE, data_pin.Pin, GPIO_PIN_SET); /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs_pin, GPIO_PIN_SET);       /* Write bit '1' to GPIO */
    return RE_OK;
}

/**
 * @brief RE_lcd_write_data
 * Send control word
 * @param cmdcode Read, Write, Reset command code
 * @retval Exit status
 */
static RE_StatusTypeDef RE_lcd_write_cmdcode(uint8_t cmdcode)
{
    uint8_t i = 0;
    HAL_GPIO_WritePin(GPIOE, cs1_pin.Pin, GPIO_PIN_SET);   /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs2_pin.Pin, GPIO_PIN_SET);   /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs1_pin.Pin, GPIO_PIN_RESET); /* Write bit '0' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs2_pin.Pin, GPIO_PIN_RESET); /* Write bit '0' to GPIO */
    RE_lcd_write_bit(1);
    RE_lcd_write_bit(0);
    RE_lcd_write_bit(0);
    for (i = 0; i < 8; i++)
    {
        if ((cmdcode & 0x80) == 0x80)
            RE_lcd_write_bit(1);
        else
            RE_lcd_write_bit(0);
        cmdcode <<= 1;
    }
    RE_lcd_write_bit(0);
    HAL_GPIO_WritePin(GPIOE, data_pin.Pin, GPIO_PIN_SET); /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs1_pin.Pin, GPIO_PIN_SET);  /* Write bit '1' to GPIO */
    HAL_GPIO_WritePin(GPIOE, cs2_pin.Pin, GPIO_PIN_SET);  /* Write bit '1' to GPIO */
    return RE_OK;
}

/**
 * @brief RE_lcd_reset
 * Reset and initialize
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_lcd_reset(void)
{
    RE_lcd_write_cmdcode(CMDOFF);    //Power down
    RE_lcd_write_cmdcode(CMDLCDOFF); //Close off
    RE_lcd_write_cmdcode(CMDON);     //Power on
    RE_lcd_write_cmdcode(CMDLCDON);  //Display
    RE_lcd_write_cmdcode(CMDB3C4);   //Mode setting
    return RE_OK;
}
