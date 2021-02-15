/**
  ******************************************************************************
  * @file    Uart.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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
#include "mdr32f8_it.h"                 // 
#include "spec.h"                       // 


/* Defines -------------------------------------------------------------------*/ 
#define UART_FREQ     8000000                                                   //8 MHz 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void uart_init(uint32_t MY_UART_CLKSRC);
void uart_send_data(uint16_t Data);
void uart_send_confirmation_command(char command);
bool is_correct_byte(char byt);
bool is_HEX_byte(char byt);



#endif /* __XFE_UART_H */

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Uart.h */
