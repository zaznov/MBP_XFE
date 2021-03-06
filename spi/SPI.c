/**
  ******************************************************************************
  * @file    Spi.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
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
void pin_init_spi()
{ 
    KEY_reg_accs();  
    PORT_InitTypeDef GPIO_user_ini;
    GPIO_user_ini.PORT_Pin              = (PORT_Pin_23 | PORT_Pin_26 | PORT_Pin_24/*| PORT_Pin_25*/);
    GPIO_user_ini.PORT_SFUNC            = PORT_SFUNC_6;
    GPIO_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTB, &GPIO_user_ini); 
}


void spi_init()
{
    pin_init_spi();                                               
    
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_SSP0_EN, ENABLE);                  // ���. ������������ ����������� SPI
    SSP_CLK_en(SSP_CLKSRC_MAX_CLK, SSP_CLKdiv1);                                // ��������� ������������ ����������� SPI
    
    SSP_InitTypeDef SSP_InitStructure; 
    SSP_InitStructure.SSP_SCR                   = 0;                            
    SSP_InitStructure.SSP_CPSDVSR               = 2;                            
    SSP_InitStructure.SSP_Mode                  = SSP_ModeMaster;
    SSP_InitStructure.SSP_WordLength            = SSP_WordLength8b;
    SSP_InitStructure.SSP_SPH                   = SSP_SPH_1Edge;                                
    SSP_InitStructure.SSP_SPO                   = SSP_SPO_Low;
    SSP_InitStructure.SSP_FRF                   = SSP_FRF_SPI_Motorola;
    SSP_InitStructure.SSP_HardwareFlowControl   = SSP_HardwareFlowControl_SSE;
    SSP_Init (MDR_SSP0,&SSP_InitStructure);                                     
    
    SSP_Cmd(MDR_SSP0, ENABLE);                                                  //��������� SPI                         
    //NVIC_EnableIRQ(SSP0_IRQn);                                                //�������� ����� ���������� �� SPI
}

void spi_reinit(MODULE MODULE_NAME)
{ 
    SSP_Cmd(MDR_SSP0, DISABLE);                                                  //���������� SPI
    SSP_DeInit(MDR_SSP0);
    SSP_InitTypeDef SSP_InitStructure; 
    SSP_InitStructure.SSP_SCR                   = 0;                            
    SSP_InitStructure.SSP_CPSDVSR               = 2;                            
    SSP_InitStructure.SSP_Mode                  = SSP_ModeMaster;
    SSP_InitStructure.SSP_WordLength            = SSP_WordLength8b;
    SSP_InitStructure.SSP_SPH                   = (MODULE_NAME == MODULE_MKAS) ? SSP_SPH_2Edge : SSP_SPH_1Edge;                               
    SSP_InitStructure.SSP_SPO                   = SSP_SPO_Low;
    SSP_InitStructure.SSP_FRF                   = SSP_FRF_SPI_Motorola;
    SSP_InitStructure.SSP_HardwareFlowControl   = SSP_HardwareFlowControl_SSE;
    SSP_Init (MDR_SSP0,&SSP_InitStructure);                                     
    SSP_Cmd(MDR_SSP0, ENABLE);                                                  //��������� SPI
}
/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE Spi.c */
