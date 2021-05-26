/**
  ******************************************************************************
  * @file    MKAS.h
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE MKAS.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MKAS_H_
#define __MKAS_H_

/* Includes ------------------------------------------------------------------*/ 
#include "Spi.h"
#include "Uart.h"
#include "delays.h"
#include "logic.h"

/* defines -------------------------------------------------------------------*/  


/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void MKAS_set_U(void);
void MKAS_get_U(void);
void MKAS_get_I(void);
void MKAS_get_Doza(void);

#endif /* __MKAS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKAS.h */

