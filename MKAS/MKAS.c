/**
  ******************************************************************************
  * @file    MKAS.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the MKAS-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE MKAS.c
  */
/* Includes ------------------------------------------------------------------*/
#include "MKAS.h"

/* Defines -------------------------------------------------------------------*/ 
#define Start_ADC   PORT_Pin_27                                                 // Start ADC
#define A3          PORT_Pin_18                                                 //CS
#define A2          PORT_Pin_22
#define A1          PORT_Pin_21
#define A0          PORT_Pin_20
#define Sel1        PORT_Pin_28
#define Sel2        PORT_Pin_29
#define Sel3        PORT_Pin_30
#define Sel4        PORT_Pin_31

#define ADC_REPETITIONS_NUMBER        5

/* Variables -----------------------------------------------------------------*/
volatile _Bool ADC_Ready = 0;
static char DAC_data[32] = {0};

/* Functions -----------------------------------------------------------------*/
static void choose_ADC_Chanel(uint16_t Chanel);
static void send_data_to_DAC(uint16_t i);
static void set_U_local(void);
static void data_response_ADC(void (*sort_function)(uint16_t*, uint8_t));
static uint16_t adc_read(void);
static inline void choose_DAC(uint16_t number);
static void send_one_byte_from_adc(uint16_t data);

/* ---------------------------------------------------------------------------*/

static void send_one_byte_from_adc(uint16_t data)
{ 
    uint8_t data_byte_to_transfer = data&0x0F;
    change_to_HEX(&data_byte_to_transfer);
    uart_send_data(data_byte_to_transfer);
    
    /*switch(data&0x0F)
        {
            case(0):
                uart_send_data(0x30);
                break;
            case(1):
                uart_send_data(0x31);
                break;
            case(2):
                uart_send_data(0x32);
                break;
            case(3):
                uart_send_data(0x33);
                break;
            case(4):
                uart_send_data(0x34);
                break;
            case(5):
                uart_send_data(0x35);
                break;
            case(6):
                uart_send_data(0x36);
                break;
            case(7):
                uart_send_data(0x37);
                break;
            case(8):
                uart_send_data(0x38);
                break;
            case(9):
                uart_send_data(0x39);
                break;
            case(0x0A):
                uart_send_data(0x41);
                break;
            case(0x0B):
                uart_send_data(0x42);
                break;
            case(0x0C):
                uart_send_data(0x43);
                break;
            case(0x0D):
                uart_send_data(0x44);
                break;
            case(0x0E):
                uart_send_data(0x45);
                break;
            case(0x0F):
                uart_send_data(0x46);
                break;          
        } */
}

static void data_response_ADC(void (*sort_function)(uint16_t*, uint8_t))
{
    uint16_t data_from_adc[ADC_REPETITIONS_NUMBER];
    for(uint8_t i = 0; i < ADC_REPETITIONS_NUMBER; i++)
    {
        data_from_adc[i] = adc_read();
    }
    sort_function(data_from_adc, ADC_REPETITIONS_NUMBER);
    
    
    send_one_byte_from_adc(data_from_adc[1] >> 8);
    send_one_byte_from_adc(data_from_adc[1] >> 4);
    send_one_byte_from_adc(data_from_adc[1]);
    uart_send_data(0x0A);
    ADC_Ready = 0;
    NVIC_EnableIRQ(PORTB_IRQn);
}



static uint16_t adc_read(void)
{
    uint16_t byte1 = 0;
    uint16_t byte2 = 0;
    PORT_SetBits(PORTB, Start_ADC);                                             //Подали команду на Start                                                
    PORT_ResetBits(PORTB, Start_ADC);                                 
    //while(!ADC_Ready); 
    
    PORT_ResetBits(PORTB, A3);                                                  // Начали считывать данные с АЦП
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x33);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        byte1 = SSP_ReceiveData(MDR_SSP0);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x33);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        byte2 = SSP_ReceiveData(MDR_SSP0);
    PORT_SetBits(PORTB, A3);                                                    // Закончили считывать данные с АЦП
    uint16_t actual = ((byte1 << 4) | (byte2 >> 4));
    return actual;
    
}

static void set_U_local(void)
{
    uart_send_confirmation_command('S');
    change_from_HEX((uint8_t *)DAC_data, MODULE_MKAS);                          // Меняем значения для передачи в ЦАП из Аски в Хекс
    PORT_ResetBits(PORTB, A2);                                                  //Выбрали ЦАП в целом      
    for(int j = 0; j < 32; j = j+8)
        {
            send_data_to_DAC(j);        
        }
}

static void choose_ADC_Chanel(uint16_t Chanel)
{
    switch(Chanel)
    {
        case 1:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);                                       
            break;
        case 2:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 3:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 4:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 5:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 6:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 7:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 8:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_ResetBits(PORTB, Sel4);  
            break;
        case 9:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 10:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 11:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 12:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_ResetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 13:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 14:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_ResetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 15:
            PORT_ResetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
        case 16:
            PORT_SetBits(PORTB, Sel1);                                        
            PORT_SetBits(PORTB, Sel2);                                       
            PORT_SetBits(PORTB, Sel3);                                        
            PORT_SetBits(PORTB, Sel4);  
            break;
    }
     delay_mks(50);
}

static inline void choose_DAC(uint16_t number)
{
    switch(number)
    {
        case 0:
            PORT_ResetBits(PORTB, A0);                                          //Выбрали ЦАП DD9 
            PORT_ResetBits(PORTB, A1);                                          //
            break;
        case 8:
            PORT_SetBits(PORTB, A0);                                            //Выбрали ЦАП DD10 
            PORT_ResetBits(PORTB, A1);                                          //
            break;
        case 16:
            PORT_ResetBits(PORTB, A0);                                          //Выбрали ЦАП DD11 
            PORT_SetBits(PORTB, A1);                                            //
            break;
        case 24:
            PORT_SetBits(PORTB, A0);                                            //Выбрали ЦАП DD12
            PORT_SetBits(PORTB, A1);                                            //
            break;
    }
}

static void send_data_to_DAC(uint16_t i)
{
    choose_DAC(i); 
    PORT_ResetBits(PORTB, A3);                                                  // опустили Chip Sellect
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, invert_bit_order((DAC_data[i+1] << 4) | DAC_data[i+2]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, invert_bit_order((DAC_data[i+6] << 4) | DAC_data[i]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, invert_bit_order((DAC_data[i+4] << 4) | DAC_data[i+5]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    PORT_SetBits(PORTB, A3);                                                    // подняли Chip Sellect
}

void MKAS_set_U(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //Выключить прерывание по приему  
    char data;
    int counter = 0;
    for(int i = 0; i < 32; i++)
    {
        while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
        data = UART_ReceiveData(MDR_UART0);
        if(is_HEX_byte(&data))                                                   // Если пришедший байт HEX  
        {
            DAC_data[counter] = data;
            counter ++;   
        }
        else                                                                    // Если байт не HEX
        {
            uart_send_confirmation_command('E');                                //Возвращаем сообщение об ошибке. 
            break;
        }
    }
    
    set_U_local();
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //Включить прерывание по приему
}


void MKAS_get_U(void)
{
    uart_send_confirmation_command('U');
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В 
    //PORTB->SIE=0x80000;                                                         //Установим разрешение прерывания по 19 выводу порта В - означающий готовность данных на АЦП    
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    PORT_SetBits(PORTB, A0);                                                    //Выбрали АЦП 2 - АЦП напряжений.
    
    for(uint8_t i = 1; i <= 16; i++)
    {
        choose_ADC_Chanel(i);                                                       //Выбрали канал 1    
        data_response_ADC(bubble_sort);
    }
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В
}

void MKAS_get_I(void)
{
    uart_send_confirmation_command('I');
    //PORTB->SIE=0x80000;                                                         //Установим разрешение прерывания по 19 выводу порта В - означающий готовность данных на АЦП
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В                                                
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    PORT_ResetBits(PORTB, A0);                                                  //Выбрали АЦП 1 - АЦП токов.
    
    for(uint8_t i = 1; i <= 16; i++)
    {
        choose_ADC_Chanel(i);                                                       //Выбрали канал 1    
        data_response_ADC(bubble_sort);
    }
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В
}

void MKAS_get_Doza(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //Выключить прерывание по приему  
    char I_oder_U = 'X'; 
    char G_oder_D = 'X'; 
    char number_chanel = '5';
    uint16_t result_number = 0;
    

    while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
    I_oder_U = UART_ReceiveData(MDR_UART0);
    while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
    G_oder_D = UART_ReceiveData(MDR_UART0);
    while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
    number_chanel = UART_ReceiveData(MDR_UART0);
    
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //Включить прерывание по приему
    
    
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В                    
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    if (I_oder_U == 'U')
    {
        PORT_SetBits(PORTB, A0);                                                //Выбрали АЦП 2 - АЦП напряжений.
        if (G_oder_D == 'G')
        {
            result_number = (uint16_t)(number_chanel - 0x30 + 1);                                                
        }
        else if(G_oder_D == 'D')
        {
            result_number = (uint16_t)(number_chanel - 0x30 + 6 + 1);                                            
        }
    }
    else if(I_oder_U == 'I')
    {
        PORT_ResetBits(PORTB, A0);                                              //Выбрали АЦП 1 - АЦП токов.
        if (G_oder_D == 'D')
        {
            result_number = (uint16_t)(number_chanel - 0x30 + 1);                                                
        }
        else if(G_oder_D == 'S')
        {
            result_number = (uint16_t)(number_chanel - 0x30 + 6 + 1);                                            
        }
    }

    uart_send_confirmation_command('D');
    
    choose_ADC_Chanel(result_number);                                           //Выбрали канал number_chanel    
    data_response_ADC(bubble_sort);
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В
}




/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKAS.c */
