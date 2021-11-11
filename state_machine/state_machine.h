/**
  ******************************************************************************
  * @file    state_machine.h
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file contains all the functions prototypes for work with 
  *          state_machine-driver:
  *          + change_event
  *          + enum for EVENT_t type
  ******************************************************************************
  * FILE state_machine.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATE_MACHINE_H_
#define __STATE_MACHINE_H_

/* Includes ------------------------------------------------------------------*/ 
#include "MKAS.h"
#include "MKDS.h"
#include "status.h"
#include "spi.h"
#include "mdr32f8_it.h"                                                         
#include <string.h>

/* Defines -------------------------------------------------------------------*/   
typedef enum{
/*-------------------------------------------------Ã ¿—-----------------------*/
    EVENT_SU_COMMAND,
    EVENT_MU_COMMAND,
    EVENT_MI_COMMAND,
    EVENT_MD_COMMAND,
/*-------------------------------------------------Ã ƒ—-----------------------*/
    EVENT_RS_COMMAND,
    EVENT_MS_COMMAND,
    EVENT_MT_COMMAND,
    EVENT_RR_COMMAND,
    
    EVENT_D1_COMMAND,
    EVENT_D2_COMMAND,
    EVENT_R1_COMMAND,
    EVENT_W1_COMMAND,
    EVENT_R2_COMMAND,
    EVENT_W2_COMMAND,
    EVENT_G1_COMMAND,
    EVENT_G2_COMMAND,
/*-------------------------------------------------Ã”»------------------------*/
    EVENT_ST_COMMAND,
/*-------------------------------------------------THE REST-------------------*/
    EVENT_NONE,
}EVENT_t;

typedef void* (*STATE_FUNC_PTR_t)(void);

/* Functions -----------------------------------------------------------------*/
void change_event(void);

/* Variables -----------------------------------------------------------------*/

#endif /* __STATE_MACHINE_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.h */

