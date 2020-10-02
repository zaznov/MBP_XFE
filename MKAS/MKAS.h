/**
  ******************************************************************************
  * @file    MKAS.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    14/06/2020
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
void Data_Response_ADC(void);
int  Invert(uint8_t x);
void Set_U(void);
void Set_U_local(void);
void Get_U(void);
void Get_I(void);
void Get_Doza(void);
static inline void Choose_ADC_Chanel(int Chanel);
static inline void Choose_DAC(int number);
static inline void send_data_to_DAC(int i);
uint8_t Change_into_HEX_(uint8_t data);
void MKAS_init(void);



#endif /* __MKAS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKAS.h */

