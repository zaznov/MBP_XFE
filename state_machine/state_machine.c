/**
  ******************************************************************************
  * @file    state_machine.c
  * @author  Zaznov NIIKP
  * @version V3.0.1
  * @date    28/08/2021
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities (states) of the state_machine-driver:   
  *                         Analog signals
  *           + STATE Measuring 16 Voltage Chanals of MKAS module
  *           + STATE Measuring 16 Current Chanals of MKAS module
  *           + STATE Setting   8  Voltage Chanals of MKAS module
  *           + STATE Measuring RADFETs parametrs  of MKAS module
  *                         Digital signals
  *           + STATE Reboot of MKDS module
  *           + STATE Start - Setting RAM on AKI module with predetermined 
  *               values through MKDS module
  *           + STATE Measuring TZCH - Heavy charged particles control of AKI module
  *           + STATE Reading results of TZCH Measuring (state_read_results_MKDS)
  *           + STATE Seting MKDS1 with certain values
  *           + STATE Seting MKDS2 with certain values
  *           + STATE Seting MKDS1 in READ mode
  *           + STATE Seting MKDS1 in WRITE mode
  *           + STATE Seting MKDS2 in READ mode
  *           + STATE Seting MKDS2 in WRITE mode
  *           + STATE None - waiting for a command state
  
  ******************************************************************************
  * FILE state_machine.c
  */
/* Includes ------------------------------------------------------------------*/
#include "state_machine.h"
#include "logic.h"
/* Defines -------------------------------------------------------------------*/ 

/* Functions -----------------------------------------------------------------*/
/*-------------------------------------------------����-----------------------*/
static void *state_setting_u(void);
static void *state_mesuring_u(void);
static void *state_mesuring_i(void);
static void *state_mesuring_d(void);
/*-------------------------------------------------���� 1---------------------*/
static void *state_reboot_MKDS(void);
static void *state_start_MKDS(void);
static void *state_mesuring_tzch_MKDS(void);
static void *state_read_results_MKDS(void);
/*-------------------------------------------------���� 2---------------------*/
static void *state_data_to_MKDS(void);
static void *state_set_MKDS_read(void);
static void *state_set_MKDS_write(void);
static void *state_data_from_MKDS(void);
/*-------------------------------------------------���------------------------*/
static void *state_get_status(void);
/*-------------------------------------------------THE REST-------------------*/
static void *state_none(void);

/* Variables -----------------------------------------------------------------*/
volatile STATE_FUNC_PTR_t state_T = state_none;
volatile EVENT_t event = EVENT_NONE;
extern char Gotten_command[4];

// ��������� RS_command[4] ��� 0x00 (����-�����������) ��� ��������� � strcmp
/*-------------------------------------------------����-----------------------*/
static char MU_command[4] = "MU\n";                                                     
static char MI_command[4] = "MI\n";                                                    
static char MD_command[4] = "MD\n";                                                                                                    
static char SU_command[4] = "SU\n";  
/*-------------------------------------------------����-----------------------*/
static char RS_command[4] = "RS\n";  
static char MS_command[4] = "MS\n";  
static char MT_command[4] = "MT\n";  
static char RR_command[4] = "RR\n";  

static char D1_command[4] = "D1\n";  
static char D2_command[4] = "D2\n";  
static char R1_command[4] = "R1\n";  
static char W1_command[4] = "W1\n"; 
static char R2_command[4] = "R2\n";  
static char W2_command[4] = "W2\n";
static char G1_command[4] = "G1\n";  
static char G2_command[4] = "G2\n";
/*-------------------------------------------------���-----------------------*/
static char ST_command[4] = "ST\n";  



/**
  * @brief  This function is called from INT_UART0_Handler, aftr the whole command 
  * was recieved from the UART. The function changes the variable event, 
  * what leads to changing of the state of the device in function state_none().
  * @param  None
  * @retval None
  */
void change_event()
{
/*-------------------------------------------------����-----------------------*/
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
/*-------------------------------------------------����-----------------------*/
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
/*-------------------------------------------------���� MODE 2----------------*/        
        else if (strcmp(Gotten_command, D1_command) == 0)
        {
            event = EVENT_D1_COMMAND;
        }
        else if (strcmp(Gotten_command, D2_command) == 0)
        {
            event = EVENT_D2_COMMAND;
        }
        else if (strcmp(Gotten_command, R1_command) == 0)
        {
            event = EVENT_R1_COMMAND;
        }
        else if (strcmp(Gotten_command, W1_command) == 0)
        {
            event = EVENT_W1_COMMAND;
        }
        else if (strcmp(Gotten_command, R2_command) == 0)
        {
            event = EVENT_R2_COMMAND;
        }
        else if (strcmp(Gotten_command, W2_command) == 0)
        {
            event = EVENT_W2_COMMAND;
        }
        else if (strcmp(Gotten_command, G1_command) == 0)
        {
            event = EVENT_G1_COMMAND;
        }
        else if (strcmp(Gotten_command, G2_command) == 0)
        {
            event = EVENT_G2_COMMAND;
        }
/*-------------------------------------------------���------------------------*/       
        else if (strcmp(Gotten_command, ST_command) == 0)
        {
            event = EVENT_ST_COMMAND;
        }
/*----------------------------------------------------------------------------*/
        else
        {
            event = EVENT_NONE;
        }
}



/**
  * @brief  This function is called most of the time in main infinite loop, 
  * wating for the variable event to be changed, and then returns pointer on 
  * necessary for execution function.
  * @param  None
  * @retval Returns pointer on void value. 
  *         This parameter can be one of the following values:
  *           @state state_none  
  *           @state state_setting_u
  *           @state state_mesuring_u  
  *           @state state_mesuring_i
  *           @state state_mesuring_d  
  *           @state state_reboot_MKDS
  *           @state state_start_MKDS  
  *           @state state_mesuring_tzch_MKDS
  *           @state state_read_results_MKDS  
  *           @state state_data_to_MKDS
  *           @state state_set_MKDS_read
  *           @state state_set_MKDS_write
  *           @state state_data_from_MKDS
  *           @state state_get_status
  */
void *state_none()
{   
    switch (event)
    {
/*-------------------------------------------------THE REST-------------------*/        
        case EVENT_NONE:        return state_none;
/*-------------------------------------------------����-----------------------*/
        case EVENT_SU_COMMAND:  return state_setting_u;
        case EVENT_MU_COMMAND:  return state_mesuring_u;
        case EVENT_MI_COMMAND:  return state_mesuring_i;
        case EVENT_MD_COMMAND:  return state_mesuring_d;
/*-------------------------------------------------���� 1---------------------*/
        case EVENT_RS_COMMAND:  return state_reboot_MKDS;
        case EVENT_MS_COMMAND:  return state_start_MKDS;
        case EVENT_MT_COMMAND:  return state_mesuring_tzch_MKDS;
        case EVENT_RR_COMMAND:  return state_read_results_MKDS;
/*-------------------------------------------------���� 2---------------------*/      
        case EVENT_D1_COMMAND:  return state_data_to_MKDS;
        case EVENT_D2_COMMAND:  return state_data_to_MKDS;
        
        case EVENT_R1_COMMAND:  return state_set_MKDS_read;
        case EVENT_R2_COMMAND:  return state_set_MKDS_read;
        
        case EVENT_W1_COMMAND:  return state_set_MKDS_write;
        case EVENT_W2_COMMAND:  return state_set_MKDS_write;
        
        case EVENT_G1_COMMAND:  return state_data_from_MKDS;
        case EVENT_G2_COMMAND:  return state_data_from_MKDS;
/*-------------------------------------------------���------------------------*/      
        case EVENT_ST_COMMAND:  return state_get_status;
/*----------------------------------------------------------------------------*/
        default: return state_none;
    }
}

/*-------------------------------------------------���� 1---------------------*/
static void *state_reboot_MKDS()
{  
    spi_reinit(MODULE_MKDS_1);
    MKDS_reboot();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_start_MKDS()
{   
    spi_reinit(MODULE_MKDS_1);
    MKDS_start();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_mesuring_tzch_MKDS()
{  
    spi_reinit(MODULE_MKDS_1);
    MKDS_mesuring();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_read_results_MKDS()
{   
    spi_reinit(MODULE_MKDS_1);
    MKDS_read_results();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

/*-------------------------------------------------���� 2---------------------*/
static void *state_data_to_MKDS()
{  
    MODULE CURRENT_MODULE = (event == EVENT_D1_COMMAND) ? MODULE_MKDS_1 : MODULE_MKDS_2;
    spi_reinit(CURRENT_MODULE);
    MKDS_data_to_MKDS(CURRENT_MODULE);
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_set_MKDS_read()
{  
    MODULE CURRENT_MODULE = (event == EVENT_R1_COMMAND) ? MODULE_MKDS_1 : MODULE_MKDS_2;
    spi_reinit(CURRENT_MODULE);
    MKDS_set_MKDS_read(CURRENT_MODULE);
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_set_MKDS_write()
{  
    MODULE CURRENT_MODULE = (event == EVENT_W1_COMMAND) ? MODULE_MKDS_1 : MODULE_MKDS_2;
    spi_reinit(CURRENT_MODULE);
    MKDS_set_MKDS_write(CURRENT_MODULE);
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_data_from_MKDS()
{  
    MODULE CURRENT_MODULE = (event == EVENT_G1_COMMAND) ? MODULE_MKDS_1 : MODULE_MKDS_2;
    spi_reinit(CURRENT_MODULE);
    MKDS_data_from_MKDS(CURRENT_MODULE);
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

/*-------------------------------------------------����-----------------------*/
static void *state_setting_u()
{   
    spi_reinit(MODULE_MKAS);
    MKAS_set_U();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_u()
{   
    spi_reinit(MODULE_MKAS);
    MKAS_get_U();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_i()
{    
    spi_reinit(MODULE_MKAS);
    MKAS_get_I();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_d()
{   
    spi_reinit(MODULE_MKAS);
    MKAS_get_single_measure();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}


/*-------------------------------------------------���-----------------------*/
static void *state_get_status()
{   
    status_get_status();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}



/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE state_machine.c */
