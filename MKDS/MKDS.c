/**
  ******************************************************************************
  * @file    MKDS.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
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
#include "Uart.h"
/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
static char MKDS_data[48] = {0};
static uint8_t data_from_MKDS[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
uint8_t result_from_MKDS = 1;
/* Functions -----------------------------------------------------------------*/

/*-------------------------------------------------МКДС 1---------------------*/
void reboot_MKDS(void)
{
    uart_send_confirmation_command('R');
    SSP_SendData_MKDS_1(MDR_SSP0, 0x8000);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x0000);   
}
void start_MKDS(void)
{
    uart_send_confirmation_command('M');
    SSP_SendData_MKDS_1(MDR_SSP0, 0x2000);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x0000);   
}
void mesuring_MKDS(void)
{
    uart_send_confirmation_command('T');  
    SSP_SendData_MKDS_1(MDR_SSP0, 0x6000);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x4000);   
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
/*-------------------------------------------------МКДС 2---------------------*/
void data_to_MKDS_1(void){
    receive_data_to_MKDS();
    send_data_to_MKDS_1();
}
void data_to_MKDS_2(void){
    receive_data_to_MKDS();
    send_data_to_MKDS_2();
}
void set_MKDS_1_read(void){
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5000);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5100);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5200);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5300);
}
void set_MKDS_1_write(void){
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5055);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5155);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5255);
    SSP_SendData_MKDS_1(MDR_SSP0, 0x5355);
}
void set_MKDS_2_read(void){
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5000);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5100);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5200);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5300);
}
void set_MKDS_2_write(void){
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5055);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5155);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5255);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x5355);
}
void set_MKDS_1_disable(void){
    SSP_SendData_MKDS_2(MDR_SSP0, 0x50AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x51AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x52AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x53AA);
}
void set_MKDS_2_disable(void){
    SSP_SendData_MKDS_2(MDR_SSP0, 0x50AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x51AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x52AA);
    SSP_SendData_MKDS_2(MDR_SSP0, 0x53AA);
}
void data_from_MKDS_1(void){
    receive_data_from_MKDS_1();
    send_data_to_XFE();
}
void data_from_MKDS_2(void){
    //receive_data_from_MKDS_2();
    //send_data_to_XFE();
}




static void receive_data_to_MKDS(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //Выключить прерывание по приему  
    char data;
    int counter_data = 0;
    for(int i = 0; i < 48; i++)
    {
            while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET);
        data = UART_ReceiveData(MDR_UART0);
        if(is_HEX_byte(data))                                                   // Если пришедший байт HEX  
        {
            MKDS_data[counter_data++] = data; 

        }
        else                                                                    // Если байт не HEX
        {
            uart_send_confirmation_command('E');                                //Возвращаем сообщение об ошибке. 
            break;
        }
    }
    
    convert_data_to_MKDS();
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //Включить прерывание по приему
}

static inline void convert_data_to_MKDS(void)
{
            change_into_HEX((uint8_t *)MKDS_data, MODULE_MKDS);                       // Меняем значения для передачи в ЦАП из Аски в Хекс 
}
static void send_data_to_MKDS_1(void)
{ 
    uint8_t chanal_number = 0x40;
    uint8_t data = 0;
    for(int i = 0; i < 48; i = i + 3)
    {
        data = ((MKDS_data[i] << 4) | MKDS_data[i + 1]);
        SSP_SendData_MKDS_1(MDR_SSP0, (chanal_number << 8) | data);
        chanal_number++;
    }
}

static void send_data_to_MKDS_2(void)
{
    int chanal_number = 0;
    for(int i = 0; i < 48; i = i + 3){
        SSP_SendData_MKDS_1(MDR_SSP0, 0x40+chanal_number);
        SSP_SendData_MKDS_1(MDR_SSP0, ((MKDS_data[i] << 4) | MKDS_data[i + 1]));
        chanal_number++;
    }
}



static void receive_data_from_MKDS_1(void)
{
    int counter = 0;
    for(int chanal = 0x000; chanal <= 0xF00; chanal += 0x100)
    {
        data_from_MKDS[counter++] = get_chanal_results_MKDS_1(chanal);
    }
}

static void send_data_to_XFE(void)
{
    uart_send_confirmation_command('G');
    int counter = 0;
    for(int chanal = 0x000; chanal <= 0xF00; chanal += 0x100)
    {
        uart_send_data(data_from_MKDS[counter++]);
    }
    //uart_send_data(0x0A);
}




/*-------------------------------------------------REST-----------------------*/
void SSP_SendData_MKDS_1(MDR_SSP_TypeDef* SSPx, uint16_t Data)
{
    my_assert_param(IS_SSP_ALL_PERIPH(SSPx));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_1_DOWN();
        SSP_SendData(SSPx, Data >> 8);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(SSPx, (uint8_t)Data);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        result_from_MKDS = SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_1_UP();
}



uint8_t get_chanal_results_MKDS_1(uint16_t chanal_number)
{
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_1_DOWN();
        SSP_SendData(MDR_SSP0, chanal_number >> 8);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, (uint8_t)chanal_number);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_1_UP();
    
    
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_1_DOWN();
        SSP_SendData(MDR_SSP0, 0x00);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x00);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        result_from_MKDS = SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_1_UP();
    return result_from_MKDS;
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

void SSP_SendData_MKDS_2(MDR_SSP_TypeDef* SSPx, uint16_t Data)
{
    my_assert_param(IS_SSP_ALL_PERIPH(SSPx));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_2_DOWN();
        SSP_SendData(SSPx, Data >> 8);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(SSPx, (uint8_t)Data);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        SSP_ReceiveData(MDR_SSP0);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_2_UP();
}

uint16_t SSP_ReceiveData_MKDS_2(MDR_SSP_TypeDef* SSPx)
{
    my_assert_param(IS_SSP_ALL_PERIPH(SSPx));
    
    uint16_t PLIS_result = 0x00;
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    CS_MKDS_2_DOWN();
    SSP_SendData(MDR_SSP0, 0x00);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    PLIS_result = SSP_ReceiveData(MDR_SSP0); 
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    CS_MKDS_2_UP();    
    return (PLIS_result);
}

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.c */
