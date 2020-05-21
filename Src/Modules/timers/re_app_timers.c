/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_int_timer.c
  * Origin Date           :   15/02/2020
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
#include "timers/re_app_timers.h"

extern uint8_t GearMode_statusA, GearMode_statusB, GearDirection_statusA, GearDirection_statusB;
extern uint32_t RpmCnt;
extern uint8_t latch_status_flag;
uint8_t timer_counter = 0;
uint8_t timer_spi_counter = 0;
uint8_t SPI_Tx_Flag = 0;

extern uint8_t RE_Gear_Mode (uint8_t Gear_ModeA, uint8_t Gear_ModeB);
extern uint8_t RE_Gear_Direction (uint8_t Gear_directionA, uint8_t Gear_directionB);
extern uint32_t RE_speed(void);
extern uint8_t RE_DockLatch_Status(void);
extern RE_StatusTypeDef RE_CloseDock_Latch(void);
extern uint8_t RE_DockLatch_Status(void);
extern uint32_t RE_distance_covered(void);
extern RE_StatusTypeDef RE_Read_Gear_Status(void);

/* @brief RE Timer ISR
 * this function handles interrupts of timer
 * @param  None
 * @retval Exit Status
 */
RE_StatusTypeDef RE_Timer_ISR (void)
{
        timer_spi_counter;
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        RE_Read_Gear_Status();
        RE_Gear_Mode (GearMode_statusA, GearMode_statusB);
        RE_Gear_Direction (GearDirection_statusA, GearDirection_statusB);
        RE_speed ();
        RE_distance_covered ();
        RE_DockLatch_Status();
        if (latch_status_flag == 1)
        {
            timer_counter++ ;
        }
        if (timer_counter == 2)
        {
            RE_CloseDock_Latch();
            latch_status_flag = 0;
            timer_counter = 0;
        }
        if(timer_spi_counter == 42)
        {
            SPI_Tx_Flag = 1;
        }
        return RE_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/