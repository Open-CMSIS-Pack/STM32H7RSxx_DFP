/**
  ******************************************************************************
  * @file    STM32H7RSOSPI.c
  * @author  MCD Application Team
  * @brief   This file defines the operations of the external loader for
  *          MX66UW1G45G OSPI memory of STM32H7S78-DK.
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


#include "STM32H7OSPI.h"

#include <string.h>


//defined to surpress vector declaration error
__attribute__ ((section(".vectors")))
const void * __Vectors[] = {
    // ... other interrupt and exception handlers ...
};
 

BSP_XSPI_NOR_Init_t Flash;
 int SystemClock_Config(void);
/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32H7B3I/B0_EVAL_OSPI_Private_Functions Private Functions
  * @{
  */

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  HAL_StatusTypeDef retr = HAL_ERROR;
  /* Check uwTickFreq for MisraC 2012 (even if uwTickFreq is a enum type that doesn't take the value zero)*/
  if ((uint32_t)uwTickFreq != 0U)
  {
    uint32_t ticks = SystemCoreClock / (1000U / (uint32_t)uwTickFreq);
    SysTick->LOAD  = (uint32_t)(ticks - 1UL);      /* Set reload register */
    SysTick->VAL   = 0UL;                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |  /* Set processor clock */
                     SysTick_CTRL_ENABLE_Msk;      /* Enable SysTick Timer */
    retr = HAL_OK;
  }
  return retr;
}

/**
  * @brief Provide a tick value in millisecond.
  * @note The function is an override of the HAL function to increment the
  *       tick on a count flag event.
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
  if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == SysTick_CTRL_COUNTFLAG_Msk)
  {
    uwTick++;
  }
  return uwTick;
}
/**
  * @brief  System initialization.
  * @param  None
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */
int Init_OSPI()
{

	/*disable interrupts*/
__disable_irq();
	int32_t result=0;


  /* Zero Init structs */
 memset(&Flash,0,sizeof(Flash));
  SystemInit(); 
	 
  Flash.InterfaceMode = BSP_XSPI_NOR_SPI_MODE;
   Flash.TransferRate  = BSP_XSPI_NOR_STR_TRANSFER;

	 /* Configure the system clock  */
   SystemClock_Config();
	
	BSP_XSPI_NOR_DeInit(0);
/*Initialaize OSPI*/
   if(BSP_XSPI_NOR_Init(0,&Flash) !=0)
    return 0;
/*Configure the OSPI in memory-mapped mode*/
   result=BSP_XSPI_NOR_EnableMemoryMappedMode(0);  

	 if(result==0)
    return 1; 

   return 0;
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 160000000
  *            HCLK(Hz)                       = 160000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_MBOOST                     = 1
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
 int SystemClock_Config(void)
{
 RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Configure the system Power Supply */
  if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }


  /* Enable voltage range 1 for VOS High level */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* Activate PLL1 with HSI as source (HSI is ON at reset) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 16;
  RCC_OscInitStruct.PLL1.PLLN = 64; /* PLL1 VCO = 64/4 * 60 = 960MHz */
  RCC_OscInitStruct.PLL1.PLLP = 2;  /* PLL1 P =480MHz */
  RCC_OscInitStruct.PLL1.PLLQ = 2;  /* PLL1 Q =480MHz */
  RCC_OscInitStruct.PLL1.PLLR = 2;  /* PLL1 R =480MHz */
  RCC_OscInitStruct.PLL1.PLLS = 2;  /* PLL1 S =480MHz */
  RCC_OscInitStruct.PLL1.PLLT = 2;  /* PLL1 T =480MHz */
  RCC_OscInitStruct.PLL1.PLLFractional = 0;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1, PCLK2,
     PCLK4 and PCLK5 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  | \
                                 RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2 | \
                                 RCC_CLOCKTYPE_PCLK4  | RCC_CLOCKTYPE_PCLK5);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;   /* System CPU clock=pll1p_ck */
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;     /* AXI/AHB System bus clock=System CPU clock/2 */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;     /* APB1 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;     /* APB2 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;     /* APB4 bus clock=System bus clock/4 */
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV2;     /* APB5 bus clock=System bus clock/4 */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }
  return 1;
}
/**
  * @brief   erase memory.
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */

 int MassErase (void){
 /*Initialaize OSPI*/
	 BSP_XSPI_NOR_DeInit(0);
   BSP_XSPI_NOR_Init(0,&Flash);
	
	 /*Erases the entire OSPI memory*/
	 BSP_XSPI_NOR_Erase_Chip(0);

  /*Reads current status of the OSPI memory*/
	while (BSP_XSPI_NOR_GetStatus(0)!=BSP_ERROR_NONE){};

	 return 0;
 
 }
 
 /**
  * @brief   Program memory.
  * @param   Address: page address
  * @param   Size   : size of data
  * @param   buffer : pointer to data buffer
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */
 int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    /*Initialaize OSPI*/
    BSP_XSPI_NOR_DeInit(0);
    Address = Address & 0x0fffffff;
   BSP_XSPI_NOR_Init(0,&Flash);
   
	 /*Writes data to the OSPI memory*/
    BSP_XSPI_NOR_Write(0,buffer,Address, Size);
  
   return 1;
}


/**
  * @brief   Sector erase.
  * @param   EraseStartAddress :  erase start address
  * @param   EraseEndAddress   :  erase end address
  * @retval  None
  */
 int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
	
  uint32_t BlockAddr;
	EraseStartAddress &= 0x0FFFFFFF;  
  EraseEndAddress &= 0x0FFFFFFF;
  EraseStartAddress = EraseStartAddress -  EraseStartAddress % 0x10000;
   /*Initialaize OSPI*/
	

		BSP_XSPI_NOR_DeInit(0);
  BSP_XSPI_NOR_Init(0,&Flash);	
    BlockAddr = EraseStartAddress;
  /*Erases the specified block of the OSPI memory*/
  BSP_XSPI_NOR_Erase_Block(0,BlockAddr, MX66UW1G45G_ERASE_64K );
 
   /*Reads current status of the OSPI memory*/
		while (BSP_XSPI_NOR_GetStatus(0)!=0);

		EraseStartAddress+=0x10000;
		/*Configure the OSPI in memory-mapped mode*/
    BSP_XSPI_NOR_EnableMemoryMappedMode(0);


 
  return 1;	
}







/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
