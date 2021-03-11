/**
  ******************************************************************************
  * @file    main.c
  * @author  Zaznov NIIKP
  * @version V2.0.0
  * @date    05/12/2020
  * @brief   This is the main file of project MBP_XFE
  ******************************************************************************
  * FILE main.c
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "my_assert.h"
//#include "MDR1986VE8T.h"
/* Defines -------------------------------------------------------------------*/ 

int main(void)
{   
        

    mcu_init();
    
    while(1)
    {    
        state_T = (STATE_FUNC_PTR_t)(*state_T)();
        wdt_rewrite();
    }
}




void mcu_init()
{
	POR_disable();                                                              /* Disable Power-on-Reset control. Hold the RESET button down until operation complete */
	CLKCTRL_DeInit();                                                           /* Set CLKCTRL to default */

	CLKCTRL_HSEconfig(CURRENT_CLK);                                             // Try to enable HSE0 clock 
    for(int i = 0; i < 50; i++){
        if(CLKCTRL_HSEstatus(CURRENT_CLK_RDY) == SUCCESS){
            break;
        }
    }
    if(CLKCTRL_HSEstatus(CURRENT_CLK_RDY) != SUCCESS){                          // if HSE0 does not clock 
        CURRENT_CLK         = HSE1_ON;                                          // change variables for HSE1
        CURRENT_CLK_RDY     = HSE1_RDY;
        CURRENT_CLKCTRL_MAX = HSE1div1;
        CURRENT_UART_CLK    = UART_HSE1;
        
        CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_OFF);                                // disable HSE0
        CLKCTRL_HSEconfig(CURRENT_CLK);                                         // try to enable HSE1
    }
    while(CLKCTRL_HSEstatus(CURRENT_CLK_RDY) != SUCCESS){}	                    // Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src 
	CLKCTRL_MAX_CLKSelection(CURRENT_CLKCTRL_MAX);	                            // Select MAX_CLK, MAX_CLK = HSE
    
    KEY_reg_accs();
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN,ENABLE);                  //Разрешение тактирования порта E
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTB_EN,ENABLE);                  //Разрешение тактирования порта B    
    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN,ENABLE);                  //Разрешение тактирования порта C
    uart_init(CURRENT_UART_CLK);     
    spi_init();       
    pin_init();  
    wdt_init();      
    NVIC_SetPriority(SysTick_IRQn, 1);                                          // Установить приоритет EXTI0        
}

void pin_init()
{
 
    PORT_InitTypeDef GPIO_user_ini;
    GPIO_user_ini.PORT_Pin              = (PORT_Pin_15|PORT_Pin_16);
    GPIO_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_user_ini.PORT_SFUNC            = PORT_SFUNC_5;
    GPIO_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTE, &GPIO_user_ini); 

    PORT_InitTypeDef GPIO_driver_user_ini;
    GPIO_driver_user_ini.PORT_Pin              = (PORT_Pin_23|PORT_Pin_22);     // Enable драйвера приема-передачи
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
    /* Прерывания "включены" и настроен активный уровень прерываний */	
    GPIO_B_user_ini.PORT_SIE           = PORT_SIE_ON;                             // Разрешается позже, по ходу выполнения программы
    GPIO_B_user_ini.PORT_SIT         = PORT_SIT_HIGH;
    PORT_Init(PORTB, &GPIO_B_user_ini);
    
    PORT_InitTypeDef GPIO_Bi_user_ini;
    GPIO_Bi_user_ini.PORT_Pin              = (PORT_Pin_18|PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_25|PORT_Pin_27|PORT_Pin_28|PORT_Pin_29|PORT_Pin_30|PORT_Pin_31);
    GPIO_Bi_user_ini.PORT_SOE              = PORT_SOE_OUT;
    GPIO_Bi_user_ini.PORT_SFUNC            = PORT_SFUNC_USER;
    GPIO_Bi_user_ini.PORT_SANALOG          = PORT_SANALOG_DIGITAL;
    GPIO_Bi_user_ini.PORT_SPD              = PORT_SPD_OFF;
    GPIO_Bi_user_ini.PORT_SPWR             = PORT_SPWR_10;
    PORT_Init(PORTB, &GPIO_Bi_user_ini); 
    
    PORT_ResetBits(PORTE, PORT_Pin_23);                                         // разреш приема и передачи UART
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
/******************* (C) COPYRIGHT 2020 NIIKP *********************************
*
* END OF FILE main.c */


