/**
  ******************************************************************************
  * @file    handlers.c
  * @author  Zaznov NIIKP
  * @version V1.0.2
  * @date    25/02/2020
  * @brief   This file provides firmware functions to work with following handlers:           
  *           + 
  *           + UART0_Handler
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
        if(IsCorrect_Byte(Gotten_byte))                                         // ���� ��������� ���� ���������  
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
            UART_Send_Confirmation_command('E');                                // ���������� ��������� �� ������. 
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



/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE handlers.c */
