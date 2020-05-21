/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_UART_Ring.c
  * Origin Date           :   25/02/2020
  * Compiler              :   Specify compiler used
  * Hardware              :   MC60
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

#include "gps_gprs/re_app_UART_Ring.h"
#include <string.h>

/* define the UART you are using  */

extern UART_HandleTypeDef huart2_t;

static int UART_Read(void);
static void UART_Write(int c);

ring_buffer rx_buffer = {{0}, 0, 0};
ring_buffer tx_buffer = {{0}, 0, 0};

ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;

void store_char(unsigned char c, ring_buffer *buffer);

void Ringbuf_Init(void)
{
    _rx_buffer = &rx_buffer;
    _tx_buffer = &tx_buffer;

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    __HAL_UART_ENABLE_IT(&huart2_t, UART_IT_ERR);

    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(&huart2_t, UART_IT_RXNE);
}

void Ringbuf_Free(void)
{
    memset(_rx_buffer->buffer, 0x00, UART_BUFFER_SIZE);
    _rx_buffer->head = NULL;
    _rx_buffer->tail = NULL;

    memset(_tx_buffer->buffer, 0x00, UART_BUFFER_SIZE);
    _tx_buffer->head = NULL;
    _tx_buffer->tail = NULL;
}

void Store_Char(unsigned char c, ring_buffer *buffer)
{
    uint32_t i = (uint32_t)(buffer->head + 1) % UART_BUFFER_SIZE;
    /** 
   * if we should be storing the received character into the location
   * just before the tail (meaning that the head would advance to the
   * current location of the tail), we're about to overflow the buffer
   * and so we don't write the character or advance the head.
   */
    if (i != buffer->tail)
    {
        buffer->buffer[buffer->head] = c;
	buffer->head = i;
    }
}

static int UART_Read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1;
    }
    else
    {
	unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
	_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
	return c;
    }
}

static void UART_Write(int c)
{
    if (c > 0)
    {
	uint32_t i = (uint32_t)(_tx_buffer->head + 1) % UART_BUFFER_SIZE;

	// If the output buffer is full, there's nothing for it other than to
	// wait for the interrupt handler to empty it a bit
	// ???: return 0 here instead?
	while (i == _tx_buffer->tail)
	{
          
	}

	_tx_buffer->buffer[_tx_buffer->head] = (uint8_t)c;
	_tx_buffer->head = i;

	__HAL_UART_ENABLE_IT(&huart2_t, UART_IT_TXE); // Enable UART transmission interrupt
    }
}

void UART_Sendstring(const char *s)
{
    while (*s)
    {
	UART_Write(*s++);
    }
}

//make this better
void Get_String(char *buffer)
{
    int index = 0;
    unsigned int start = _rx_buffer->tail;
    unsigned int end = (_rx_buffer->head);
    if ((_rx_buffer->tail > _rx_buffer->head) && (_rx_buffer->buffer[end - 1] == '\n'))
    {
        for (unsigned int i = start; i < UART_BUFFER_SIZE; i++)
	{
            buffer[index] = UART_Read();
            index++;
	}
	for (unsigned int i = 0; i < end; i++)
	{
            buffer[index] = UART_Read();
            index++;
	}
    }
    else if ((_rx_buffer->buffer[end - 1] == '\n'))
    {

	for (unsigned int i = start; i < end; i++)
	{
            buffer[index] = UART_Read();
            index++;
	}
    }
}

void UART_ISR(UART_HandleTypeDef *huart)
{
    uint32_t isrflags = READ_REG(huart->Instance->SR);
    uint32_t cr1its = READ_REG(huart->Instance->CR1);

    /* if DR is not empty and the Rx Int is enabled */
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
		/******************
    	    	      * @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
    	    	      *         error) and IDLE (Idle line detected) flags are cleared by software
    	    	      *         sequence: a read operation to USART_SR register followed by a read
    	    	      *         operation to USART_DR register.
    	    	      * @note   RXNE flag can be also cleared by a read to the USART_DR register.
    	    	      * @note   TC flag can be also cleared by software sequence: a read operation to
    	    	      *         USART_SR register followed by a write operation to USART_DR register.
    	    	      * @note   TXE flag is cleared only by a write to the USART_DR register.

    	 *********************/
	huart->Instance->SR;				   /* Read status register */
	unsigned char c = huart->Instance->DR; /* Read data register */
	Store_Char(c, _rx_buffer);			   // store data in buffer
	return;
    }

	/*If interrupt is caused due to Transmit Data Register Empty */
    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
        if (tx_buffer.head == tx_buffer.tail)
        {
            // Buffer empty, so disable interrupts
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
	}

	else
	{
            // There is more data in the output buffer. Send the next byte
            unsigned char c = tx_buffer.buffer[tx_buffer.tail];
            tx_buffer.tail = (tx_buffer.tail + 1) % UART_BUFFER_SIZE;

			/******************
    	      * @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
    	      *         error) and IDLE (Idle line detected) flags are cleared by software
    	      *         sequence: a read operation to USART_SR register followed by a read
    	      *         operation to USART_DR register.
    	      * @note   RXNE flag can be also cleared by a read to the USART_DR register.
    	      * @note   TC flag can be also cleared by software sequence: a read operation to
    	      *         USART_SR register followed by a write operation to USART_DR register.
    	      * @note   TXE flag is cleared only by a write to the USART_DR register.

    	      *********************/

            huart->Instance->SR;
            huart->Instance->DR = c;
	}
	return;
    }
}

/************************ (C) COPYRIGHT RACEnergy **********END OF FILE********/
