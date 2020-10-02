/**
  ******************************************************************************
  * @file    Main.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    08/06/2020
  * @brief   This file contains all MAIN functions prototypes for work with MAIN 
             from XFE project
  ******************************************************************************
  * FILE Main.h
  */
  
 /* Includes ------------------------------------------------------------------*/
#include "mdr32f8_it.h"                 // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup
#include "spec.h"                       // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup
#include "mdr32f8_config.h"             // Keil::Drivers:CONFIG
#include "mdr32f8_clkctrl.h"            // Keil::Drivers:CLKCTRL
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "mdr32f8_ssp.h"
#include "core_cm4.h"
//#include <assert.h>

/* My Includes ---------------------------------------------------------------*/
#include "Uart.h"
#include "Spi.h"
#include "MKAS.h"
#include "MKDS.h"
#include "handlers.h"
#include "state_machine.h"
#include "my_assert.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
extern STATE_FUNC_PTR_t state_T;

/* Functions -----------------------------------------------------------------*/
void Pin_init(void);
void mcu_init(void);

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Main.h */
