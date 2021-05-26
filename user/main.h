/**
  ******************************************************************************
  * @file    Main.h
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file contains all MAIN functions prototypes for work with MAIN 
             from XFE project
  ******************************************************************************
  * FILE Main.h
  */
 
 #ifndef __XFE_MAIN_H
#define __XFE_MAIN_H

 /* Includes ------------------------------------------------------------------*/
#include "mdr32f8_it.h"                 // 
#include "spec.h"                       // 
#include "mdr32f8_config.h"             // Keil::Drivers:CONFIG
#include "mdr32f8_clkctrl.h"            // Keil::Drivers:CLKCTRL
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "mdr32f8_ssp.h"
#include "core_cm4.h"

/* My Includes ---------------------------------------------------------------*/
#include "Uart.h"
#include "Spi.h"
#include "state_machine.h"
#include "my_assert.h"
#include "wdt.h"
#include "my_assert.h"
/* Defines -------------------------------------------------------------------*/ 
typedef enum{
    HSE0_ON     =   (uint32_t)0x08000000,
    HSE1_ON     =   (uint32_t)0x08000001,
}CLK;
typedef enum{
    HSE0_RDY    =   (uint32_t)0x54,
    HSE1_RDY    =   (uint32_t)0x74,
}CLK_RDY;
typedef enum{
    HSE0div1    =   (uint32_t)0x00000002,
    HSE0div2    =   (uint32_t)0x00000003,
    HSE1div1    =   (uint32_t)0x00000004,
    HSE1div2    =   (uint32_t)0x00000005,
}CLKCTRL_MAX;
typedef enum{
    UART_HSE0   =   (uint32_t)0x00000001,
    UART_HSE1   =   (uint32_t)0x00000002,
}UART_CLK;



/* Variables -----------------------------------------------------------------*/
CLK         CURRENT_CLK         = HSE0_ON;
CLK_RDY     CURRENT_CLK_RDY     = HSE0_RDY;
CLKCTRL_MAX CURRENT_CLKCTRL_MAX = HSE0div1;
UART_CLK    CURRENT_UART_CLK    = UART_HSE0;


/* Functions -----------------------------------------------------------------*/


#endif /* __XFE_MAIN_H */
/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Main.h */
