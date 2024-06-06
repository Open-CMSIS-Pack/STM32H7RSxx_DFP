/**
  ******************************************************************************
  * @file    STM32H7RSOSPI.h
  * @author  MCD Application Team
  * @brief   Header file of STM32H7RSOSPI.c
  *           
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


#ifndef __STM32U5OSPI_H
#define __STM32U5OSPI_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stm32h7rsxx_hal.h"
#include "stm32h7rsxx_hal_xspi.h"
#include "stm32h7s78_discovery_xspi.h"
#include <string.h>
#include "./mx66uw1g45g/mx66uw1g45g.h"

#define TIMEOUT 5000U

/* Private function prototypes -----------------------------------------------*/
int Init_OSPI(void);
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
 int MassErase ( void);
int SystemClock_Config(void);
 
static void    XSPI_NOR_MspInit(XSPI_HandleTypeDef *hxspi);
static void    XSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hxspi);
static int32_t XSPI_NOR_ResetMemory(uint32_t Instance);
static int32_t XSPI_NOR_EnterDOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_EnterSOPIMode(uint32_t Instance);
static int32_t XSPI_NOR_ExitOPIMode(uint32_t Instance);

#endif
