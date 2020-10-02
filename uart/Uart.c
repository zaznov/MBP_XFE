/**
  ******************************************************************************
  * @file    Uart.c
  * @author  Zaznov NIIKP
  * @version V1.0.1
  * @date    18/03/2020
  * @brief   This file provides all the functions prototypes for work with UART 
             from XFE project
  ******************************************************************************
  * FILE Uart.c
  */

/* Includes ------------------------------------------------------------------*/


/* My Includes ---------------------------------------------------------------*/
#include "Uart.h"


/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
char Permissible_value[24] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','I','M','O','R','S','T','U','\n'};            // 0x10 - �������� ������ Enter //������ ����������� ��������.
    
/* Functions -----------------------------------------------------------------*/
void Pin_init_uart()
{
    
}

void uart_init()
{
    Pin_init_uart();                                               
    
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_UART0_EN, ENABLE);                 // ���. ������������ ����������� UART0
    UART_CLK_en(MDR_UART0, UART_CLKSRC_HSE0, UART_CLKdiv1);                     // ��������� ������������ ����������� UART0
    
    UART_InitTypeDef uart_user_ini;
    uart_user_ini.UART_BaudRate             = 9600;                             // �������� �������� ������
    uart_user_ini.UART_FIFOMode             = UART_FIFO_OFF;                    // ���������/���������� ������
    uart_user_ini.UART_HardwareFlowControl  = (UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE);    // ���������� �������� �� ��������� � ������� ������. �����������(!!) - �������� RX and TX !!
    uart_user_ini.UART_Parity               = UART_Parity_No;                   // �������� ��������
    uart_user_ini.UART_StopBits             = UART_StopBits1;                   // ���������� STOP-�����
    uart_user_ini.UART_WordLength           = UART_WordLength8b;                // ���������� ����� ������ � ���������
    UART_Init(MDR_UART0, &uart_user_ini, UART_FREQ);                            //������������� UART 
    
    UART_Cmd(MDR_UART0, ENABLE);                                                //��������� UART 
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //�������� ���������� �� ������
    NVIC_EnableIRQ(UART0_IRQn);                                                 //�������� ����� ���������� �� UART 0
}


bool IsCorrect_Byte(char byt)                                     // inline - ����� ������ ������� ��� ���������� ������ ��������������� � �� ���. 
{
    for(int i = 0; i < 24; i++)
    {
        if (Permissible_value[i] == byt) return true;
        if (i == 23) 
        {
            return false;
        }
    }
    return false;
}
bool Is_HEX_Byte(char byt)                                               // inline - ����� ������ ������� ��� ���������� ������ ��������������� � �� ���. 
{
    for(int i = 0; i <= 0x0F; i++)
    {
        if (byt == 0x0A) return true;
        if (Permissible_value[i] == byt) return true;
        /*if (i > 0x0F) 
        {
            return false;
        }*/
    }
    return false;
}


void UART_Send_Confirmation_command(char command)                               // ������� ������������� �������� �������
{
    uint16_t Data = 0;
    switch(command)
        {
            case('R'):
                Data =0x524F; 
                break;
            case ('U'):
                Data =0x554F;
                break;
            case ('I'):
                Data =0x494F;
                break;
            case ('D'):
               Data =0x444F;
                break;
            case ('T'):
                Data =0x544F;
                break;
            case ('S'):
                Data =0x534F;
                break;
            case ('E'):
                Data =0x4552;
                break;
        } 
    UART_SendData_fixed(Data >> 8);
    UART_SendData_fixed(Data);
    UART_SendData_fixed(0x0A);
}

void UART_SendData_fixed(uint16_t Data)
{
    PORT_SetBits(PORTE, PORT_Pin_23);                                           // �������� �������� �� UART �� ������� RS-485
    PORT_SetBits(PORTE, PORT_Pin_22);                                           // �� �����
    while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_TXFE)!= SET);               // ���� �������� ��������� �������� ������������ UART, �� ������ ������ :)
    UART_SendData(MDR_UART0, Data);
    while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_BUSY)== SET);               // ���� ��������� �������� ������ � ����� UART
    PORT_ResetBits(PORTE, PORT_Pin_23);                                         // �������� ����� �� UART �� ������� RS-485
    PORT_ResetBits(PORTE, PORT_Pin_22);                                         // �� �����
}



/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Uart.c */
