/**
  ******************************************************************************
  * @file    status.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    26/05/2021
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE status.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATUS_H_
#define __STATUS_H_
/* Includes ------------------------------------------------------------------*/ 
#include "MDR1986VE8T.h"               
/* defines -------------------------------------------------------------------*/  



/* Variables -----------------------------------------------------------------*/
typedef enum{
    HSE0_ON     =   (uint32_t)0x08000000,
    HSE1_ON     =   (uint32_t)0x08000001,
}CLK;
typedef enum{
    HSE0_RDY    =   (uint32_t)0x54,
    HSE1_RDY    =   (uint32_t)0x74,
}CLK_RDY;
typedef enum{
    HSE0div1    =   (uint32_t)0x00000002,
    HSE0div2    =   (uint32_t)0x00000003,
    HSE1div1    =   (uint32_t)0x00000004,
    HSE1div2    =   (uint32_t)0x00000005,
}CLKCTRL_MAX;
typedef enum{
    UART_HSE0   =   (uint32_t)0x00000001,
    UART_HSE1   =   (uint32_t)0x00000002,
}UART_CLK;

typedef enum{
    SPI_HSE0   =   (uint32_t)0x00000001,
    SPI_HSE1   =   (uint32_t)0x00000002,
}SPI_CLK;




struct MUI_st
{
    unsigned char main_clk_sourse : 2;
    unsigned char uart_clk_sourse : 2;
    unsigned char spi_clk_sourse : 2;
};
struct MKAS_st
{
    unsigned char first : 2;
    unsigned char second : 2;
    unsigned char third : 4;
};
struct MKDS_st
{
    unsigned char first : 2;
    unsigned char second : 2;
    unsigned char third : 4;
};
struct AKI_st
{
    unsigned char first : 2;
    unsigned char second : 2;
    unsigned char third : 4;
};
typedef struct
{
    struct MUI_st   MUI;
    struct MKAS_st  MKAS;
    struct MKDS_st  MKDS_1;
    struct MKDS_st  MKDS_2;
    struct AKI_st   AKI;
}status_TypeDef;


/* Functions -----------------------------------------------------------------*/


uint32_t    get_CURRENT_CLK(void);
void        set_CURRENT_CLK(CLK NEW_CURRENT_CLK);

uint32_t    get_CURRENT_CLK_RDY(void);
void        set_CURRENT_CLK_RDY(CLK_RDY NEW_CURRENT_CLK_RDY);

uint32_t    get_CURRENT_CLKCTRL_MAX(void);
void        set_CURRENT_CLKCTRL_MAX(CLKCTRL_MAX NEW_CURRENT_CLKCTRL_MAX);

uint32_t    get_CURRENT_UART_CLK(void);
void        set_CURRENT_UART_CLK(UART_CLK NEW_CURRENT_UART_CLK);

uint32_t    get_CURRENT_SPI_CLK(void);
void        set_CURRENT_SPI_CLK(SPI_CLK NEW_CURRENT_SPI_CLK);

void status_init(status_TypeDef* status_struct);
void update_status(void);
void status_get_status(void);

#endif /* __STATUS_H_ */

/************************* 2021 Zaznov NIIKP ***********************************
*
* END OF FILE status.h */

