/**
  ******************************************************************************
  * @file    logic.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    01/05/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Template-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE logic.c
  */
/* Includes ------------------------------------------------------------------*/
#include "logic.h"
/* Defines -------------------------------------------------------------------*/ 
#define DATA_ARRAY_MKAS 32
#define DATA_ARRAY_MKDS 48


/* Variables -----------------------------------------------------------------*/
extern char Gotten_command[4]; 
/* Functions -----------------------------------------------------------------*/
void clean_uart_buffer(void)
{
    memset(Gotten_command, 0x00, 4);
}



void change_into_HEX(uint8_t *data, MODULE MODULE_name)
{
    uint8_t array_size = (MODULE_name == MODULE_MKDS)? DATA_ARRAY_MKDS : DATA_ARRAY_MKAS;

    for(int i = 0; i < array_size; i++)
    {
        if(data[i] >= 0x30 && data[i] <= 0x39)
        {
            data[i] -= 0x30;
        }
        else if(data[i] >= 0x41 && data[i] <= 0x46)
        {
            data[i] -= 0x37;
        }
        else if(data[i] == 0x0A)
        {
        }
        else 
        {
            //my_assert_param(0);
            while(1);                                                           // assert?
        }
    }
}
/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE logic.c */
