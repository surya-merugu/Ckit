/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_spi_init.c
  * Origin Date           :   20/05/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   
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
  * Written by Team RACEnergy, MAY 2020
  *****************************************************************************
  */

/* Includes */
#include "re_spi_init.h"


SPI_HandleTypeDef hspi1_t;

/** 
 * @brief SPI GPIO Initialization
 * This function configures the GPIO pins used by I2C
 * @param hi2c1: I2C handle pointer
 * @retval Exit status
 */
static RE_StatusTypeDef RE_SPI_GpioInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef SPI_GPIO = {0};
    if(hspi->Instance == SPI1)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /** SPI1 GPIO Configuration
         * PB3     ------> SPI1_SCK
         * PB4     ------> SPI1_MISO
         * PB5     ------> SPI1_MOSI  
         * PD13    ------> CS/SS(active low)
         */
        SPI_GPIO.Pin              = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
        SPI_GPIO.Mode             = GPIO_MODE_OUTPUT_OD;
        SPI_GPIO.Pull             = GPIO_PULLUP;
        SPI_GPIO.Speed            = GPIO_SPEED_FREQ_VERY_HIGH;
        SPI_GPIO.Alternate        = GPIO_AF5_SPI1;;
        HAL_GPIO_Init(GPIOB, &SPI_GPIO);

        SPI_GPIO.Pin              = GPIO_PIN_13;
        SPI_GPIO.Mode             = GPIO_MODE_OUTPUT_PP;
        SPI_GPIO.Pull             = GPIO_PULLUP;
        SPI_GPIO.Speed            = GPIO_SPEED_FREQ_LOW;        
        HAL_GPIO_Init(GPIOD, &SPI_GPIO);
    }
    return RE_OK;
}

/**
 * @brief Initialize the spi peripheral
 * @param None
 * @retval Exit status
 */
RE_StatusTypeDef RE_SPI_Init(void)
{

      hspi1_t.Instance                   = SPI1;
      hspi1_t.Init.Mode                  = SPI_MODE_MASTER;
      hspi1_t.Init.Direction             = SPI_DIRECTION_2LINES;
      hspi1_t.Init.DataSize              = SPI_DATASIZE_16BIT;
      hspi1_t.Init.CLKPolarity           = SPI_POLARITY_LOW;
      hspi1_t.Init.CLKPhase              = SPI_PHASE_1EDGE;
      hspi1_t.Init.NSS                   = SPI_NSS_SOFT;
      hspi1_t.Init.BaudRatePrescaler     = SPI_BAUDRATEPRESCALER_16;
      hspi1_t.Init.FirstBit              = SPI_FIRSTBIT_MSB;
      hspi1_t.Init.TIMode                = SPI_TIMODE_DISABLE;
      hspi1_t.Init.CRCCalculation        = SPI_CRCCALCULATION_DISABLE;
      hspi1_t.Init.CRCPolynomial         = 10;
      if(RE_SPI_GpioInit(&hspi1_t) == RE_OK)
      {
          if (HAL_SPI_Init(&hspi1_t) != HAL_OK)
          {
            RE_Error_Handler(__FILE__, __LINE__);
          }
      }
    return RE_OK;
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/
