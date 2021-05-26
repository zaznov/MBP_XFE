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

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
static char     data_to_MKDS[48] = {0};
static uint8_t  data_from_MKDS[16] = {0};
static uint8_t  result_from_MKDS = 1;

/* Functions -----------------------------------------------------------------*/
static void receive_data_from_MUI(void);
static void send_data_to_MKDS(MODULE MODULE_NAME);
static void receive_data_from_MKDS(MODULE MODULE_NAME);
static void send_data_to_MUI(void);
static void SPI_SendData_to_MKDS(MODULE MODULE_NAME, uint16_t Data);
static uint8_t get_chanal_results_MKDS(MODULE MODULE_NAME, uint16_t chanal_number);
static uint16_t SPI_ReceiveData_from_MKDS(MODULE MODULE_NAME);



/*-------------------------------------------------local-----------------------*/
static void receive_data_from_MUI(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //Выключить прерывание по приему  
    char data;
    uint8_t counter_data = 0;
    for(uint8_t i = 0; i < 48; i++)
    {
            while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET);
        data = UART_ReceiveData(MDR_UART0);
        if(is_HEX_byte(&data))                                                   // Если пришедший байт HEX  
        {
            data_to_MKDS[counter_data++] = data; 

        }
        else                                                                    // Если байт не HEX
        {
            uart_send_confirmation_command('E');                                //Возвращаем сообщение об ошибке. 
            break;
        }
    }
    
    change_from_HEX((uint8_t *)data_to_MKDS, MODULE_MKDS_1);                       // Меняем значения для передачи в ЦАП из Аски в Хекс 
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //Включить прерывание по приему
}


static void send_data_to_MKDS(MODULE MODULE_NAME)
{ 
    uint8_t chanal_number = 0x40;
    uint8_t data = 0;
    for(int i = 0; i < 48; i = i + 3)
    {
        data = ((data_to_MKDS[i] << 4) | data_to_MKDS[i + 1]);
        SPI_SendData_to_MKDS(MODULE_NAME, (chanal_number << 8) | data);
        chanal_number++;
    }
}


static void receive_data_from_MKDS(MODULE MODULE_NAME)
{
    uint32_t counter = 0;
    for(uint32_t chanal = 0x000; chanal <= 0xF00; chanal += 0x100)
    {
        data_from_MKDS[counter++] = get_chanal_results_MKDS(MODULE_NAME,chanal);
    }
}

static void send_data_to_MUI(void)
{
    uart_send_confirmation_command('G');
    uint32_t counter = 0;
    for(uint32_t chanal = 0x000; chanal <= 0xF00; chanal += 0x100)
    {
        uart_send_data(data_from_MKDS[counter++]);
    }
    //uart_send_data(0x0A);
}

static void SPI_SendData_to_MKDS(MODULE MODULE_NAME, uint16_t Data)
{
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_DOWN() : CS_MKDS_2_DOWN();
            SSP_SendData(MDR_SSP0, Data >> 8);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
            SSP_SendData(MDR_SSP0, (uint8_t)Data);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            result_from_MKDS = SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_UP() : CS_MKDS_2_UP();

}



static uint8_t get_chanal_results_MKDS(MODULE MODULE_NAME, uint16_t chanal_number)
{

    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_DOWN() : CS_MKDS_2_DOWN();
            SSP_SendData(MDR_SSP0, chanal_number >> 8);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
            SSP_SendData(MDR_SSP0, (uint8_t)chanal_number);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
        (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_UP() : CS_MKDS_2_UP();
        
        
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_DOWN() : CS_MKDS_2_DOWN();
            SSP_SendData(MDR_SSP0, 0x00);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
            SSP_SendData(MDR_SSP0, 0x00);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            result_from_MKDS = SSP_ReceiveData(MDR_SSP0);
                while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
        (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_UP() : CS_MKDS_2_UP();
        return result_from_MKDS;
}


static uint16_t SPI_ReceiveData_from_MKDS(MODULE MODULE_NAME)
{
    uint16_t PLIS_result = 0x00;  
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_DOWN() : CS_MKDS_2_DOWN();
            SSP_SendData(MDR_SSP0, 0x00);
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
            PLIS_result = SSP_ReceiveData(MDR_SSP0); 
            while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    (MODULE_NAME == MODULE_MKDS_1) ? CS_MKDS_1_UP() : CS_MKDS_2_UP();    
            return (PLIS_result); 
}


/*-------------------------------------------------МКДС 1---------------------*/
void MKDS_reboot(void)
{
    uart_send_confirmation_command('R');
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x8000);
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x0000);   
}
void MKDS_start(void)
{
    uart_send_confirmation_command('M');
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x2000);
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x0000);   
}
void MKDS_mesuring(void)
{
    uart_send_confirmation_command('T');  
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x6000);
    SPI_SendData_to_MKDS(MODULE_MKDS_1, 0x4000);   
}
void MKDS_read_results(void)
{
    uart_send_confirmation_command('O');
    uint16_t PLIS_result[2]  = {0};   
    PLIS_result[0] = SPI_ReceiveData_from_MKDS(MODULE_MKDS_1);
    PLIS_result[1] = SPI_ReceiveData_from_MKDS(MODULE_MKDS_1);
    uart_send_data(PLIS_result[0]);                                      
    uart_send_data(PLIS_result[1]);                                      
    uart_send_data(0x0A);
}
/*-------------------------------------------------МКДС 2---------------------*/
void MKDS_data_to_MKDS(MODULE MODULE_NAME){
    receive_data_from_MUI();
    send_data_to_MKDS(MODULE_NAME);
}


void MKDS_set_MKDS_read(MODULE MODULE_NAME){
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5000);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5100);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5200);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5300);
}

void MKDS_set_MKDS_write(MODULE MODULE_NAME){
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5055);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5155);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5255);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x5355);
}

void MKDS_set_MKDS_disable(MODULE MODULE_NAME){
    SPI_SendData_to_MKDS(MODULE_NAME, 0x50AA);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x51AA);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x52AA);
    SPI_SendData_to_MKDS(MODULE_NAME, 0x53AA);
}



void MKDS_data_from_MKDS(MODULE MODULE_NAME){
    receive_data_from_MKDS(MODULE_NAME);
    send_data_to_MUI();
}




/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.c */
