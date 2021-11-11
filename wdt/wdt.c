/**
  ******************************************************************************
  * @file    wdt.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    28/08/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Watch dog Timer driver of Milandr 1986VE8T:           
  *           + initialisation
  *           + rewriting
  *           + reinitialisation
  ******************************************************************************
  * FILE wdt.c
  */
/* Includes ------------------------------------------------------------------*/
#include "wdt.h"
#include "MDR1986VE8T.h"
/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/



/**
  * @brief  Watch dog Timer initialisation.
  * @param  None
  * @retval None
  */
void wdt_init(void)
{
    WDT->KEY = 0x05555;
    while(WDT->KEY != 0x05555);
    WDT->PRL = 0x60000;
    while(WDT->PRL != 0x60000);
    WDT->KEY = 0x0AAAA;
    
    while(WDT->CNT != WDT->PRL);
    WDT->KEY = 0x0CCCC;
    while(WDT->KEY != 0x0CCCC);
    WDT->EN = 0x03333;
}

/**
  * @brief  Watch dog Timer rewriting.
  * @param  None
  * @retval None
  */
void wdt_rewrite(void)
{
    if(0x01 < WDT->CNT && WDT->CNT < 0x60000){
        WDT->KEY = 0x0AAAA;
    }
}

/**
  * @brief  Watch dog Timer reinitialisation.
  * @param  None
  * @retval None
  */
void wdt_reinit(void)
{
    WDT->KEY = 0x0CCCC;
    while(WDT->KEY != 0x0CCCC);
    WDT->EN = 0x01111;
    
    WDT->KEY = 0x05555;
    while(WDT->KEY != 0x05555);
    WDT->PRL = 0x60000;
    while(WDT->PRL != 0x60000);
    WDT->KEY = 0x0AAAA;
    
    while(WDT->CNT != WDT->PRL);
    WDT->KEY = 0x0CCCC;
    while(WDT->KEY != 0x0CCCC);
    WDT->EN = 0x03333;
}
/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE wdt.c */
