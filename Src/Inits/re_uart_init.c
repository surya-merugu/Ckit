/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_uart_init.c
  * Origin Date           :   16/03/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   None
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
#include "re_uart_init.h"

UART_HandleTypeDef huart2_t;
UART_HandleTypeDef huart3_t;

static RE_StatusTypeDef RE_UART_GpioInit(UART_HandleTypeDef *huart);
static RE_StatusTypeDef RE_UART3_GpioDeInit(UART_HandleTypeDef *huart);


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
static RE_StatusTypeDef RE_UART_GpioInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == USART2)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration    
           PA2     ------> USART2_TX
           PA3     ------> USART2_RX 
        */
        GPIO_InitStruct.Pin             = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode            = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull            = GPIO_PULLUP;
        GPIO_InitStruct.Speed           = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate       = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 interrupt Init */
//        HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
        HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    return RE_OK;
}


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance == USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
static RE_StatusTypeDef RE_UART_GpioDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration    
           PA2     ------> USART2_TX
           PA3     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
    return RE_OK;
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval Exit status
  */
RE_StatusTypeDef RE_USART2_UART_Init(void)
{
    huart2_t.Instance = USART2;
    huart2_t.Init.BaudRate = 9600;
    huart2_t.Init.WordLength = UART_WORDLENGTH_8B;
    huart2_t.Init.StopBits = UART_STOPBITS_1;
    huart2_t.Init.Parity = UART_PARITY_NONE;
    huart2_t.Init.Mode = UART_MODE_TX_RX;
    huart2_t.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2_t.Init.OverSampling = UART_OVERSAMPLING_16;
    if (RE_UART_GpioInit(&huart2_t) != RE_OK)
    {
      RE_Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_UART_Init(&huart2_t) != HAL_OK)
    {
      RE_Error_Handler(__FILE__, __LINE__);
    }
    return RE_OK;
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval Exit status
  */
 void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3_t.Instance = USART3;
  huart3_t.Init.BaudRate = 115200;
  huart3_t.Init.WordLength = UART_WORDLENGTH_8B;
  huart3_t.Init.StopBits = UART_STOPBITS_1;
  huart3_t.Init.Parity = UART_PARITY_NONE;
  huart3_t.Init.Mode = UART_MODE_TX_RX;
  huart3_t.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3_t.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3_t) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}
/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/
