/**
  ******************************************************************************
  * @file    MKAS.c
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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
static char DAC_data[32] = {0};



/* Functions -----------------------------------------------------------------*/
static inline void Choose_ADC_Chanel(uint16_t Chanel);
static inline void Choose_DAC(uint16_t number);
static inline void send_data_to_DAC(uint16_t i);
static int  Invert(uint8_t x);
/* ---------------------------------------------------------------------------*/

void send_one_byte_from_adc(int data)
{ 
    switch(data&0x0F)
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
        } 
}

void data_response_ADC()
{
    uint16_t data_from_adc[5];
    for(int i = 0; i < 5; i++)
    {
        data_from_adc[i] = adc_read();
    }
    bubble_sort(data_from_adc);
    
    
    send_one_byte_from_adc(data_from_adc[3] >> 8);
    send_one_byte_from_adc(data_from_adc[3] >> 4);
    send_one_byte_from_adc(data_from_adc[3]);
    uart_send_data(0x0A);
    ADC_Ready = 0;
    NVIC_EnableIRQ(PORTB_IRQn);
}

static void swap(uint16_t* a, uint16_t* b)
{
    uint16_t tmp = *a;
    *a = *b;
    *b = tmp;
}
static void bubble_sort(uint16_t* my_arry)
{
    uint8_t lenth = 5;
    bool not_sorted = true;
    
    while(not_sorted)
    {
        not_sorted = false;
        for(uint8_t i = 0; i < lenth-1; i++)
        {
            if(my_arry[i] > my_arry[i+1])
            {
                swap(&my_arry[i], &my_arry[i+1]);
                not_sorted = true;
            }
        }
        lenth--;
    }
}

static uint16_t adc_read(void)
{
    uint16_t byte1 = 0;
    uint16_t byte2 = 0;
    PORT_SetBits(PORTB, Start_ADC);                                             //������ ������� �� Start                                                
    PORT_ResetBits(PORTB, Start_ADC);                                 
    //while(!ADC_Ready); 
    
    PORT_ResetBits(PORTB, A3);                                                  // ������ ��������� ������ � ���
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x33);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        byte1 = SSP_ReceiveData(MDR_SSP0);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_TFE) == RESET);
        SSP_SendData(MDR_SSP0, 0x33);
        while (SSP_GetFlagStatus(MDR_SSP0, SSP_FLAG_RNE) == RESET);
        byte2 = SSP_ReceiveData(MDR_SSP0);
    PORT_SetBits(PORTB, A3);                                                    // ��������� ��������� ������ � ���
    uint16_t actual = ((byte1 << 4) | (byte2 >> 4));
    return actual;
    
}

void set_U_local(void)
{
    uart_send_confirmation_command('S');
    for(int i = 0; i < 32; i++)
        {
            DAC_data[i] = change_into_HEX(DAC_data[i]);                        // ������ �������� ��� �������� � ��� �� ���� � ����
        }
    
    PORT_ResetBits(PORTB, A2);                                                  //������� ��� � �����      
    for(int j = 0; j < 32; j = j+8)
        {
            send_data_to_DAC(j);        
        }
}

void set_U(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //��������� ���������� �� ������  
    char data;
    int counter = 0;
    for(int i = 0; i < 32; i++)
    {
        while (UART_GetFlagStatus (MDR_UART0, UART_FLAG_RXFF)!= SET)
            ;
        data = UART_ReceiveData(MDR_UART0);
        if(is_HEX_byte(data))                                                   // ���� ��������� ���� HEX  
        {
            DAC_data[counter] = data;
            counter ++;   
        }
        else                                                                    // ���� ���� �� HEX
        {
            uart_send_confirmation_command('E');                                //���������� ��������� �� ������. 
            break;
        }
    }
    
    set_U_local();
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //�������� ���������� �� ������
}


void get_U(void)
{
    uart_send_confirmation_command('U');
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // �������� ���������� �� ����� � 
    //PORTB->SIE=0x80000;                                                         //��������� ���������� ���������� �� 19 ������ ����� � - ���������� ���������� ������ �� ���    
    PORT_SetBits(PORTB, A2);                                                    //������� ��� � �����      
    PORT_SetBits(PORTB, A0);                                                    //������� ��� 2 - ��� ����������.
    
    for(uint8_t i = 1; i <= 16; i++)
    {
        Choose_ADC_Chanel(i);                                                       //������� ����� 1    
        data_response_ADC();
    }
    NVIC_DisableIRQ(PORTB_IRQn);                                                // ��������� ���������� �� ����� �
}

void get_I(void)
{
    uart_send_confirmation_command('I');
    //PORTB->SIE=0x80000;                                                         //��������� ���������� ���������� �� 19 ������ ����� � - ���������� ���������� ������ �� ���
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // �������� ���������� �� ����� �                                                
    PORT_SetBits(PORTB, A2);                                                    //������� ��� � �����      
    PORT_ResetBits(PORTB, A0);                                                  //������� ��� 1 - ��� �����.
    
    for(uint8_t i = 1; i <= 16; i++)
    {
        Choose_ADC_Chanel(i);                                                       //������� ����� 1    
        data_response_ADC();
    }
    NVIC_DisableIRQ(PORTB_IRQn);                                                // ��������� ���������� �� ����� �
}

void get_Doza(void)
{
    UART_ITConfig(MDR_UART0, UART_IT_RX, DISABLE);                              //��������� ���������� �� ������  
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
    
    UART_ITConfig(MDR_UART0, UART_IT_RX, ENABLE);                               //�������� ���������� �� ������
    
    
    NVIC_EnableIRQ(PORTB_IRQn);                                                 // �������� ���������� �� ����� �                    
    PORT_SetBits(PORTB, A2);                                                    //������� ��� � �����      
    if (I_oder_U == 'U')
    {
        PORT_SetBits(PORTB, A0);                                                //������� ��� 2 - ��� ����������.
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
        PORT_ResetBits(PORTB, A0);                                              //������� ��� 1 - ��� �����.
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
    
    Choose_ADC_Chanel(result_number);                                           //������� ����� number_chanel    
    data_response_ADC();
    NVIC_DisableIRQ(PORTB_IRQn);                                                // ��������� ���������� �� ����� �
}


uint8_t change_into_HEX(uint8_t data)
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

static inline void Choose_ADC_Chanel(uint16_t Chanel)
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
    //for(int i = 0; i < 400; i++);
}

static inline void Choose_DAC(uint16_t number)
{
    switch(number)
    {
        case 0:
            PORT_ResetBits(PORTB, A0);                                          //������� ��� DD9 
            PORT_ResetBits(PORTB, A1);                                          //
            break;
        case 8:
            PORT_SetBits(PORTB, A0);                                            //������� ��� DD10 
            PORT_ResetBits(PORTB, A1);                                          //
            break;
        case 16:
            PORT_ResetBits(PORTB, A0);                                          //������� ��� DD11 
            PORT_SetBits(PORTB, A1);                                            //
            break;
        case 24:
            PORT_SetBits(PORTB, A0);                                            //������� ��� DD12
            PORT_SetBits(PORTB, A1);                                            //
            break;
    }
}

static inline void send_data_to_DAC(uint16_t i)
{
    Choose_DAC(i); 
    PORT_ResetBits(PORTB, A3);                                                  // �������� Chip Sellect
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
    PORT_SetBits(PORTB, A3);                                                    // ������� Chip Sellect
}


static int Invert(uint8_t x) 
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
