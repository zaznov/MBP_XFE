/**
  ******************************************************************************
  * @file    state_machine.h
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE state_machine.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_

/* Includes ------------------------------------------------------------------*/ 
#include "MKAS.h"
#include "MKDS.h"
#include "spi.h"
#include "mdr32f8_it.h"                 // Keil.EVAL-BOARD1986VE8TSupport::Board Support:Board Setup
#include <string.h>

/* Defines -------------------------------------------------------------------*/   

typedef enum{
    STATE_NONE,
    STATE_REBOOT_XFE,
    STATE_SETTING_U,
    STATE_MESURING_U,
    STATE_MESURING_I,
    STATE_MESURING_D,
    STATE_MESURING_T,
}STATE_t;

typedef enum{
/*-------------------------------------------------МКАС-----------------------*/
    EVENT_SU_COMMAND,
    EVENT_MU_COMMAND,
    EVENT_MI_COMMAND,
    EVENT_MD_COMMAND,
/*-------------------------------------------------МКДС-----------------------*/
    EVENT_RS_COMMAND,
    EVENT_MS_COMMAND,
    EVENT_MT_COMMAND,
    EVENT_RR_COMMAND,
/*-------------------------------------------------THE REST-------------------*/
    EVENT_NONE,
}EVENT_t;

typedef void* (*STATE_FUNC_PTR_t)(void);

/* Functions -----------------------------------------------------------------*/
void change_event(void);
/*-------------------------------------------------THE REST-------------------*/
void *state_none(void);
/*-------------------------------------------------МКАС-----------------------*/
void *state_setting_u(void);
void *state_mesuring_u(void);
void *state_mesuring_i(void);
void *state_mesuring_d(void);
/*-------------------------------------------------МКДС-----------------------*/
void *state_reboot_MKDS(void);
void *state_start_MKDS(void);
void *state_mesuring_tzch_MKDS(void);
void *state_read_results_MKDS(void);

/* Variables -----------------------------------------------------------------*/
// оставляем RS_command[4] под 0x00 (нуль-терминатора) для сравнения в strcmp
/*-------------------------------------------------МКАС-----------------------*/
static char MU_command[4] = "MU\n";                                                     
static char MI_command[4] = "MI\n";                                                    
static char MD_command[4] = "MD\n";                                                                                                    
static char SU_command[4] = "SU\n";  
/*-------------------------------------------------МКДС-----------------------*/
static char RS_command[4] = "RS\n";  
static char MS_command[4] = "MS\n";  
static char MT_command[4] = "MT\n";  
static char RR_command[4] = "RR\n";  
/*-------------------------------------------------THE REST-------------------*/
extern char Gotten_command[4];     




#endif /* __STATE_MACHINE_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.h */

