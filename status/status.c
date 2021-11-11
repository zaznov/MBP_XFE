/**
  ******************************************************************************
  * @file    status.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    26/05/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Template-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE status.c
  */
/* Includes ------------------------------------------------------------------*/
#include "status.h"
#include "logic.h"
#include "Uart.h"
#include <stdio.h>
/* Defines -------------------------------------------------------------------*/ 


/* Variables -----------------------------------------------------------------*/
CLK         CURRENT_CLK         = HSE0_ON;
CLK_RDY     CURRENT_CLK_RDY     = HSE0_RDY;
CLKCTRL_MAX CURRENT_CLKCTRL_MAX = HSE0div1;
UART_CLK    CURRENT_UART_CLK    = UART_HSE0;
SPI_CLK     CURRENT_SPI_CLK     = SPI_HSE0;

extern status_TypeDef status; 

/* Functions -----------------------------------------------------------------*/
/* -----------------------MAIN CLK--------------------------------------------*/
uint32_t get_CURRENT_CLK(void){
    return CURRENT_CLK;
}
void set_CURRENT_CLK(CLK NEW_CURRENT_CLK){
    CURRENT_CLK = NEW_CURRENT_CLK;
}
/* -----------------------CLK RDY---------------------------------------------*/
uint32_t    get_CURRENT_CLK_RDY(void){
    return CURRENT_CLK_RDY;
}
void set_CURRENT_CLK_RDY(CLK_RDY NEW_CURRENT_CLK_RDY){
    CURRENT_CLK_RDY = NEW_CURRENT_CLK_RDY;
}
/* -----------------------CLK MAX---------------------------------------------*/
uint32_t    get_CURRENT_CLKCTRL_MAX(void){
    return CURRENT_CLKCTRL_MAX;
}
void set_CURRENT_CLKCTRL_MAX(CLKCTRL_MAX NEW_CURRENT_CLKCTRL_MAX){
    CURRENT_CLKCTRL_MAX = NEW_CURRENT_CLKCTRL_MAX;
}
/* -----------------------CLK UART--------------------------------------------*/
uint32_t    get_CURRENT_UART_CLK(void){
    return CURRENT_UART_CLK;
}
void set_CURRENT_UART_CLK(UART_CLK NEW_CURRENT_UART_CLK){
    CURRENT_UART_CLK = NEW_CURRENT_UART_CLK;
}
/* -----------------------CLK SPI---------------------------------------------*/
uint32_t    get_CURRENT_SPI_CLK(void){
    return CURRENT_SPI_CLK;
}
void set_CURRENT_SPI_CLK(SPI_CLK NEW_CURRENT_SPI_CLK){
    CURRENT_SPI_CLK = NEW_CURRENT_SPI_CLK;
}
/* ---------------------------------------------------------------------------*/
void status_init(status_TypeDef* status_struct)
{
    status_struct->MUI.main_clk_sourse  = 0x0;
    status_struct->MUI.spi_clk_sourse   = 0x0;
    status_struct->MUI.uart_clk_sourse  = 0x0;
}

void status_update_status(void)
{
    if      (CURRENT_CLK == HSE0_ON) status.MUI.main_clk_sourse = 0x01;
    else if (CURRENT_CLK == HSE1_ON) status.MUI.main_clk_sourse = 0x02;
    else                             status.MUI.main_clk_sourse = 0x03;
    
    if      (CURRENT_UART_CLK == UART_HSE0) status.MUI.uart_clk_sourse = 0x01;
    else if (CURRENT_UART_CLK == UART_HSE1) status.MUI.uart_clk_sourse = 0x02;
    else                                    status.MUI.uart_clk_sourse = 0x03;
    
    if      (CURRENT_SPI_CLK == SPI_HSE0)   status.MUI.spi_clk_sourse = 0x01;
    else if (CURRENT_SPI_CLK == SPI_HSE1)   status.MUI.spi_clk_sourse = 0x02;
    else                                    status.MUI.spi_clk_sourse = 0x03;
    

}

void status_get_status(void)
{
    
    
    uart_send_confirmation_command('A');
    status_update_status();
    uart_send_data(*(uint8_t*)&status.MUI);
    uart_send_data(0x0A);
}
/************************* 2021 Zaznov NIIKP ***********************************
*
* END OF FILE status.c */
