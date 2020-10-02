/**
  ******************************************************************************
  * @file    MKDS.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    03/07/2020
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the MKDS-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE MKDS.c
  */
/* Includes ------------------------------------------------------------------*/
#include "MKDS.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
uint16_t PLIS_status  = 0xBB;

/* Functions -----------------------------------------------------------------*/
void Data_Response_from_MKDS(void)                                                            // Функиция работы с ТЗЧ - платой МКДС
{
    uint16_t byte1 = 0;
    uint16_t byte2 = 0;
    uint16_t byte3 = 0;
    uint16_t byte4 = 0;
    uint16_t byte_false = 0;
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x06); 
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    byte_false = SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x33);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    byte1 = SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x33);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    byte2 = SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x33);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    byte3 = SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x33);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    byte4 = SSP_ReceiveData(MDR_SSP0);  
    //Delay1(100);
    UART_SendData_fixed(byte1);                                      //Отправляем обратно byte1 
    //Delay1(100);
    UART_SendData_fixed(byte2);                                      //Отправляем обратно byte2 
    //Delay1(100);
    UART_SendData_fixed(byte3);                                      //Отправляем обратно byte3 
    //Delay1(100);
    UART_SendData_fixed(byte4);                                      //Отправляем обратно byte4        
    //Delay1(100);
}


void Get_TZCH(void)
{
    UART_Send_Confirmation_command('T');
    PLIS_status = 0xBB;
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, 0x01);
    
    while(PLIS_status  == 0xBB)
    { 
        Delay2(50000);                                                          //Ждем результаты 
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x07);                                           //Отправляем команду запроса статуса
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x33); 
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        PLIS_status = SSP_ReceiveData(MDR_SSP0);                                //Фиктивное считывание для смещая буфера
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        PLIS_status = SSP_ReceiveData(MDR_SSP0);                                //Считываем статус ПЛИС
        
        if(PLIS_status == 0xCC) Data_Response_from_MKDS();                      //Если результаты готовы, то считываем их   
    }
}

void Delay2(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}


/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.c */
