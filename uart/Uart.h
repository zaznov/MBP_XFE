/**
  ******************************************************************************
  * @file    Uart.h
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    18/03/2020
  * @brief   This file contains all the functions prototypes for work with UART 
             from XFE project
  ******************************************************************************
  * FILE Uart.h
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XFE_UART_H
#define __XFE_UART_H

/* Includes ------------------------------------------------------------------*/  
#include "stdbool.h"
#include "mdr32f8_uart.h"               // Keil::Drivers:UART
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "mdr32f8_clkctrl.h"            // Keil::Drivers:CLKCTRL
#include "mdr32f8_it.h"                 // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup
#include "spec.h"                       // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup


/* Defines -------------------------------------------------------------------*/ 
#define UART_FREQ     8000000 //8 MHz, Calculate it going through your clock tree
/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void Pin_init_uart(void);
void uart_init(void);

void UART_SendData_fixed(uint16_t Data);
void UART_Send_Confirmation_command(char command);
bool IsCorrect_Byte(char byt);
bool Is_HEX_Byte(char byt);



#endif /* __XFE_UART_H */

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Uart.h */
