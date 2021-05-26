/**
  ******************************************************************************
  * @file    MKDS.h
  * @author  Zaznov NIIKP
  * @version V3.0.0
  * @date    01/05/2021
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE MKDS.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MKDS_H_
#define __MKDS_H_

/* Includes ------------------------------------------------------------------*/ 
#include "Spi.h"
#include "Uart.h"
#include "my_assert.h"
#include "logic.h"
#include "delays.h"

/* Defines -------------------------------------------------------------------*/  

/* Variables -----------------------------------------------------------------*/


/* Functions -----------------------------------------------------------------*/
/*-------------------------------------------------ÌÊÄÑ 1---------------------*/
void MKDS_reboot(void);
void MKDS_start(void);
void MKDS_mesuring(void);
void MKDS_read_results(void);

/*-------------------------------------------------ÌÊÄÑ 2---------------------*/
void MKDS_data_to_MKDS(MODULE MODULE_NAME);
void MKDS_set_MKDS_read(MODULE MODULE_NAME);
void MKDS_set_MKDS_write(MODULE MODULE_NAME);
void MKDS_data_from_MKDS(MODULE MODULE_NAME);
void MKDS_set_MKDS_disable(MODULE MODULE_NAME);


/*----------------------------------------------------------------------------*/



    
#endif /* __MKDS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.h */

