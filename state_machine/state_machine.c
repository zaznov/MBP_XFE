/**
  ******************************************************************************
  * @file    state_machine.c
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
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
/*-------------------------------------------------Ã ¿—-----------------------*/
        if (strcmp(Gotten_command, MU_command) == 0)
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
        else if (strcmp(Gotten_command, SU_command) == 0) 
        {
            event = EVENT_SU_COMMAND;            
        }
/*-------------------------------------------------Ãƒ —-----------------------*/
        else if (strcmp(Gotten_command, RS_command) == 0)
        {
            event = EVENT_RS_COMMAND;
        }
        else if (strcmp(Gotten_command, MS_command) == 0)
        {
            event = EVENT_MS_COMMAND;
        }
        else if (strcmp(Gotten_command, MT_command) == 0)
        {
            event = EVENT_MT_COMMAND;
        }
        else if (strcmp(Gotten_command, RR_command) == 0)
        {
            event = EVENT_RR_COMMAND;
        }
/*----------------------------------------------------------------------------*/
}

void *state_none()
{   
    switch (event)
    {
/*-------------------------------------------------THE REST-------------------*/        
        case EVENT_NONE:        return state_none;
/*-------------------------------------------------Ã ¿—-----------------------*/
        case EVENT_SU_COMMAND:  return state_setting_u;
        case EVENT_MU_COMMAND:  return state_mesuring_u;
        case EVENT_MI_COMMAND:  return state_mesuring_i;
        case EVENT_MD_COMMAND:  return state_mesuring_d;
/*-------------------------------------------------Ãƒ —-----------------------*/
        case EVENT_RS_COMMAND:  return state_reboot_MKDS;
        case EVENT_MS_COMMAND:  return state_start_MKDS;
        case EVENT_MT_COMMAND:  return state_mesuring_tzch_MKDS;
        case EVENT_RR_COMMAND:  return state_read_results_MKDS;
/*----------------------------------------------------------------------------*/
        default: return state_none;
    }
}

/*-------------------------------------------------Ãƒ —-----------------------*/
void *state_reboot_MKDS()
{  
    spi_init_MKDS();
    reboot_MKDS();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}
void *state_start_MKDS()
{   
    spi_init_MKDS();
    start_MKDS();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}
void *state_mesuring_tzch_MKDS()
{  
    spi_init_MKDS();
    mesuring_MKDS();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}
void *state_read_results_MKDS()
{   
    spi_init_MKDS();
    read_results_MKDS();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}
/*-------------------------------------------------Ã ¿—-----------------------*/
void *state_setting_u()
{   
    spi_init_MKAS();
    set_U();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_u()
{   
    spi_init_MKAS();
    get_U();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_i()
{    
    spi_init_MKAS();
    get_I();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}

void *state_mesuring_d()
{   
    spi_init_MKAS();
    get_Doza();
    Gotten_command[2] = '0';
    event = EVENT_NONE;
    return state_none;
}




/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.c */
