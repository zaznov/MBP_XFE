/**
  ******************************************************************************
  * @file    state_machine.c
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
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
/*-------------------------------------------------Ã ¿—-----------------------*/
static void *state_setting_u(void);
static void *state_mesuring_u(void);
static void *state_mesuring_i(void);
static void *state_mesuring_d(void);
/*-------------------------------------------------Ã ƒ—-----------------------*/
static void *state_reboot_MKDS(void);
static void *state_start_MKDS(void);
static void *state_mesuring_tzch_MKDS(void);
static void *state_read_results_MKDS(void);

static void *state_data_to_MKDS_1(void);
static void *state_data_to_MKDS_2(void);
static void *state_set_MKDS_1_read(void);
static void *state_set_MKDS_1_write(void);
static void *state_set_MKDS_2_read(void);
static void *state_set_MKDS_2_write(void);
static void *state_data_from_MKDS_1(void);
static void *state_data_from_MKDS_2(void);
/*-------------------------------------------------THE REST-------------------*/
void *state_none(void);

/* Variables -----------------------------------------------------------------*/
volatile STATE_FUNC_PTR_t state_T = state_none;
volatile EVENT_t event = EVENT_NONE;
extern char Gotten_command[4];

// ÓÒÚ‡‚ÎˇÂÏ RS_command[4] ÔÓ‰ 0x00 (ÌÛÎ¸-ÚÂÏËÌ‡ÚÓ‡) ‰Îˇ Ò‡‚ÌÂÌËˇ ‚ strcmp
/*-------------------------------------------------Ã ¿—-----------------------*/
static char MU_command[4] = "MU\n";                                                     
static char MI_command[4] = "MI\n";                                                    
static char MD_command[4] = "MD\n";                                                                                                    
static char SU_command[4] = "SU\n";  
/*-------------------------------------------------Ã ƒ—-----------------------*/
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
/*-------------------------------------------------Ãƒ — MODE 2----------------*/        
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
/*----------------------------------------------------------------------------*/
        else
        {
            event = EVENT_NONE;
        }
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
        
        case EVENT_D1_COMMAND:  return state_data_to_MKDS_1;
        case EVENT_D2_COMMAND:  return state_data_to_MKDS_2;
        case EVENT_R1_COMMAND:  return state_set_MKDS_1_read;
        case EVENT_W1_COMMAND:  return state_set_MKDS_1_write;
        case EVENT_R2_COMMAND:  return state_set_MKDS_2_read;
        case EVENT_W2_COMMAND:  return state_set_MKDS_2_write;
        case EVENT_G1_COMMAND:  return state_data_from_MKDS_1;
        case EVENT_G2_COMMAND:  return state_data_from_MKDS_2;
/*----------------------------------------------------------------------------*/
        default: return state_none;
    }
}

/*-------------------------------------------------Ãƒ —-----------------------*/
static void *state_reboot_MKDS()
{  
    spi_reinit(MODULE_MKDS);
    reboot_MKDS();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_start_MKDS()
{   
    spi_reinit(MODULE_MKDS);
    start_MKDS();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_mesuring_tzch_MKDS()
{  
    spi_reinit(MODULE_MKDS);
    mesuring_MKDS();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_read_results_MKDS()
{   
    spi_reinit(MODULE_MKDS);
    read_results_MKDS();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}


static void *state_data_to_MKDS_1()
{  
    spi_reinit(MODULE_MKDS);
    data_to_MKDS_1();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_data_to_MKDS_2()
{  
    spi_reinit(MODULE_MKDS);
    data_to_MKDS_2();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_set_MKDS_1_read()
{  
    spi_reinit(MODULE_MKDS);
    set_MKDS_1_read();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_set_MKDS_1_write()
{  
    spi_reinit(MODULE_MKDS);
    set_MKDS_1_write();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_set_MKDS_2_read()
{  
    spi_reinit(MODULE_MKDS);
    set_MKDS_2_read();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_set_MKDS_2_write()
{  
    spi_reinit(MODULE_MKDS);
    set_MKDS_2_write();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_data_from_MKDS_1()
{  
    spi_reinit(MODULE_MKDS);
    data_from_MKDS_1();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
static void *state_data_from_MKDS_2()
{  
    spi_reinit(MODULE_MKDS);
    data_from_MKDS_2();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}
/*-------------------------------------------------Ã ¿—-----------------------*/
static void *state_setting_u()
{   
    spi_reinit(MODULE_MKAS);
    set_U();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_u()
{   
    spi_reinit(MODULE_MKAS);
    get_U();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_i()
{    
    spi_reinit(MODULE_MKAS);
    get_I();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}

static void *state_mesuring_d()
{   
    spi_reinit(MODULE_MKAS);
    get_Doza();
    clean_uart_buffer();
    event = EVENT_NONE;
    return state_none;
}




/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE state_machine.c */
