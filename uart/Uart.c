/**
  ******************************************************************************
  * @file    Uart.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
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
   
/* Functions -----------------------------------------------------------------*/


void uart_init(uint32_t MY_UART_CLKSRC)
{
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_UART0_EN, ENABLE);                 // ���. ������������ ����������� UART0
    UART_CLK_en(MDR_UART0, MY_UART_CLKSRC, UART_CLKdiv1);                     // ��������� ������������ ����������� UART0
    
    UART_InitTypeDef uart_user_ini;
    uart_user_ini.UART_BaudRate             = 19200;                             // �������� �������� ������
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

void uart_send_confirmation_command(const char command)                               // ������� ������������� �������� �������
{
    uint16_t Data = 0;
    switch(command)
        {
/*-------------------------------------------------����-----------------------*/
            case ('U'):
                Data =0x554F;
                break;
            case ('I'):
                Data =0x494F;
                break;
            case ('D'):
               Data =0x444F;
                break;
            case ('S'):
                Data =0x534F;
                break;
            case ('E'):
                Data =0x4552;
                break;
/*-------------------------------------------------����-----------------------*/ 
            case('R'):
                Data =0x524F; 
                break;
            case ('T'):
                Data =0x544F;
                break;
            case ('M'):
                Data =0x4D4F;
                break;
            case ('O'):
                Data =0x4F4F;
                break;
            case ('G'):
                Data =0x474F;
                break;
/*----------------------------------------------------------------------------*/   
        } 
    uart_send_data(Data >> 8);
    uart_send_data(Data);
    uart_send_data(0x0A);
}

void uart_send_data(uint16_t Data)
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
