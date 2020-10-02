/**
  ******************************************************************************
  * @file    my_assert.h
  * @author  Zaznov NIIKP
  * @version V1.0.0
  * @date    21/08/2020
  * @brief   This file contains all the functions prototypes for work with...
  To view Datasheet visit:
  https:....
  ******************************************************************************
  * FILE my_assert.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/ 

/* defines -------------------------------------------------------------------*/  
//#define NDEBUG

#ifndef NDEBUG
    #define my_assert_param(expr) ((expr) ? (void)0 : my_assert_failed())
    void my_assert_failed(void);
#else 
    #define my_assert_param(expr) ((void)0)
#endif
/* Variables -----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/



/************************* 2020 Zaznov NIIKP ***********************************
*
* END OF FILE my_assert.h */

