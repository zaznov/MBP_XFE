/**
  ******************************************************************************
  * @file    handlers.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
  * @brief   This file contains functions prototypes for work with interrupt
  handlers of stm32f4xx
  ******************************************************************************
  * FILE handlers.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HANDLERS_H_
#define __HANDLERS_H_
/* Includes ------------------------------------------------------------------*/  
#include "mdr32f8_it.h"                 // 
#include "Uart.h"
#include "state_machine.h"

/* defines -------------------------------------------------------------------*/  

/* Variables -----------------------------------------------------------------*/
extern volatile _Bool ADC_Ready;                                              


/* Functions -----------------------------------------------------------------*/

#endif /* __HANDLERS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE handlers.h */
