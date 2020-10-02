/**
  ******************************************************************************
  * @file    Spi.c
  * @author  Zaznov NIIKP
  * @version V1.0.3
  * @date    21/06/2020
  * @brief   This file provides all the functions prototypes for work with SPI 
             from XFE project
  ******************************************************************************
  * FILE Spi.c
  */

/* Includes ------------------------------------------------------------------*/

/* My Includes ---------------------------------------------------------------*/
#include "Spi.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void Pin_init_spi()
{ 

    KEY_reg_accs();  
    PORT_InitTypeDef GPIO_user_ini;
    GPIO_user_ini.PORT_Pin              = (PORT_Pin_23 | PORT_Pin_26 | PORT_Pin_24 | PORT_Pin_25);
    GPIO_user_ini.PORT_SFUNC            = PORT_SFUNC_6;
    GPIO_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTB, &GPIO_user_ini); 
    
}



void spi_init()
{
    Pin_init_spi();                                               
    
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_SSP0_EN, ENABLE);                  // Вкл. тактирование контроллера SPI
    SSP_CLK_en(SSP_CLKSRC_MAX_CLK, SSP_CLKdiv1);                                // Настройка тактирование контроллера SPI
    
    SSP_InitTypeDef SSP_InitStructure; 
    SSP_InitStructure.SSP_SCR                   = 0;                            //был 0x10
    SSP_InitStructure.SSP_CPSDVSR               = 2;                            //был 20
    SSP_InitStructure.SSP_Mode                  = SSP_ModeMaster;
    SSP_InitStructure.SSP_WordLength            = SSP_WordLength8b;
    SSP_InitStructure.SSP_SPH                   = SSP_SPH_2Edge;
    SSP_InitStructure.SSP_SPO                   = SSP_SPO_Low;
    SSP_InitStructure.SSP_FRF                   = SSP_FRF_SPI_Motorola;
    SSP_InitStructure.SSP_HardwareFlowControl   = SSP_HardwareFlowControl_SSE;
    SSP_Init (MDR_SSP0,&SSP_InitStructure);                                     
    
    SSP_Cmd(MDR_SSP0, ENABLE);                                                  //Включение SPI                         
    //NVIC_EnableIRQ(SSP0_IRQn);                                                  //Включить общее прерывание по SPI
}




/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Spi.c */
