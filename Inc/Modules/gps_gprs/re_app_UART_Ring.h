/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_app_uart_ring.h
  * Origin Date           :   25/02/2020
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


#ifndef _RE_APP_UART_RING_H
#define _RE_APP_UART_RING_H

#include "re_uart_init.h"
#include "stm32f4xx_hal.h"

/* change the size of the buffer here */
#define UART_BUFFER_SIZE 200

typedef struct
{
  unsigned char buffer[UART_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;

/* Initialize the ring buffer */
void Ringbuf_Init(void);

/* Free buffer memory*/
void Ringbuf_Free(void);

/* reads the data in the rx_buffer and increment the tail count in rx_buffer */
static int UART_Read(void);

/* writes the data to the tx_buffer and increment the head count in tx_buffer */
static void UART_Write(int c);

/* function to send the string to the uart */
void UART_Sendstring(const char *s);

/* Print a number with any base
 * base can be 10, 8 etc*/
//void UART_Printbase(long n, uint8_t base);

/* checks if the data is available to read in the rx_buffer */
//uint16_t Is_Data_Available(void);

/* get the position of the given string within the incoming data.
 * It returns the position, where the string ends */
//int Get_Position(const char *string);

/*get the position of the given string's first encounter within the incoming data.
 * It returns the position, where the string ends */
//int Get_Position_First_Encounter(const char *string);

/* once you hit 'enter' (\r\n), it copies the entire string to the buffer*/
void Get_String(char *buffer);

/*After the predefined string is reached, it will copy the numberofchars after that into the buffertosave
 *USAGE---->>> if (Get_after("some string", 8, buffer)) { do something here}
 * */
//uint8_t Get_After(char *string, uint8_t numberofchars, char *buffertosave);

/* the ISR for the uart. put it in the IRQ handler */
extern void UART_ISR(UART_HandleTypeDef *huart);

#endif /* RE_APP_UART_RING_H */
/**************************** END OF FILE *************************************/
