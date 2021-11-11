/**
  ******************************************************************************
  * @file    logic.c
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    28/08/2021
  * @brief   This file contains following logical functions for XFE project:        
  *           + clean_uart_buffer
  *           + change_from_HEX
  *           + change_to_HEX
  *           + change_to_ASCII
  *           + is_correct_byte
  *           + is_HEX_byte
  *           + bubble_sort
  *           + invert_bit_order
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
/**
  * @brief  Cleans uart recieve buffer.
  * @param  None
  * @retval None
  */
void clean_uart_buffer(void)
{
    memset(Gotten_command, 0x00, 4);
}


/**
  * @brief  Changes data from ASCII-HEX type (is used for UART transfering) 
  * to uint8_t type.
  */
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


/**
  * @brief  Changes data to ASCII-HEX type (is used for UART transfering) 
  * form uint8_t type.
  */
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
/*
uint16_t change_to_ASCII(const uint8_t data)
{
    uint16_t rezult = 0;
    
    if(data <= 0x09)
    {
        rezult = data + 0x30;
    }
    else if(data >= 0x0A && data <= 0x0F)
    {
        rezult = data + 0x37;
    }
    else 
    {
        //my_assert_param(0);
        while(1);                                                               // assert?
    }
    return rezult;
}
*/


/**
  * @brief  Check if the gotten byte belongs to Permissible_value[] array
  */
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


/**
  * @brief  Check if the gotten byte is ASCII-HEX type value
  */
bool is_HEX_byte(const char * byte)                                                     // inline - место вызова функции при компиляции просто разворачивается в ее код. 
{
    for(int i = 0; i <= 0x0F; i++)
    {
        if (Permissible_value[i] == *byte || *byte == 0x0A) return true;
    }
    return false;
}

/**
  * @brief  Swap 2 uint16_t values.
  */
void swap(uint16_t* a, uint16_t* b)
{
    uint16_t tmp = *a;
    *a = *b;
    *b = tmp;
}



/**
  * @brief  Bubble sorting of an array.
  * @param  my_arry: Poiner on an array to be sorted.
  * @param  lenth: Lenth of the array.
  * @retval None
  */
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


/**
  * @brief  Invers bits order in uint8_t value.
  * @param  x: Value to be inverted.
  * @retval res: Inverted value
  */
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


/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE logic.c */
