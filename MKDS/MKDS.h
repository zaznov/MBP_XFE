/**
  ******************************************************************************
  * @file    MKDS.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE MKDS.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MKDS_H_
#define __MKDS_H_

/* Includes ------------------------------------------------------------------*/ 
#include "Spi.h"
#include "Uart.h"
#include "my_assert.h"

/* Defines -------------------------------------------------------------------*/  

/* Variables -----------------------------------------------------------------*/


/* Functions -----------------------------------------------------------------*/
void Data_Response_from_MKDS(void);
void get_TZCH(void);


void reboot_MKDS(void);
void start_MKDS(void);
void mesuring_MKDS(void);
void read_results_MKDS(void);


uint16_t SSP_ReceiveData_MKDS_1(MDR_SSP_TypeDef* SSPx);
void SSP_SendData_MKDS1(MDR_SSP_TypeDef* SSPx, uint16_t Data);

    
#endif /* __MKDS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.h */

