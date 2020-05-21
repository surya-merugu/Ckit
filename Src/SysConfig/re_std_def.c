/**
  *****************************************************************************
  * Title                 :   CONVERSION KIT
  * Filename              :   re_std_def.c
  * Origin Date           :   08/02/2020
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
#include "re_std_def.h"

/**
 * @brief Error handler for the application code
 * @param pfile Pointer to the file name where the error occured
 * @param line Line number at which the error occured
 * @retval void
 */
void RE_Error_Handler (const char* pfile, uint16_t line)
{
    printf(pfile);
    printf("\n%d\n", line);
  //  while(1);
}

/*********************** (C) COPYRIGHT RACEnergy **********END OF FILE********/
