/**
  ******************************************************************************
  * @file    logic.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    01/05/2021
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE logic.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOGIC_H_
#define __LOGIC_H_
/* Includes ------------------------------------------------------------------*/ 
#include "MDR1986VE8T.h"
#include "my_assert.h"
#include <string.h>
/* defines -------------------------------------------------------------------*/  

typedef enum{
    MODULE_MKDS,
    MODULE_MKAS,
}MODULE;

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void clean_uart_buffer(void);
void change_into_HEX(uint8_t *data, MODULE MODULE_name);
#endif /* __LOGIC_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE logic.h */

