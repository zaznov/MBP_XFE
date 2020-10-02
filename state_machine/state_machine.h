/**
  ******************************************************************************
  * @file    state_machine.h
  * @author  Zaznov NIIKP
  * @version V1.0.2
  * @date    27/05/2020
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
    EVENT_NONE,
    EVENT_RS_COMMAND,
    EVENT_SU_COMMAND,
    EVENT_MU_COMMAND,
    EVENT_MI_COMMAND,
    EVENT_MD_COMMAND,
    EVENT_MT_COMMAND,
}EVENT_t;

typedef void *(*STATE_FUNC_PTR_t)(void);

/* Functions -----------------------------------------------------------------*/
void change_event(void);

void *state_none(void);
void *state_reboot(void);
void *state_setting_u(void);
void *state_mesuring_u(void);
void *state_mesuring_i(void);
void *state_mesuring_d(void);
void *state_mesuring_t(void);

/* Variables -----------------------------------------------------------------*/
static char RS_command[4] = "RS\n";                                                    // оставляем RS_command[4] под 0x00 (нуль-терминатора) для сравнения в strcmp
static char MU_command[4] = "MU\n";                                                     
static char MI_command[4] = "MI\n";                                                    
static char MD_command[4] = "MD\n";                                                   
static char MT_command[4] = "MT\n";                                                  
static char SU_command[4] = "SU\n";  

extern char Gotten_command[4];     
extern char DAC_data[32];



#endif /* __STATE_MACHINE_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.h */

