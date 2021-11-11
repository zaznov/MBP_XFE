/**
  ******************************************************************************
  * @file    main.c
  * @author  Zaznov NIIKP
  * @version V3.0.1
  * @date    20/08/2021
  * @brief   This is the main file of project MBP_XFE. Contains general 
  * functions realisations for work with MAIN from XFE project:
  *           + GPIO pin initialisation
  *           + MCU periphery initialisation
  *           + main() function
  ******************************************************************************
  * FILE main.c
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mdr32f8_it.h"                 // 
#include "spec.h"                       // 
#include "mdr32f8_config.h"             // Keil::Drivers:CONFIG
#include "mdr32f8_clkctrl.h"            // Keil::Drivers:CLKCTRL
#include "mdr32f8_port.h"               // Keil::Drivers:PORT
#include "mdr32f8_ssp.h"
#include "core_cm4.h"

/* My Includes ---------------------------------------------------------------*/
#include "Uart.h"
#include "Spi.h"
#include "state_machine.h"
#include "my_assert.h"
#include "wdt.h"
#include "my_assert.h"
#include "status.h"
/* Defines -------------------------------------------------------------------*/ 


/* Variables -----------------------------------------------------------------*/
uint32_t SystemCoreClock = 8000000;
extern STATE_FUNC_PTR_t state_T;
status_TypeDef status; 

/* Functions -----------------------------------------------------------------*/
static void pin_init(void);
static void mcu_perif_init(void);

/**
  * @brief  Entry point. Main function of the project. Calls initialaze perif 
  * function and then calls in infitate loop state_T function, which is 
  * basically a pointer, which points on necessary at this moment function. All 
  * possible "states of the program" are discribed in state_machine module. 
  * @param  None
  * @retval None
  */
int main(void)
{     
    mcu_perif_init();
    while(1)
    {    
        state_T = (STATE_FUNC_PTR_t)(*state_T)();
        wdt_rewrite();

    }
}


/**
  * @brief  Function of the periferal initialization.
  * @param  None
  * @retval None
  */
static void mcu_perif_init(void)
{
	POR_disable();                                                              // Disable Power-on-Reset control 
	CLKCTRL_DeInit();                                                           // Set CLKCTRL to default 

	CLKCTRL_HSEconfig(get_CURRENT_CLK());                                       // Try to enable HSE0 clock 
    for(int i = 0; i < 50; i++){
        if(CLKCTRL_HSEstatus(get_CURRENT_CLK_RDY()) == SUCCESS){
            break;
        }
    }
    if(CLKCTRL_HSEstatus(get_CURRENT_CLK_RDY()) != SUCCESS){                    // if HSE0 does not work 
        set_CURRENT_CLK(HSE1_ON);                                               // change variables for HSE1
        set_CURRENT_CLK_RDY(HSE1_RDY);
        set_CURRENT_CLKCTRL_MAX(HSE1div1);
        set_CURRENT_UART_CLK(UART_HSE1);
        set_CURRENT_SPI_CLK(SPI_HSE1);
        
        CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_OFF);                                // disable HSE0
        CLKCTRL_HSEconfig(get_CURRENT_CLK());                                   // try to enable HSE1
    }
    while(CLKCTRL_HSEstatus(get_CURRENT_CLK_RDY()) != SUCCESS){}	            // Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src 
	CLKCTRL_MAX_CLKSelection(get_CURRENT_CLKCTRL_MAX());	                    // Select MAX_CLK, MAX_CLK = HSE
    
    KEY_reg_accs();
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN,ENABLE);                  //Enable clock of port E
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN,ENABLE);                  //Enable clock of port B    
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN,ENABLE);                  //Enable clock of port C
    uart_init(get_CURRENT_UART_CLK());     
    spi_init(get_CURRENT_SPI_CLK());       
    pin_init();  
    wdt_init();      
    NVIC_SetPriority(SysTick_IRQn, 1);                                          // Set priorites EXTI0        
    status_init(&status);
}
/**
  * @brief  Initialisation of GPIO
  * @param  None
  * @retval None
  */
static void pin_init(void)
{
 
    PORT_InitTypeDef GPIO_user_ini;
    GPIO_user_ini.PORT_Pin              = (PORT_Pin_15|PORT_Pin_16);
    GPIO_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_user_ini.PORT_SFUNC            = PORT_SFUNC_5;
    GPIO_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTE, &GPIO_user_ini); 

    PORT_InitTypeDef GPIO_driver_user_ini;
    GPIO_driver_user_ini.PORT_Pin              = (PORT_Pin_23|PORT_Pin_22);     // Enable receive-transmit driver
    GPIO_driver_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_driver_user_ini.PORT_SFUNC            = PORT_SFUNC_USER;
    GPIO_driver_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_driver_user_ini.PORT_SPD              = PORT_SPD_OFF;
    GPIO_driver_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTE, &GPIO_driver_user_ini); 

    PORT_InitTypeDef GPIO_B_user_ini;
    GPIO_B_user_ini.PORT_Pin         = PORT_Pin_19;
    GPIO_B_user_ini.PORT_SOE         = PORT_SOE_IN;
    GPIO_B_user_ini.PORT_SANALOG     = PORT_SANALOG_DIGITAL;
    GPIO_B_user_ini.PORT_SPD         = PORT_SPD_OFF;
    GPIO_B_user_ini.PORT_SPWR        = PORT_SPWR_10;
    /* Interrupts are enable, active level is high */	
    GPIO_B_user_ini.PORT_SIE           = PORT_SIE_ON;                           // Will be enable later, during program execution
    GPIO_B_user_ini.PORT_SIT         = PORT_SIT_HIGH;
    PORT_Init(PORTB, &GPIO_B_user_ini);
    
    PORT_InitTypeDef GPIO_Bi_user_ini;
    GPIO_Bi_user_ini.PORT_Pin              = (PORT_Pin_17|PORT_Pin_18|PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_25|PORT_Pin_27|PORT_Pin_28|PORT_Pin_29|PORT_Pin_30|PORT_Pin_31);
    GPIO_Bi_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_Bi_user_ini.PORT_SFUNC            = PORT_SFUNC_USER;
    GPIO_Bi_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_Bi_user_ini.PORT_SPD              = PORT_SPD_OFF;
    GPIO_Bi_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTB, &GPIO_Bi_user_ini); 
    
    PORT_ResetBits(PORTE, PORT_Pin_23);                                         // Enable UART receive and transmit 
    PORT_ResetBits(PORTE, PORT_Pin_22);
    PORT_ResetBits(PORTB, PORT_Pin_27);
    PORT_SetBits(PORTB, PORT_Pin_18);
    PORT_SetBits(PORTB, PORT_Pin_22);
    PORT_SetBits(PORTB, PORT_Pin_21);
    PORT_SetBits(PORTB, PORT_Pin_20);
    PORT_SetBits(PORTB, PORT_Pin_25);
    
    
    PORT_InitTypeDef GPIO_Ci_user_ini;
    GPIO_Ci_user_ini.PORT_Pin              = (PORT_Pin_5);
    GPIO_Ci_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_Ci_user_ini.PORT_SFUNC            = PORT_SFUNC_USER;
    GPIO_Ci_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_Ci_user_ini.PORT_SPD              = PORT_SPD_OFF;
    GPIO_Ci_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTC, &GPIO_Ci_user_ini); 
    
}

/**************** (C) COPYRIGHT 2021 Zaznov NIIKP ******************************
*
* END OF FILE main.c */


