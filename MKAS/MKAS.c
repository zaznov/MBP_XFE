/**
  ******************************************************************************
  * @file    MKAS.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    14/06/2020
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

/* Variables -----------------------------------------------------------------*/
volatile _Bool ADC_Ready = 0;
char DAC_data[32];

/* Functions -----------------------------------------------------------------*/
void MKAS_init(void)
{
    for(int i = 0; i < 32; i++)
    {
        DAC_data[i] = 0;                         
    }
}

void send_one_byte_from_adc(int data)
{ 
    switch(data&0x0F)
        {
            case(0):
                UART_SendData_fixed(0x30);
                break;
            case(1):
                UART_SendData_fixed(0x31);
                break;
            case(2):
                UART_SendData_fixed(0x32);
                break;
            case(3):
                UART_SendData_fixed(0x33);
                break;
            case(4):
                UART_SendData_fixed(0x34);
                break;
            case(5):
                UART_SendData_fixed(0x35);
                break;
            case(6):
                UART_SendData_fixed(0x36);
                break;
            case(7):
                UART_SendData_fixed(0x37);
                break;
            case(8):
                UART_SendData_fixed(0x38);
                break;
            case(9):
                UART_SendData_fixed(0x39);
                break;
            case(0x0A):
                UART_SendData_fixed(0x41);
                break;
            case(0x0B):
                UART_SendData_fixed(0x42);
                break;
            case(0x0C):
                UART_SendData_fixed(0x43);
                break;
            case(0x0D):
                UART_SendData_fixed(0x44);
                break;
            case(0x0E):
                UART_SendData_fixed(0x45);
                break;
            case(0x0F):
                UART_SendData_fixed(0x46);
                break;          
        } 
}

void Data_Response_ADC()
{
    uint16_t byte1 = 0;
    uint16_t byte2 = 0;
    PORT_SetBits(PORTB, Start_ADC);                                             //Подали команду на Start 
    //Delay1(10);                                                         
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
    send_one_byte_from_adc(actual >> 8);
    send_one_byte_from_adc(actual >> 4);
    send_one_byte_from_adc(actual);
    UART_SendData_fixed(0x0A);
    ADC_Ready = 0;
    NVIC_EnableIRQ(PORTB_IRQn);
}


void Set_U_local(void)
{
    UART_Send_Confirmation_command('S');
    for(int i = 0; i < 32; i++)
        {
            DAC_data[i] = Change_into_HEX_(DAC_data[i]);                        // Меняем значения для передачи в ЦАП из Аски в Хекс
        }
    
    PORT_ResetBits(PORTB, A2);                                                  //Выбрали ЦАП в целом      
    for(int j = 0; j < 32; j = j+8)
        {
            Choose_DAC(j);    
            send_data_to_DAC(j);        
        }
}

void Set_U(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //Выключить прерывание по приему  
    char data;
    int counter = 0;
    for(int i = 0; i < 32; i++)
    {
        while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
        data = UART_ReceiveData(MDR_UART0);
        if(Is_HEX_Byte(data))                                                   // Если пришедший байт HEX  
        {
            DAC_data[counter] = data;
            counter ++;   
        }
        else                                                                    // Если байт не HEX
        {
            UART_Send_Confirmation_command('E');                                //Возвращаем сообщение об ошибке. 
            break;
        }
    }
    
    Set_U_local();
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //Включить прерывание по приему
}


void Get_U(void)
{
    UART_Send_Confirmation_command('U');
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В 
    //PORTB->SIE=0x80000;                                                         //Установим разрешение прерывания по 19 выводу порта В - означающий готовность данных на АЦП    
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    PORT_SetBits(PORTB, A0);                                                    //Выбрали АЦП 2 - АЦП напряжений.
    
    Choose_ADC_Chanel(1);                                                       //Выбрали канал 1    
    Data_Response_ADC();
    Choose_ADC_Chanel(2);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(3);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(4);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(5);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(6);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(7);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(8);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(9);                                                        
    Data_Response_ADC();
    Choose_ADC_Chanel(10);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(11);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(12);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(13);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(14);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(15);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(16); 
    //for(int i = 0; i < 10000; i++);
    Data_Response_ADC();
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В

}

void Get_I(void)
{
    UART_Send_Confirmation_command('I');
    //PORTB->SIE=0x80000;                                                         //Установим разрешение прерывания по 19 выводу порта В - означающий готовность данных на АЦП
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В                                                
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    PORT_ResetBits(PORTB, A0);                                                  //Выбрали АЦП 1 - АЦП токов.
    
    Choose_ADC_Chanel(1);                                                       //Выбрали канал 1    
    Data_Response_ADC();
    Choose_ADC_Chanel(2);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(3);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(4);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(5);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(6);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(7);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(8);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(9);                                                         
    Data_Response_ADC();
    Choose_ADC_Chanel(10);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(11);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(12);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(13);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(14);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(15);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(16); 
    for(int i = 0; i < 10000; i++);    
    Data_Response_ADC();
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В
}

void Get_Doza(void)
{
    UART_Send_Confirmation_command('D');
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // Разрешим прерывания на порту В                                                 
    PORT_SetBits(PORTB, A2);                                                    //Выбрали АЦП в целом      
    PORT_SetBits(PORTB, A0);                                                    //Выбрали АЦП 2 - АЦП напряжений.
    
    Choose_ADC_Chanel(1);                                                       //Выбрали канал 1    
    Data_Response_ADC();
    Choose_ADC_Chanel(2);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(3);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(4);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(5);                                                          
    Data_Response_ADC();
    Choose_ADC_Chanel(6);                                                          
    Data_Response_ADC();
    NVIC_DisableIRQ(PORTB_IRQn);                                                // Запрещаем прерывания по порту В
}


uint8_t Change_into_HEX_(uint8_t data)
{
    switch(data)
        {
            case(0x0A):
                return 0x0A;
            case(0x30):
                return 0x00;
            case(0x31):
                return 0x01;
            case(0x32):
                return 0x02;
            case(0x33):
                return 0x03;
            case(0x34):
                return 0x04;
            case(0x35):
                return 0x05;
            case(0x36):
                return 0x06;
            case(0x37):
                return 0x07;
            case(0x38):
                return 0x08;
            case(0x39):
                return 0x09;
            case(0x41):
                return 0x0A;
            case(0x42):
                return 0x0B;
            case(0x43):
                return 0x0C;
            case(0x44):
                return 0x0D;
            case(0x45):
                return 0x0E;
            case(0x46):
                return 0x0F;
        } 
    return 1;    
}

static inline void Choose_ADC_Chanel(int Chanel)
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
    for(int i = 0; i < 10000; i++);

}

static inline void Choose_DAC(int number)
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

static inline void send_data_to_DAC(int i)
{
    PORT_ResetBits(PORTB, A3);                                                  // опустили Chip Sellect
    //Delay1(10);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, Invert((DAC_data[i+1] << 4) | DAC_data[i+2]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, Invert((DAC_data[i+6] << 4) | DAC_data[i]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
    SSP_SendData(MDR_SSP0, Invert((DAC_data[i+4] << 4) | DAC_data[i+5]));
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
    SSP_ReceiveData(MDR_SSP0);
    while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_BSY) == SET);
    PORT_SetBits(PORTB, A3);                                                    // подняли Chip Sellect
}


int Invert(uint8_t x) 
{        
    int base = 256;
    int  res = 0;
    while (x != 0) 
    {
        res += (x & 1) * (base >>= 1);
        x >>= 1;
    }
    return res;
}
/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKAS.c */
