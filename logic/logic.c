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
const static char Permissible_value[26] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F', 'G', 'I','M','O','R','S','T','U','W','\n'};  // 0x10 - означает символ Enter. Список разрешенных символов.

/* Functions -----------------------------------------------------------------*/
void clean_uart_buffer(void)
{
    memset(Gotten_command, 0x00, 4);
}



void change_from_HEX(uint8_t *data, MODULE MODULE_name)
{
    uint8_t array_size = (MODULE_name == MODULE_MKAS)? DATA_ARRAY_MKAS : DATA_ARRAY_MKDS;

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
void change_to_HEX(uint8_t *data)
{
        if(*data <= 0x09)
        {
            *data += 0x30;
        }
        else if(*data >= 0x0A && *data <= 0x0F)
        {
            *data += 0x37;
        }
        else 
        {
            //my_assert_param(0);
            while(1);                                                           // assert?
        }
}




bool is_correct_byte(const char *byte)                                                 // inline - место вызова функции при компиляции просто разворачивается в ее код. 
{
    for(int i = 0; i < 26; i++)
    {
        if (Permissible_value[i] == *byte) return true;
        if (i == 25) 
        {
            return false;
        }
    }
    return false;
}

bool is_HEX_byte(const char * byte)                                                     // inline - место вызова функции при компиляции просто разворачивается в ее код. 
{
    for(int i = 0; i <= 0x0F; i++)
    {
        if (Permissible_value[i] == *byte || *byte == 0x0A) return true;
    }
    return false;
}


void swap(uint16_t* a, uint16_t* b)
{
    uint16_t tmp = *a;
    *a = *b;
    *b = tmp;
}
void bubble_sort(uint16_t* my_arry, uint8_t lenth)
{
    //uint8_t lenth = 5;
    bool not_sorted = true;
    
    while(not_sorted)
    {
        not_sorted = false;
        for(uint8_t i = 0; i < lenth-1; i++)
        {
            if(my_arry[i] > my_arry[i+1])
            {
                swap(&my_arry[i], &my_arry[i+1]);
                not_sorted = true;
            }
        }
        lenth--;
    }
}

int  invert_bit_order(uint8_t x) 
{        
    int base = 256;
    int  res = 0;
    while (x != 0) 
    {
        res += (x & 1) * (base >>= 1);
        x >>= 1;
    }
    return res;
}
/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE logic.c */
