/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2011 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashPrg.c:  Flash Programming Functions adapted for               */
/*               ST Microelectronics STM32H7Rx-7Sx Flash               */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

typedef volatile unsigned char   vu8;
typedef          unsigned char    u8;
typedef volatile unsigned short   vu16;
typedef          unsigned short    u16;
typedef volatile unsigned long    vu32;
typedef          unsigned long     u32;
typedef volatile unsigned long long    vu64;
typedef          unsigned long long     u64;


typedef unsigned       int uint64_t;

#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))


#define FLASH_BASE      0x52002000
#define IWDG1_BASE      0x58004800
#define WWDG1_BASE      0x40002C00

#define WWDG1           ((WWDG_TypeDef *) WWDG1_BASE)
#define IWDG1           ((IWDG_TypeDef *) IWDG1_BASE)
#define FLASH           ((FLASH_TypeDef*) FLASH_BASE)


typedef struct
{
  vu32 CR;   /*!< WWDG Control register,       Address offset: 0x00 */
  vu32 CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
  vu32 SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;


typedef struct
{
  vu32 KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  vu32 PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  vu32 RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  vu32 SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  vu32 WINR; /*!< IWDG Window register,    Address offset: 0x10 */
} IWDG_TypeDef;


typedef struct
{
vu32 ACR;             /*!< FLASH access control register,            Address offset: 0x00 */
vu32 KEYR;          /*!< Flash Key Register ,                      Address offset: 0x04 */
vu32 RESERVED1[2];     /*!< Reserved,                                               0x8 to 0xc*/
vu32 CR;             /*!< Flash control Register,                Address offset: 0x10 */
vu32 SR;           /*!< Flash option control Register,                Address offset: 0x14 */
vu32 RESERVED2[2];     /*!< Reserved,                                               0x18 to 0x1c*/
vu32 IER;               /*!< FLASH interrupt enable register,         Address offset: 0x20 */
vu32 ISR;              /*!< FLASH interrupt status register,     Address offset: 0x24 */
vu32 ICR;                      /*!< FLASH interrupt clear register,     Address offset: 0x28 */
vu32 RESERVED3[1];     /*!< Reserved,                                               0x2c    */
vu32 CRCCR;                      /*!< FLASH CRC control register,     Address offset: 0x30 */
vu32 CRCSADDR;                      /*!< FLASH  CRC start address register ,     Address offset: 0x34 */
vu32 CRCEADDR;                      /*!< FLASH  CRC  end address register ,     Address offset: 0x38 */
vu32 CRCDATAR;                      /*!< FLASH  CRC   data register ,            Address offset: 0x3C */
vu32 ECCFADDR;                      /*!< FLASH   ECC fail register ,              Address offset: 0x40 */
vu32 RESERVED4[47];     /*!< Reserved,                                               0x44 to 0xfc*/
vu32 OPTKEYR;                      /*!< FLASH  options key address register ,     Address offset: 0x100 */
vu32 OPTCR;                      /*!< FLASH  options control register ,     Address offset: 0x104 */
vu32 OPTISR;                      /*!< FLASH  options interrupt status register  ,     Address offset: 0x108 */
vu32 OPTICR;                      /*!< FLASH  options interrupt clear register  ,     Address offset: 0x10C */
vu32 OBKCR;                      /*!< FLASH   option byte key control register ,     Address offset: 0x110 */
vu32 RESERVED5[59];     /*!< Reserved,                                               0x114 to 0x1FC*/
vu32 NVSR;                      /*!< FLASH   non-volatile status register  ,     Address offset: 0x200 */
vu32 NVSRP;                      /*!< FLASH   security status register programming  ,     Address offset: 0x204 */
vu32 ROTSR;                      /*!< FLASH   RoT status register  ,     Address offset: 0x208*/
vu32 ROTSRP;                      /*!< FLASH   RoT status register  ,     Address offset: 0x20C*/
vu32 OTPLSR;                      /*!< FLASH   RoT status register  ,     Address offset: 0x210*/
vu32 OTPLSRP;                      /*!< FLASH   RoT status register  ,     Address offset: 0x214*/
vu32 WRPSR;                      /*!< FLASH   write protection status register  ,     Address offset: 0x218 */
vu32 WRPSRP;                      /*!< FLASH   write protection status register programming   ,     Address offset: 0x21C */
vu32 RESERVED6[4];     /*!< Reserved,                                               0x220 to 0x2FC*/
vu32 HDPSR;                      /*!< FLASH    hide protection status register  ,     Address offset: 0x230 */
vu32 HDPSRP;                      /*!< FLASH   hide protection status register programming  ,     Address offset: 0x234 */
vu32 RESERVED7[3];     /*!< Reserved,                                               0x2438to 0x244*/    
vu32 FIXEDSR;                      /*!< FLASH option byte word 1 status register  ,     Address offset: 0x248 */
vu32 RESERVED8;     /*!< Reserved,                                               0x24C*/   
vu32 EPOCSR;                      /*!< FLASH option byte word 1 status register  ,     Address offset: 0x250 */
vu32 EPOCSRP;                      /*!< FLASH   option byte word 1 status register programming  ,     Address offset: 0x254 */
vu32 RESERVED9[3];     /*!< Reserved,                                               0x258to 0x25c*/
vu32 OBW1SR;                      /*!< FLASH option byte word 1 status register  ,     Address offset: 0x260 */
vu32 OBW1SRP;                      /*!< FLASH   option byte word 1 status register programming  ,     Address offset: 0x264 */
vu32 OBW2SR;                      /*!< FLASH option byte word 2 status register  ,     Address offset: 0x268 */
vu32 OBW2SRP;                      /*!< FLASH   option byte word 2 status register programming  ,     Address offset: 0x26c */
} FLASH_TypeDef;





/*******************  Bits definition for FLASH_CR register  ***********************/
#define FLASH_CR_LOCK                     0x00000001U
#define FLASH_CR_PG                       0x00000002U
#define FLASH_CR_SER                      0x00000004U
#define FLASH_CR_BER                      0x00000008U


#define FLASH_CR_START                    0x00000020U

#define FLASH_CR_SNB                      0x00000700U



/*******************  Bits definition for FLASH_SR register  ***********************/
#define FLASH_SR_BSY                      0x00000001U


#define FLASH_KEY1                         ((vu32)0x45670123) /*!< Flash key1 */
#define FLASH_KEY2                         ((vu32)0xCDEF89AB) /*!< Flash key2: used with FLASH_KEY1 
                                                                       to unlock the FLASH registers access */
#define FLASH_PDKEY1                       ((vu32)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2                       ((vu32)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                                       to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_OPTKEY1                      ((vu32)0x08192A3B) /*!< Flash option byte key1 */
#define FLASH_OPTKEY2                      ((vu32)0x4C5D6E7F) /*!< Flash option byte key2: used with FLASH_OPTKEY1*/ 


#define FLASH_FLAG_EOP										 ((vu32)0x00010000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_EOPERR									 ((vu32)0x00400000) /*!< Flash BUSY FLAG*/
#define FLASH_FLAG_QW			                 ((vu32)0x00000004) /*!< Flash BUSY FLAG_wait queue flag*/

#define FLASH_CR1_SNB1                      ((vu32)0x000001C0)

#define FLASH_BANK1_BASE          ((vu32)0x08000000) /*!< Base address of : Flash Bank1 accessible over AXI */ 




#define IS_FLASH_BANK1_ADDRESS(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) <= (FLASH_BANK1_BASE + 0x0000FFFF) ))



/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM 
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {


    __disable_irq();
 FLASH->ICR = 0x1F2E0000;
 while (FLASH->SR & FLASH_FLAG_QW);									
	 if (FLASH->CR & 1 == 1) 
  {
	/* Unlock FLASH A Registers access */
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	}
	
	 if ((FLASH->OBW1SR & 0x10) == 0x00) 
  {
     IWDG1->KR  = 0xAAAA;   
  IWDG1->KR  = 0x5555;                         // Enable write access to IWDG_PR and IWDG_RLR     
    IWDG1->PR  = 0x06;                           // Set prescaler to 256  
    IWDG1->RLR = 4095;                           // Set reload value to 4095
    WWDG1->CFR = 0x1FF;
    WWDG1->CR = 0x7F;

  }
	if(FLASH->CR & 1)
     return 1;
	
	 __enable_irq();

  return (0);
}
#endif



/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM 
int UnInit (unsigned long fnc) {

  FLASH->CR |=  1;                             // Lock Flash BAnk A


  return (0);
}
#endif


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseChip (void) {
	
 __disable_irq();
	while (FLASH->SR & FLASH_SR_BSY){};									// Reset Error Flags of Bank 1
	FLASH->ICR = 0x1F2E0000;

	/* Bank1 erase */

	FLASH->CR |= FLASH_CR_BER; 
	FLASH->CR |= FLASH_CR_START;
	
  while (FLASH->SR & FLASH_SR_BSY) {                // Wait BSY Flag Bank 1
  }
	 while (FLASH->SR & FLASH_FLAG_QW) ;
	FLASH->CR &= ~FLASH_CR_BER;

	
  return (0);                                          
}
#endif


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

#ifdef FLASH_MEM
int EraseSector (unsigned long adr) {
	
	 __disable_irq();
    FLASH->ICR = 0x1F2E0000;
	u8 Sector = ((adr & 0x00FFFFFF) >> 13)& 0xF;
	
	
	/* BankA erase */

		
		  while (FLASH->SR & FLASH_SR_BSY) {                // Wait BSY Flag Bank 1
  }
	  while (FLASH->SR & FLASH_FLAG_QW){
		}
			
		FLASH->CR &= (~FLASH_CR1_SNB1);   //clear the SNB (sector erase selection number) BIT
    FLASH->CR |= (FLASH_CR_SER | (Sector << 6));	
    FLASH->CR |= FLASH_CR_START;	
			  while (FLASH->SR & FLASH_SR_BSY) {                // Wait BSY Flag Bank 1
  }
		while (FLASH->SR & FLASH_FLAG_QW);                  // Wait BSY Flag Bank 1
		
		FLASH->CR &= ~FLASH_CR_SER;
		
		 if (FLASH->SR & FLASH_FLAG_EOP != 0)
        return 1;

		
  return (0);                                           
}
#endif


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

#if defined FLASH_MEM || defined FLASH_OTP
int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
__disable_irq();
	 u64 *dest = ( u64 *)adr;
   u64 *src = ( u64*)((u32)buf);
	int i = 0;
	
	
  	/* BankA  */

		while (FLASH->SR & FLASH_FLAG_QW);
		while (FLASH->SR & FLASH_SR_BSY);

	
	 FLASH->ICR = 0x1F2E0000; // Reset Error Flags
   
	while (sz) {

			FLASH->CR = FLASH_CR_PG;			 // Programming Enabled 
		
		if (sz>=32)
		{
			/* Program the 256 bits flash */
			for (i=0;i<4;i++)
			{
         *dest++ = *src++;
		    	
			}
			
			sz  -= 32;
						
		}
		else
		{
      
		    u8 *dest8 = ( u8*)dest;
        u8 *src8 = ( u8*)(src);
			
			 /*write a word completed with 0xFF*/		
				for (i=0;i<(sz);i++)
				{
				 	*(dest8)++ = *(src8)++;
				}
				for (i=0;i<((32 - sz));i++)
				{
					*(dest8)++ = 0xFF;
				}
							
			sz  = 0;
		}
			

			while (FLASH->SR & FLASH_FLAG_QW){
			}
		  while (FLASH->SR & FLASH_SR_BSY);
	
		if (FLASH->ISR & FLASH_FLAG_EOP == 1)	// Check for Error
		{
				__enable_irq();
				FLASH->CR &= ~FLASH_CR_PG;			 				 	// Programming disabled 
			
			return (1);
		}
	  		
 
}
	__enable_irq();
  return (0);                                           
}
#endif
