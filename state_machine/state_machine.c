/**
  ******************************************************************************
  * @file    state_machine.c
  * @author  Zaznov NIIKP
  * @version V1.0.2
  * @date    27/05/2020
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the state_machine-driver:           
  *           + ...
  *           + ...
  *           + ...
  ******************************************************************************
  * FILE state_machine.c
  */
  
/* Includes ------------------------------------------------------------------*/
#include "state_machine.h"

/* Defines -------------------------------------------------------------------*/ 

/* Variables -----------------------------------------------------------------*/
volatile STATE_FUNC_PTR_t state_T = state_none;
volatile EVENT_t event = EVENT_NONE;

/* Functions -----------------------------------------------------------------*/

void change_event()
{
        if(strcmp(Gotten_command, RS_command) == 0)
        {
            event = EVENT_RS_COMMAND;
        }
        else if (strcmp(Gotten_command, MU_command) == 0)
        {
            event = EVENT_MU_COMMAND;
        }
        else if (strcmp(Gotten_command, MI_command) == 0)
        {
            event = EVENT_MI_COMMAND;
        }
        else if (strcmp(Gotten_command, MD_command) == 0)
        {
            event = EVENT_MD_COMMAND;
        }
        else if (strcmp(Gotten_command, MT_command) == 0)
        {
            event = EVENT_MT_COMMAND;
        }
        else if (strcmp(Gotten_command, SU_command) == 0) 
        {
            event = EVENT_SU_COMMAND;            
        }
}

void *state_none()
{   
    switch (event)
    {
        case EVENT_NONE:        return state_none;
        case EVENT_RS_COMMAND:  return state_reboot;
        case EVENT_SU_COMMAND:  return state_setting_u;
        case EVENT_MU_COMMAND:  return state_mesuring_u;
        case EVENT_MI_COMMAND:  return state_mesuring_i;
        case EVENT_MD_COMMAND:  return state_mesuring_d;
        case EVENT_MT_COMMAND:  return state_mesuring_t;
        default: return state_none;
    }
}

void *state_reboot()
{  
    UART_Send_Confirmation_command('R');
    Gotten_command[2] = '0';
    //NVIC_SystemReset();                                                // —бравымаем €дро контроллера
    //SCB->AIRCR  =0x05FA0004;
    event = EVENT_NONE;
    return state_none;
}

void *state_setting_u()
{   
    Set_U();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_u()
{   
    Get_U();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_i()
{    
    Get_I();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_d()
{   
    Get_Doza();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_t()
{   
    Get_TZCH();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}


/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.c */
