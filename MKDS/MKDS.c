/**
  ******************************************************************************
  * @file    MKDS.c
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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
 

/* Functions -----------------------------------------------------------------*/
void reboot_MKDS(void)
{
    uart_send_confirmation_command('R');

    SSP_SendData_MKDS1(MDR_SSP0, 0x80);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    
}
void start_MKDS(void)
{
    uart_send_confirmation_command('M');

    SSP_SendData_MKDS1(MDR_SSP0, 0x20);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    
}
void mesuring_MKDS(void)
{
    uart_send_confirmation_command('T');
    
    SSP_SendData_MKDS1(MDR_SSP0, 0x60);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    SSP_SendData_MKDS1(MDR_SSP0, 0x40);
    SSP_SendData_MKDS1(MDR_SSP0, 0x00);
    
}
void read_results_MKDS(void)
{
    uart_send_confirmation_command('O');
    
    uint16_t PLIS_result_1b  = 0x00;   
    uint16_t PLIS_result_2b  = 0x00; 
    PLIS_result_1b = SSP_ReceiveData_MKDS_1(MDR_SSP0);
    PLIS_result_2b = SSP_ReceiveData_MKDS_1(MDR_SSP0);
    uart_send_data(PLIS_result_1b);                                      
    uart_send_data(PLIS_result_2b);                                      
    uart_send_data(0x0A);
}



void SSP_SendData_MKDS1(MDR_SSP_TypeDef* SSPx, uint16_t Data)
{
    my_assert_param(IS_SSP_ALL_PERIPH(SSPx));
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_1_DOWN();
    SSP_SendData(SSPx, Data);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_1_UP();
}

uint16_t SSP_ReceiveData_MKDS_1(MDR_SSP_TypeDef* SSPx)
{
    my_assert_param(IS_SSP_ALL_PERIPH(SSPx));
    
    uint16_t PLIS_result = 0x00;
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_1_DOWN();
    SSP_SendData(MDR_SSP0, 0x00);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    PLIS_result = SSP_ReceiveData(MDR_SSP0); 
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_1_UP();    
    return (PLIS_result);
}

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.c */
