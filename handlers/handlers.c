/**
  ******************************************************************************
  * @file    handlers.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file provides firmware functions to work with following handlers:           
  *           + UART0_Handler
  *           + PORTB_Handler
  *           + SysTick_Handler
  ******************************************************************************
  * FILE handlers.c
  */
/* Includes ------------------------------------------------------------------*/
#include "handlers.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
char Gotten_command[4] = "000";                                                 // ��������� RS_command[4] ��� 0x00 (����-�����������) ��� ��������� � strcmp
extern volatile bool ADC_Ready;                                              
extern  uint32_t ticks_Wert;

/* Functions -----------------------------------------------------------------*/

void INT_UART0_Handler()  
{
    if(UART_GetITStatusMasked(MDR_UART0, UART_IT_RX) == SET)                    //���� ������� ��� ���������� �� ������
    {  
        for(uint8_t i = 0; i < 3; i ++)
        {
                while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET);
            Gotten_command[i] = UART_ReceiveData(MDR_UART0);  
            if(is_correct_byte(&Gotten_command[i]))                                         // ���� ��������� ���� ���������  
            {
                if (Gotten_command[i] == 0x0A) 
                {
                    change_event();
                    return;
                }                
            }
            else                                                                    // ���� ���� �� ���������
            {
                uart_send_confirmation_command('E');                                // ���������� ��������� �� ������. 
                return;
            }
        }
        //UART_ClearITPendingBit(MDR_UART0, UART_IT_RX);                          // ������� ��� ������ ������.
    }
}


void IRQ_PORTB_Handler()
{
    NVIC_DisableIRQ(PORTB_IRQn);   
    ADC_Ready = 1;
}


void SysTick_Handler()
{
    ticks_Wert++;
} 

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE handlers.c */
