/**
  ******************************************************************************
  * @file    delays.c
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    28/10/2019
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the delays-library of Cortex-M4 family:           
  *           + Delay in milliseconds
  *           + Delay in microseconds
  *           + ...
  * @attention SysTick Timer is used for delays!
  ******************************************************************************
  * FILE delays.c
  */
/* Includes ------------------------------------------------------------------*/
#include "delays.h"

/* Variables -----------------------------------------------------------------*/
volatile uint32_t delay_Wert = 0;                                               //delay_Value
volatile uint32_t ticks_Wert = 0;                                               //ticks_Value
extern uint32_t SystemCoreClock;


/* Functions -----------------------------------------------------------------*/

/**
  * @brief  Function creats a delay in milliseconds
  * @param  delay_in_mils: delay value in milliseconds.
  * @retval None
  */
void delay_mls(uint32_t delay_in_mils)
{  
    SysTick_Config(SystemCoreClock/1000);                                       //SystemCoreClock - �������� ������ ����������� 
    uint32_t start = ticks_Wert;
    while((ticks_Wert - start) < delay_in_mils);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  // ��������� ������ SysTick
}   

/**
  * @brief  Function creats a delay in microseconds
  * @param  delay_in_mks: delay value in microseconds.
  * @retval None
  */
void delay_mks(const uint32_t delay_in_mks)
{  
    if (delay_in_mks > SYSTICK_MAX_VALUE || delay_in_mks == 0)                  // ����������� �� ������ �������� � ����� � ������������ ������������ ������ 24 �������
        return;
    
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;                                 // ��� TICKINT = 0 � ��������� ���������� �� ���������� 0
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;                                // ��� CLKSOURCE = 1 � ������� �������� ������������ 0 � �������, 1 � ���������
    SysTick->LOAD = (SystemCoreClock / 1000000 * delay_in_mks - 1);             // ������ ������� �������� �� ��, ����� �������� ����� ���������� ��� ���������� �������
    SysTick->VAL = 0;                                                           // ���������� ������� ��������
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                                   // ��� ENABLE = 1 � ��������� ��� ���������� ������� 0 � ��������� ������, 1 � ��������� ������
    
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));                      // ������� ��������� 1 � ����, ���������� �� COUNTFLAG 
    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;                               // �������� COUNTFLAG ;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  // ��������� ������ SysTick
} 

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE delays.c */



