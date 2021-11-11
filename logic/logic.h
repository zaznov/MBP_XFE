/**
  ******************************************************************************
  * @file    logic.h
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    28/08/2021
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
#include "stdbool.h"
/* defines -------------------------------------------------------------------*/  

typedef enum{
    MODULE_MKDS_1,
    MODULE_MKDS_2,
    MODULE_MKAS,
}MODULE;

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void clean_uart_buffer(void);
void change_from_HEX(uint8_t *data, MODULE MODULE_name);
void change_to_HEX(uint8_t *data);
uint16_t change_to_ASCII(const uint8_t data);
bool is_correct_byte(const char *byte);
bool is_HEX_byte(const char * byte);
int  invert_bit_order(uint8_t x);
void swap(uint16_t* a, uint16_t* b);
void bubble_sort(uint16_t* my_arry, uint8_t lenth);



#endif /* __LOGIC_H_ */

/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE logic.h */

