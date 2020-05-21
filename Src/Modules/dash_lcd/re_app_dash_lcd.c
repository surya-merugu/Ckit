/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_dash_lcd.c
  * Origin Date           :   10/02/2020
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
#include "dash_lcd/re_app_dash_lcd.h"
#include "stm32f4xx_hal_gpio.h"

extern LogData_t LogData;
//extern uint16_t err;
extern uint16_t bat_error;
extern GPIO_InitTypeDef cs1_pin, cs2_pin, clk_pin, data_pin;
uint8_t range = 0;

FSTNLCD_data_t LcdDisplay;
const uint8_t lcd_mem_addr[2] = {0, 16};

CS_seg_t CS[2];

uint8_t n[25];
uint8_t z[56];

static uint8_t RE_num_to_7seg(uint8_t number);

static RE_StatusTypeDef RE_split_two_digit(uint8_t num, uint8_t addr);
static RE_StatusTypeDef RE_split_three_digit(uint16_t num, uint8_t addr);
static RE_StatusTypeDef RE_split_six_digit(uint32_t num, uint8_t addr);

static RE_StatusTypeDef RE_speed_to_graphic(uint8_t seg);
static RE_StatusTypeDef RE_efficiency_to_graphic(uint8_t seg);
static RE_StatusTypeDef RE_soc_to_graphic(uint8_t seg);


static RE_StatusTypeDef RE_update_err_code(void);
static RE_StatusTypeDef RE_update_speed(void);
static RE_StatusTypeDef RE_update_odo(void);
static RE_StatusTypeDef RE_update_speed_graphic(void);
static RE_StatusTypeDef RE_update_gear_mode(void);
static RE_StatusTypeDef RE_update_gear_dir(void);
static RE_StatusTypeDef RE_update_motor_temp(void);
static RE_StatusTypeDef RE_update_motor_fault(void);
static RE_StatusTypeDef RE_update_dock_latch_state(void);
static RE_StatusTypeDef RE_update_bat_temp(void);
static RE_StatusTypeDef RE_update_soc(void);
static RE_StatusTypeDef RE_update_soc_graphic(void);
static RE_StatusTypeDef RE_update_voltage(void);
static RE_StatusTypeDef RE_update_bat_fault(void);
static RE_StatusTypeDef RE_update_eff_in_graphic(void);
static RE_StatusTypeDef RE_update_range(void);
static RE_StatusTypeDef RE_write_to_display(void);
static RE_StatusTypeDef RE_load_lcd_data(void);

static uint16_t RE_AvgVolt(void);
static uint16_t RE_AvgTemp(void);
static uint8_t RE_AvgSoc(void);
static uint16_t RE_LowestVolt(void);
static uint16_t RE_HighestTemp(void);
static uint8_t RE_LowestSoc(void);

static uint8_t RE_calculate_eff(void);

/**
 * @brief RE_num_to_7seg
 * Convert the number to 7 segment
 * @param number to be converted into 7 segment
 * @retval Equivalent HEX
 */
static uint8_t RE_num_to_7seg(uint8_t number)
{
    switch (number)
    {
    case 0:
    {
        return ZERO;
        break;
    }
    case 1:
    {
        return ONE;
        break;
    }
    case 2:
    {
        return TWO;
        break;
    }
    case 3:
    {
        return THREE;
        break;
    }
    case 4:
    {
        return FOUR;
        break;
    }
    case 5:
    {
        return FIVE;
        break;
    }
    case 6:
    {
        return SIX;
        break;
    }
    case 7:
    {
        return SEVEN;
        break;
    }
    case 8:
    {
        return EIGHT;
        break;
    }
    case 9:
    {
        return NINE;
        break;
    }
    default:
    {
        return 0;
        break;
    }
    }
}

/**
 * @brief RE_split_two_digit
 * Split a two digit number into individual digits
 * @param num to be split into digits and convert to hex
 * @param addr address to be written onto LCD memory
 * @retval Exit status
 */
static RE_StatusTypeDef RE_split_two_digit(uint8_t num, uint8_t addr)
{
    uint8_t factor = 100;
    uint8_t arr[2] = {0, 0};
    for (uint8_t i = 0; i < 2; i++)
    {
        factor = factor / 10;
        arr[i] = num / factor;
        num = num % factor;
        /* Convert the digit into 7 segment */
        arr[i] = RE_num_to_7seg(arr[i]);
        /* Assign the equivalent 7 segment to LCD memory address */
        n[addr + i] = arr[i];
    }
    return RE_OK;
}

/**
 * @brief RE_split_three_digit
 * Split a three digit number into individual digits
 * @param num to be split into digits and convert to hex
 * @param addr address to be written onto LCD memory
 * @retval Exit status
 */
static RE_StatusTypeDef RE_split_three_digit(uint16_t num, uint8_t addr)
{
    uint16_t factor = 1000;
    uint8_t arr[3] = {0, 0, 0};
    for (uint8_t i = 0; i < 3; i++)
    {
        factor = factor / 10;
        arr[i] = num / factor;
        num = num % factor;
        /* Convert the digit into 7 segment */
        arr[i] = RE_num_to_7seg(arr[i]);
        /* Assign the equivalent 7 segment to LCD memory address */
        n[addr + i] = arr[i];
    }
    return RE_OK;
}

/**
 * @brief RE_split_six_digit
 * Split a six digit number into individual digits
 * @param num to be split into digits and convert to hex
 * @param addr address to be written onto LCD memory
 * @retval Exit status
 */
static RE_StatusTypeDef RE_split_six_digit(uint32_t num, uint8_t addr)
{
    uint32_t factor = 1000000;
    uint8_t arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (uint8_t i = 0; i < 6; i++)
    {
        factor = factor / 10;
        arr[i] = num / factor;
        num = num % factor;
        /* Convert the digit into 7 segment */
        arr[i] = RE_num_to_7seg(arr[i]);
        /* Assign the equivalent 7 segment to LCD memory address */
        n[addr + i] = arr[i];
    }
    return RE_OK;
}

/**
 * @brief RE_speed_to_graphic
 * Set speed on graphic meter
 * @param seg segment to be displayed
 * @retval Exit status
 */
static RE_StatusTypeDef RE_speed_to_graphic(uint8_t seg)
{
    switch (seg)
    {
    case 0:
        z[54] = 0x10;
        for (uint8_t i = 0; i < 9; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 1:
        z[53] = 0x30;
        for (uint8_t i = 0; i < 8; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 2:
        z[52] = 0x70;
        for (uint8_t i = 0; i < 7; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 3:
        z[51] = 0xF0;
        for (uint8_t i = 0; i < 6; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 4:
        z[51] = 0XF0;
        z[50] = 0x08;
        for (uint8_t i = 0; i < 5; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 5:
        z[51] = 0XF0;
        z[49] = 0x0C;
        for (uint8_t i = 0; i < 4; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 6:
        z[51] = 0XF0;
        z[48] = 0x0E;
        for (uint8_t i = 0; i < 3; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 7:
        z[51] = 0XF0;
        z[47] = 0x0F;
        for (uint8_t i = 0; i < 2; i++)
        {
            z[45 + i] = 0;
        }
        break;
    case 8:
        z[51] = 0XF0;
        z[46] = 0x2F;
        z[45] = 0;
        break;
    case 9:
        z[51] = 0XF0;
        z[45] = 0x6F;
        break;
    }
    return RE_OK;
}

/**
 * @brief RE_speed_to_graphic
 * Set efficiency on graphic meter
 * @param seg segment to be displayed
 * @retval Exit status
 */
static RE_StatusTypeDef RE_efficiency_to_graphic(uint8_t seg)
{
    switch (seg)
    {
    case 0:
        z[24] = 0x80;
        for (uint8_t i = 1; i < 8; i++)
        {
            z[24 + i] = 0;
        }
        break;
    case 1:
        z[25] = 0xC0;
        for (uint8_t i = 2; i < 8; i++)
        {
            z[24 + i] = 0;
        }
        break;
    case 2:
        z[26] = 0xE0;
        for (uint8_t i = 3; i < 8; i++)
        {
            z[24 + i] = 0;
        }
        break;
    case 3:
        z[27] = 0xF0;
        for (uint8_t i = 4; i < 8; i++)
        {
            z[24 + i] = 0;
        }
        break;
    case 4:
        z[27] = 0XF0;
        z[28] = 0x01;
        z[29] = z[30] = z[31] = 0;
        break;
    case 5:
        z[27] = 0XF0;
        z[28] = z[29] = 0x01;
        z[30] = z[31] = 0;
        break;
    case 6:
        z[27] = 0XF0;
        z[28] = z[29] = z[30] = 0x01;
        z[31] = 0;
        break;
    case 7:
        z[27] = 0XF0;
        z[28] = z[29] = z[30] = z[31] = 0x01;
        break;
    }
    return RE_OK;
}

/**
 * @brief Re_soc_to_graphic
 * Set SoC on graphic meter
 * @param seg segment to be displayed
 * @retval Exit status
 */
static RE_StatusTypeDef RE_soc_to_graphic(uint8_t seg)
{
    switch (seg)
    {
    case 0:
        /* TODO: Blink first segment and also alert symbol */
        break;
    case 1:
        z[7] = 0x01;
        z[8] = z[9] = z[10] = 0;
        for (uint8_t i = 1; i < 8; i++)
        {
            z[7 + i] = 0;
        }
        break;
    case 2:
        z[8] = 0x03;
        z[7] = z[9] = z[10] = 0;
        for (uint8_t i = 2; i < 8; i++)
        {
            z[7 + i] = 0;
        }
        break;
    case 3:
        z[9] = 0x07;
        z[7] = z[8] = z[10] = 0;
        for (uint8_t i = 3; i < 8; i++)
        {
            z[7 + i] = 0;
        }
        break;
    case 4:
        z[10] = 0x0F;
        for (uint8_t i = 4; i < 8; i++)
        {
            z[7 + i] = 0;
        }
        break;
    case 5:
        z[7] = 0X0F;
        z[11] = 0x80;
        z[14] = z[12] = z[13] = 0;
        break;
    case 6:
        z[7] = 0X0F;
        z[11] = z[12] = 0xC0;
        z[13] = z[14] = 0;
        break;
    case 7:
        z[7] = 0X0F;
        z[11] = z[13] = z[12] = 0xE0;
        z[14] = 0;
        break;
    case 8:
        z[7] = 0X0F;
        z[14] = z[13] = z[12] = z[11] = 0XF0;
        break;
    }
    return RE_OK;
}

RE_StatusTypeDef RE_update_lcd_static_seg(void)
{
    /* reset the screen */
    RE_lcd_reset();
    /* Initialise all NUMBERS to zero */
    for (uint8_t i = 0; i < 56; i++)
    {
        n[i] = ZERO;
    }

    for (uint8_t i = 0; i <= 7; i++)
    {
        z[i + 7] = 0;
        z[i + 24] = 0;
    }
    for (uint8_t i = 0; i < 10; i++)
    {
        z[i + 45] = 0;
    }
    /* Error,Lock symbol */
    z[2] = 0x04;
    z[3] = 0x00;

    /* Speed */
    z[44] = 0x10;
    z[43] = 0x01;
    /* Gear */
    z[55] = 0x01;
    /* Motor */
    z[39] = z[38] = 0x01;
    z[36] = 0x10;
    /* Battery */
    z[5] = z[4] = 0x01;
    z[0] = z[18] = z[17] = 0x01;
    z[15] = 0x08;
    z[16] = 0;
    /* Range */
    z[21] = z[19] = 1;
    /* Odo */
    z[23] = z[22] = 0x01;
    z[32] = 0x08;
    z[44] = 0x10;
    z[21] = z[43] = z[19] = 0x01;
    z[1] = 0x08;
    z[34] = 0x04;
    z[33] = 0x02;
    return RE_OK;
}

/**
 * @brief RE_set_err_code
 * Display error code on the screen
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_err_code(void)
{

    RE_GetBatInfoErr();
    if (0x0U != bat_error)
    {
        RE_split_three_digit(bat_error, 1);
        z[2] = 0x04;

        if (bat_error < 4)
        {
            LcdDisplay.bat_soc = RE_AvgSoc();
            LcdDisplay.bat_temp = RE_AvgTemp() / 100;
            LcdDisplay.bat_volt = RE_AvgVolt();
        }
        else
        {
            LcdDisplay.bat_soc = 0;
            LcdDisplay.bat_temp = 0;
            LcdDisplay.bat_volt = 0;
        }
    }
    else
    {
        z[2] = 0;

        LcdDisplay.bat_soc = RE_LowestSoc();
        LcdDisplay.bat_temp = RE_HighestTemp() / 100;
        LcdDisplay.bat_volt = RE_LowestVolt();
    }

    return RE_OK;
}

/**
 * @brief RE_update_speed
 * update speed
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_speed(void)
{
    RE_split_two_digit(LcdDisplay.speed, 4);
    return RE_OK;
}

/**
 * @brief RE_update_odo
 * update ODO
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_odo(void)
{
//    LcdDisplay.odo = ReadLogFile("ODO.TXT", 10);
    RE_split_six_digit(ReadLogFile("ODO.TXT", 10), 15);
    return RE_OK;
}

/**
 * @brief RE_speed_in_graphics
 * update speed graphics
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_speed_graphic(void)
{
    uint8_t kmph = 0;
    kmph = LcdDisplay.speed / 5;
    for (uint8_t i = 0; i < 10; i++)
    {
        z[45 + i] = 0x0;
    }
    if (LcdDisplay.speed > 0)
    {
        RE_speed_to_graphic(kmph);
    }
    return RE_OK;
}

/**
 * @brief RE_update_gear_mode
 * update gear mode
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_gear_mode(void)
{
    switch (LcdDisplay.gear)
    {
    case 1:
        n[23] = LOW;
        break;
    case 2:
        n[23] = AUTO;
        break;
    case 3:
        n[23] = HIGH;
        break;
    }
    return RE_OK;
}

/**
 * @brief RE_update_gear_dir
 * update gear diirection
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_gear_dir(void)
{
    switch (LcdDisplay.gear_direction)
    {
    case 1:
        /* Neutral */
        z[41] = 0x5;
        z[40] = z[42] = 0;
        break;
    case 2:
        /* Drive */
        z[42] = 0x3;
        z[40] = z[41] = 0;
        break;
    case 3:
        /* Reverse */
        z[40] = 0x9;
        z[41] = z[42] = 0;
        break;
    default:
        break;
    }
    return RE_OK;
}

/**
 * @brief RE_update_motor_temp
 * update motor temperature
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_motor_temp(void)
{
    RE_split_two_digit(LcdDisplay.motor_temp, 21);
    return RE_OK;
}

/**
 * @brief RE_update_motor_fault
 * update motor fault
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_motor_fault(void)
{
    /**
     * @todo Add logic
     */
    return RE_OK;
}

/**
 * @brief RE_update_dock_latch_state
 * update dock latch status
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_dock_latch_state(void)
{
    z[3] = LcdDisplay.lock_status;
    return RE_OK;
}

/**
 * @brief RE_update_bat_temp
 * update battery temperature
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_bat_temp(void)
{
    //  LcdDisplay.bat_temp = LogData.bat[2].temp / 100;
    RE_split_two_digit(LcdDisplay.bat_temp, 6);
    return RE_OK;
}

/**
 * @brief RE_update_soc
 * update battery SoC
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_soc(void)
{
    //  LcdDisplay.bat_soc = LogData.bat[2].soc;
    RE_split_two_digit(LcdDisplay.bat_soc, 11);
    return RE_OK;
}

/**
 * @brief RE_update_soc_graphic
 * update battery SoC in graphic
 * @param None
 * @retval Exit status
 */
static RE_StatusTypeDef RE_update_soc_graphic(void)
{
    uint8_t soc = 0;
    soc = LcdDisplay.bat_soc / 12;
    for (uint8_t i = 0; i < 8; i++)
    {
        z[7 + i] = 0x0;
    }
    RE_soc_to_graphic(soc);
    return RE_OK;
}

static RE_StatusTypeDef RE_update_voltage(void)
{
    RE_split_three_digit((LcdDisplay.bat_volt) / 100, 8);
    return RE_OK;
}

static RE_StatusTypeDef RE_update_bat_fault(void)
{
    if (bat_error == 0)
    {
        z[6] = 0;
    }
    else
    {
        z[6] = 0x04;
    }
    return RE_OK;
}

static RE_StatusTypeDef RE_update_eff_in_graphic(void)
{
    uint8_t seg = 0;
    LcdDisplay.efficiency = RE_calculate_eff();
    if (LcdDisplay.efficiency > 0 && LcdDisplay.efficiency <= 5)
    {
        seg = 0;
    }
    else if (LcdDisplay.efficiency > 5 && LcdDisplay.efficiency <= 10)
    {
        seg = 1;
    }
    else if (LcdDisplay.efficiency > 10 && LcdDisplay.efficiency <= 15)
    {
        seg = 2;
    }
    else if (LcdDisplay.efficiency > 15 && LcdDisplay.efficiency <= 20)
    {
        seg = 3;
    }
    else if (LcdDisplay.efficiency > 20 && LcdDisplay.efficiency <= 25)
    {
        seg = 4;
    }
    else if (LcdDisplay.efficiency > 25 && LcdDisplay.efficiency <= 30)
    {
        seg = 5;
    }
    else if (LcdDisplay.efficiency > 30 && LcdDisplay.efficiency <= 38)
    {
        seg = 6;
    }
    else if (LcdDisplay.efficiency > 38)
    {
        seg = 7;
    }

    RE_efficiency_to_graphic(seg);
    return RE_OK;
}

static RE_StatusTypeDef RE_update_range(void)
{
    range = (LcdDisplay.bat_soc * 3) / 4;
    RE_split_two_digit(range, 13);
    return RE_OK;
}

static RE_StatusTypeDef RE_write_to_display(void)
{
    RE_load_lcd_data();
    /* CS1 */
    RE_lcd_write_data(lcd_mem_addr[0], CS[0].seg[0], cs1_pin.Pin);
    RE_lcd_write_data(lcd_mem_addr[1], CS[0].seg[1], cs1_pin.Pin);
    /* CS2 */
    RE_lcd_write_data(lcd_mem_addr[0], CS[1].seg[0], cs2_pin.Pin);
    RE_lcd_write_data(lcd_mem_addr[1], CS[1].seg[1], cs2_pin.Pin);

    return RE_OK;
}

static RE_StatusTypeDef RE_load_lcd_data(void)
{

    CS[0].seg[0] = ((~(~(n[22] | z[39]) & 0xFFFFFFFFFFFFFFFF)) |
                    (~(~(n[21] | z[38]) & 0xFFFFFFFFFFFFFFFF) << 8) |
                    (~(~(n[14] | z[21]) & 0xFFFFFFFFFFFFFFFF) << 16) |
                    (~(~(n[13] | z[20]) & 0xFFFFFFFFFFFFFFFF) << 24) |
                    (~(~(n[23]) & 0xFFFFFFFFFFFFFFFF) << 32) |
                    (~(~(n[5] | z[43]) & 0xFFFFFFFFFFFFFFFF) << 40) |
                    (~(~(n[4]) & 0xFFFFFFFFFFFFFFFF) << 48) |
                    (~(~(n[3]) & 0xFFFFFFFFFFFFFFFF) << 56));

    CS[0].seg[1] = ((~(~(n[2]) & 0xFFFFFFFFFFFFFFFF)) |
                    (~(~(n[1]) & 0xFFFFFFFFFFFFFFFF) << 8) |
                    (~(~(z[51] | z[52] | z[53] | z[54] | z[1] | z[2] | z[3] | z[19]) & 0xFFFFFFFFFFFFFFFF) << 16) |
                    (~(~(z[45] | z[46] | z[44] | z[50] | z[49] | z[48] | z[47]) & 0xFFFFFFFFFFFFFFFF) << 24) |
                    (~(~(z[35] | z[37] | z[36] | z[42] | z[41] | z[40] | z[55]) & 0xFFFFFFFFFFFFFFFF) << 32));

    CS[1].seg[0] = ((~(~(n[20] | z[23]) & 0xFFFFFFFFFFFFFFFF)) |
                    (~(~(n[19] | z[31]) & 0xFFFFFFFFFFFFFFFF) << 8) |
                    (~(~(n[18] | z[30]) & 0xFFFFFFFFFFFFFFFF) << 16) |
                    (~(~(n[17] | z[29]) & 0xFFFFFFFFFFFFFFFF) << 24) |
                    (~(~(n[16] | z[28]) & 0xFFFFFFFFFFFFFFFF) << 32) |
                    (~(~(n[15] | z[22]) & 0xFFFFFFFFFFFFFFFF) << 40) |
                    (~(~(n[7] | z[5]) & 0xFFFFFFFFFFFFFFFF) << 48) |
                    (~(~(n[6] | z[4]) & 0xFFFFFFFFFFFFFFFF) << 56));

    CS[1].seg[1] = ((~(~(n[11] | z[16]) & 0xFFFFFFFFFFFFFFFF)) |
                    (~(~(n[12] | z[17]) & 0xFFFFFFFFFFFFFFFF) << 8) |
                    (~(~(n[8]) & 0xFFFFFFFFFFFFFFFF) << 16) |
                    (~(~(n[9] | z[0]) & 0xFFFFFFFFFFFFFFFF) << 24) |
                    (~(~(n[10] | z[18]) & 0xFFFFFFFFFFFFFFFF) << 32) |
                    (~(~(z[11] | z[12] | z[13] | z[14] | z[10] | z[9] | z[8] | z[7]) & 0xFFFFFFFFFFFFFFFF) << 40) |
                    (~(~(z[15] | z[6]) & 0xFFFFFFFFFFFFFFFF) << 48) |
                    (~(~(z[24] | z[25] | z[26] | z[27] | z[32] | z[34] | z[33]) & 0xFFFFFFFFFFFFFFFF) << 56));

    return RE_OK;
}

static uint16_t RE_AvgVolt(void)
{
    return (LogData.bat[0].volt + LogData.bat[1].volt + LogData.bat[2].volt + LogData.bat[3].volt) / (4 - bat_error);
}

static uint16_t RE_AvgTemp(void)
{
    return (LogData.bat[0].temp + LogData.bat[1].temp + LogData.bat[2].temp + LogData.bat[3].temp) / (4 - bat_error);
}

static uint8_t RE_AvgSoc(void)
{
    return (LogData.bat[0].soc + LogData.bat[1].soc + LogData.bat[2].soc + LogData.bat[3].soc) / (4 - bat_error);
}

static uint16_t RE_LowestVolt(void)
{
    if (LogData.bat[0].volt < LogData.bat[1].volt && LogData.bat[0].volt < LogData.bat[2].volt && LogData.bat[0].volt < LogData.bat[3].volt)
    {
        return LogData.bat[0].volt;
    }
   else if (LogData.bat[1].volt < LogData.bat[0].volt && LogData.bat[1].volt < LogData.bat[2].volt && LogData.bat[1].volt < LogData.bat[3].volt)
    {
        return LogData.bat[1].volt;
    }
    else if (LogData.bat[2].volt < LogData.bat[0].volt && LogData.bat[2].volt < LogData.bat[1].volt && LogData.bat[2].volt < LogData.bat[3].volt)
    {
        return LogData.bat[2].volt;
    }
    else
    {
        return LogData.bat[3].volt;
    }
}

static uint16_t RE_HighestTemp(void)
{
    if (LogData.bat[0].temp > LogData.bat[1].temp && LogData.bat[0].temp > LogData.bat[2].temp && LogData.bat[0].temp > LogData.bat[3].temp)
    {
        return LogData.bat[0].temp;
    }
    else if (LogData.bat[1].temp > LogData.bat[0].temp && LogData.bat[1].temp > LogData.bat[2].temp && LogData.bat[1].temp > LogData.bat[3].temp)
    {
        return LogData.bat[1].temp;
    }
    else if (LogData.bat[2].temp > LogData.bat[0].temp && LogData.bat[2].temp > LogData.bat[1].temp && LogData.bat[2].temp > LogData.bat[3].temp)
    {
        return LogData.bat[2].temp;
    }
    else
    {
        return LogData.bat[3].temp;
    }
}

static uint8_t RE_LowestSoc(void)
{
    if (LogData.bat[0].soc < LogData.bat[1].soc && LogData.bat[0].soc < LogData.bat[2].soc && LogData.bat[0].soc < LogData.bat[3].soc)
    {
        return LogData.bat[0].soc;
    }
    else if (LogData.bat[1].soc < LogData.bat[0].soc && LogData.bat[1].soc < LogData.bat[2].soc && LogData.bat[1].soc < LogData.bat[3].soc)
    {
        return LogData.bat[1].soc;
    }
    else if (LogData.bat[2].soc < LogData.bat[0].soc && LogData.bat[2].soc < LogData.bat[1].soc && LogData.bat[2].soc < LogData.bat[3].soc)
    {
        return LogData.bat[2].soc;
    }
    else if (LogData.bat[3].soc < LogData.bat[0].soc && LogData.bat[3].soc < LogData.bat[1].soc && LogData.bat[3].soc < LogData.bat[2].soc)
    {
        return LogData.bat[3].soc;
    }
    else
    {
        return (LogData.bat[0].soc + LogData.bat[1].soc + LogData.bat[2].soc + LogData.bat[3].soc) / 4;
    }
}

static uint8_t RE_calculate_eff(void)
{

    for (uint8_t i = 0; i < 4; i++)
    {
        if (LogData.bat[i].current > 0)
        {
            LcdDisplay.current_in_amp[i] = (65535 - LogData.bat[i].current) / 1000;
        }
        else
        {
            LcdDisplay.current_in_amp[i] = 0;
        }
    }
    /* 3800 mech rpm = 150 elec rpm */
    return (LcdDisplay.rpm_eff * 25.33) / (LcdDisplay.current_in_amp[0] + LcdDisplay.current_in_amp[1] + LcdDisplay.current_in_amp[2] + LcdDisplay.current_in_amp[3]);
}

RE_StatusTypeDef RE_lcd_start_animation(void)
{
    RE_update_lcd_static_seg();
    RE_write_to_display();
    /* Increment the segment */
    for (uint8_t i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            RE_speed_to_graphic(0);
        }
        else if (i == 1)
        {
            RE_speed_to_graphic(2);
        }
        else if (i == 2)
        {
            RE_speed_to_graphic(4);
        }
        else
        {
            RE_speed_to_graphic(i + 2);
        }
        RE_efficiency_to_graphic(i);
        RE_soc_to_graphic(i);
        RE_write_to_display();
        HAL_Delay(75);
    }
    /* Decrement the segment */
    HAL_Delay(150);
    for (uint8_t i = 7; i > 0; i--)
    {
        if (i == 1)
        {
            RE_speed_to_graphic(0);
        }
        else if (i == 2)
        {
            RE_speed_to_graphic(2);
        }
        else if (i == 3)
        {
            RE_speed_to_graphic(4);
        }
        else
        {
            RE_speed_to_graphic(i + 1);
        }
        RE_efficiency_to_graphic(i - 1);
        RE_soc_to_graphic(i - 1);
        RE_write_to_display();
        HAL_Delay(75);
    }
    return RE_OK;
}

/* Dispaly speed digits. N1 being the left most digit */
RE_StatusTypeDef RE_RefreshScreen(void)
{
    /* Error code and number */
    z[2] = 0x00;
    n[1] = n[2] = n[3] = 0x00;
    RE_update_err_code();
    RE_update_dock_latch_state();
    RE_update_speed();
    RE_update_speed_graphic();
    RE_update_odo();
    RE_update_gear_mode();
    RE_update_gear_dir();
    RE_update_motor_temp();
    RE_update_motor_fault();
    RE_update_bat_temp();
    RE_update_soc();
    RE_update_soc_graphic();
    RE_update_voltage();
    RE_update_bat_fault();
    RE_update_eff_in_graphic();
    RE_update_range();

    RE_write_to_display();
    return RE_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/