/**
  ******************************************************************************
  * @file    Uart.h
  * @author  Zaznov NIIKP
  * @version V3.0.1
  * @date    27/08/2021
  * @brief   This file contains all the functions prototypes for work with UART 
             from XFE project
  ******************************************************************************
  * FILE Uart.h
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XFE_UART_H
#define __XFE_UART_H

/* Includes ------------------------------------------------------------------*/  
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
void uart_send_confirmation_command(const char command);

#endif /* __XFE_UART_H */

/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE Uart.h */
