/**
  ******************************************************************************
  * @file    MKDS.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    03/07/2020
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

/* Defines -------------------------------------------------------------------*/  

/* Variables -----------------------------------------------------------------*/


/* Functions -----------------------------------------------------------------*/
void Data_Response_from_MKDS(void);
void Get_TZCH(void);

void Delay2(__IO uint32_t nCount);
    
#endif /* __MKDS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.h */

