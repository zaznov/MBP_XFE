/**
  ******************************************************************************
  * @file    Spi.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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

/* Defines -------------------------------------------------------------------*/ 
#define CS_MKDS_1_DOWN()  PORT_ResetBits(PORTB, PORT_Pin_25)
#define CS_MKDS_1_UP()    PORT_SetBits(PORTB, PORT_Pin_25)
/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void pin_init_spi(void);
void spi_init(void);
void spi_init_MKAS(void);
void spi_init_MKDS(void);

#endif /* __XFE_SPI_H */

/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Spi.h */
