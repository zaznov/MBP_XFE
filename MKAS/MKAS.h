/**
  ******************************************************************************
  * @file    MKAS.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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

/* defines -------------------------------------------------------------------*/  
#define Start_ADC   PORT_Pin_27                                                 // Start ADC
#define A3          PORT_Pin_18                                                 //CS
#define A2          PORT_Pin_22
#define A1          PORT_Pin_21
#define A0          PORT_Pin_20
#define Sel1        PORT_Pin_28
#define Sel2        PORT_Pin_29
#define Sel3        PORT_Pin_30
#define Sel4        PORT_Pin_31

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void data_response_ADC(void);
void set_U(void);
void set_U_local(void);
void get_U(void);
void get_I(void);
void get_Doza(void);
uint8_t change_into_HEX(uint8_t data);
void MKAS_init(void);



#endif /* __MKAS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKAS.h */

