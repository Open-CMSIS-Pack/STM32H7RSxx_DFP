/**
  ******************************************************************************
  * @file    STM32H7OSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32H7OSPI.c
  *           
**************************************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *************************************************************************************************/


#ifndef __STM32H7OSPI_H
#define __STM32H7OSPI_H
#include "stdint.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_hal.h"
#include "stm32h7s78_discovery_xspi.h"


#define TIMEOUT 5000U


/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
 int MassErase ( void);


#endif
