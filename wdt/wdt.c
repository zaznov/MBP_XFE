/**
  ******************************************************************************
  * @file    wdt.c
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    12/02/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Template-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE wdt.c
  */
/* Includes ------------------------------------------------------------------*/
//#include "wdt.h"
#include "MDR1986VE8T.h"
/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/




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

void wdt_rewrite(void)
{
    if(0x01 < WDT->CNT && WDT->CNT < 0x60000){
        WDT->KEY = 0x0AAAA;
    }
}

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
/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE wdt.c */
