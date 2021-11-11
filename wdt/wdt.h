/**
  ******************************************************************************
  * @file    wdt.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    12/02/2021
  * @brief   This file contains all the functions prototypes for work with 
  * Watch dog Timer driver of Milandr 1986VE8T
  ******************************************************************************
  * FILE wdt.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WGT_H_
#define __WGT_H_
/* Includes ------------------------------------------------------------------*/ 
//#include "MDR1986VE8T.h"
/* defines -------------------------------------------------------------------*/  

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void wdt_init(void);
void wdt_rewrite(void);
void wdt_reinit(void);
#endif /* __WGT_H_ */

/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE wdt.h */

