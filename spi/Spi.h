/**
  ******************************************************************************
  * @file    Spi.h
  * @author  Zaznov NIIKP
  * @version V1.0.3
  * @date    21/06/2020
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
#include "spec.h"                       // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void Pin_init_spi(void);
void spi_init(void);

#endif /* __XFE_SPI_H */

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Spi.h */
