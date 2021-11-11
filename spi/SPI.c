/**
  ******************************************************************************
  * @file    Spi.c
  * @author  Zaznov NIIKP
  * @version V3.0.1
  * @date    20/08/2021
  * @brief   This file provides all the functions realisations for work with SPI 
  * from XFE project:
  *           + SPI pin initialisation
  *           + SPI controller initialisation
  *           + SPI controller reinitialisation
  ******************************************************************************
  * FILE Spi.c
  */

/* Includes ------------------------------------------------------------------*/

/* My Includes ---------------------------------------------------------------*/
#include "Spi.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**
  * @brief      Initialisation of SPI pins
  * @attention  Chip Select(CS) for SPI is done by software
  * @param      None
  * @retval     None
  */
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

/**
  * @brief  Initialisation of SPI peripheral
  * @param  MY_SPI_CLK_SRC: Select the clock source for the SPI peripheral.
  *         This parameter can be one of the following values:
  *           @arg SPI_HSE0
  *           @arg SPI_HSE1
  * @retval None
  */
void spi_init(uint32_t MY_SPI_CLK_SRC)
{
    pin_init_spi();                                               
    
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_SSP0_EN, ENABLE);                  // Enable clocking of SPI controller
    SSP_CLK_en(MY_SPI_CLK_SRC, SSP_CLKdiv1);                                    // Setting SPI clocking
    
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
    
    SSP_Cmd(MDR_SSP0, ENABLE);                                                  //Enable SPI                         
    //NVIC_EnableIRQ(SSP0_IRQn);                                                //Enable SPI interrupts. 
}
/**
  * @brief  Reinitialisation of SPI peripheral.
  * @clarification  It is neccecery, due to the fact, that this one SPI module
  * have to communicate with several micpochips, that have differrent SPI Edge 
  * standarts 
  * @param  MODULE_NAME: Select current MODULE
  *         This parameter can be one of the following values:
  *           @arg MODULE_MKAS: MODULE MKAS contains microchips with 
  * SSP_SPH_2Edge SPI Edge standart 
  *           @arg MODULE_MKDS: MODULE MODULE_MKDS contains microchips with 
  * SSP_SPH_1Edge SPI Edge standart 
  * @retval None
  */
void spi_reinit(MODULE MODULE_NAME)
{ 
    
    SSP_Cmd(MDR_SSP0, DISABLE);                                                 //Disable SPI
    SSP_DeInit(MDR_SSP0);
    SSP_InitTypeDef SSP_InitStructure; 
    SSP_InitStructure.SSP_SCR                   = 0;                            //CLK = (OSC CLK)/(SSP_CPSDVSR*(1+SSP_SCR))
    SSP_InitStructure.SSP_CPSDVSR               = 5;                            
    SSP_InitStructure.SSP_Mode                  = SSP_ModeMaster;
    SSP_InitStructure.SSP_WordLength            = SSP_WordLength8b;
    SSP_InitStructure.SSP_SPH                   = (MODULE_NAME == MODULE_MKAS) ? SSP_SPH_2Edge : SSP_SPH_1Edge;                               
    SSP_InitStructure.SSP_SPO                   = SSP_SPO_Low;
    SSP_InitStructure.SSP_FRF                   = SSP_FRF_SPI_Motorola;
    SSP_InitStructure.SSP_HardwareFlowControl   = SSP_HardwareFlowControl_SSE;
    SSP_Init (MDR_SSP0,&SSP_InitStructure);                                     
    SSP_Cmd(MDR_SSP0, ENABLE);                                                  //Enable SPI
    if(MODULE_NAME == MODULE_MKAS){
        CS_MKDS_1_UP();                                                         //Disable MKDS 1
        CS_MKDS_2_UP();                                                         //Disable MKDS 2
    }
}
/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE Spi.c */
