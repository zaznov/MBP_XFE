/**
  ******************************************************************************
  * @file    handlers.c
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
  * @brief   This file provides firmware functions to work with following handlers:           
  *           + UART0_Handler
  *           + PORTB_Handler
  *           + ...
  ******************************************************************************
  * FILE handlers.c
  */
/* Includes ------------------------------------------------------------------*/
#include "handlers.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
int command_counter = 0;
char Gotten_command[4] = "000";                                                 // ��������� RS_command[4] ��� 0x00 (����-�����������) ��� ��������� � strcmp
/* Functions -----------------------------------------------------------------*/

void INT_UART0_Handler(void)  
{
    char Gotten_byte = 0;
    if(UART_GetITStatusMasked(MDR_UART0, UART_IT_RX) == SET)                    //���� ������� ��� ���������� �� ������
    {  
        Gotten_byte = UART_ReceiveData(MDR_UART0);   
        if(is_correct_byte(Gotten_byte))                                         // ���� ��������� ���� ���������  
        {
            Gotten_command[command_counter] = Gotten_byte;
            command_counter++;
            if (Gotten_byte == 0x0A) 
            {
                command_counter = 0;                                            // ���� ������ ������� ����� ������, �� ���������� ������ 
                Gotten_command[3] = 0x00;
                change_event();
            }                
        }
        else                                                                    // ���� ���� �� ���������
        {
            uart_send_confirmation_command('E');                                // ���������� ��������� �� ������. 
            command_counter = 0;
        }
        UART_ClearITPendingBit(MDR_UART0, UART_IT_RX);                          // ������� ��� ������ ������.
    }
}


void IRQ_PORTB_Handler()
{
    NVIC_DisableIRQ(PORTB_IRQn);   
    ADC_Ready = 1;
}


void SysTick_Handler(void)
{
    ticks_Wert++;
} 

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE handlers.c */
