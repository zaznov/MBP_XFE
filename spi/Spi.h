/**
  ******************************************************************************
  * @file    Spi.h
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file contains all the functions prototypes for work with SPI 
             from XFE project
  ******************************************************************************
  * FILE Spi.h
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XFE_SPI_H
#define __XFE_SPI_H

/* Includes ------------------------------------------------------------------*/
#include "mdr32f8_ssp.h"
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "mdr32f8_clkctrl.h"            // Keil::Drivers:CLKCTRL
#include "spec.h"                       // 

#include "logic.h"
/* Defines -------------------------------------------------------------------*/ 
#define CS_MKDS_1_DOWN()  PORT_ResetBits(PORTB, PORT_Pin_25)
#define CS_MKDS_1_UP()    PORT_SetBits(PORTB, PORT_Pin_25)
#define CS_MKDS_2_DOWN()  PORT_ResetBits(PORTB, PORT_Pin_17)
#define CS_MKDS_2_UP()    PORT_SetBits(PORTB, PORT_Pin_17)
/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void pin_init_spi(void);
void spi_init(void);
void spi_reinit(MODULE MODULE_NAME);

#endif /* __XFE_SPI_H */

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Spi.h */
