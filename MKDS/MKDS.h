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
void Data_Response_from_MKDS(void);
void get_TZCH(void);


void reboot_MKDS(void);
void start_MKDS(void);
void mesuring_MKDS(void);
void read_results_MKDS(void);

void data_to_MKDS_1(void);
void data_to_MKDS_2(void);
void set_MKDS_1_read(void);
void set_MKDS_1_write(void);
void set_MKDS_2_read(void);
void set_MKDS_2_write(void);
void data_from_MKDS_1(void);
void data_from_MKDS_2(void);

void set_MKDS_1_disable(void);
void set_MKDS_2_disable(void);

void send_data_to_MKDS_1(void);
void send_data_to_MKDS_2(void);
static void receive_data_to_MKDS(void);
static inline void convert_data_to_MKDS(void);
static void send_data_to_MKDS_1(void);
static void send_data_to_MKDS_2(void);
void receive_data_from_MKDS_1(void);
void receive_data_from_MKDS_2(void);
void send_data_to_XFE(void);


uint16_t SSP_ReceiveData_MKDS_1(MDR_SSP_TypeDef* SSPx);
void SSP_SendData_MKDS_1(MDR_SSP_TypeDef* SSPx, uint16_t Data);
uint16_t SSP_ReceiveData_MKDS_2(MDR_SSP_TypeDef* SSPx);
void SSP_SendData_MKDS_2(MDR_SSP_TypeDef* SSPx, uint16_t Data);
void get_result_MKDS_1(MDR_SSP_TypeDef* SSPx, uint16_t Data);
uint8_t get_chanal_results_MKDS_1(uint16_t chanal_number);

    
#endif /* __MKDS_H_ */

/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE MKDS.h */

