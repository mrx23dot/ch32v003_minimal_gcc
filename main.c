#include "ch32v00x.h"

void SystemInit48HSI( void )
{
	// Values lifted from the EVT.  There is little to no documentation on what this does.
	RCC->CTLR  = RCC_HSION | RCC_PLLON; 				// Use HSI, but enable PLL.
	RCC->CFGR0 = RCC_HPRE_DIV1 | RCC_PLLSRC_HSI_Mul2;	// PLLCLK = HSI * 2 = 48 MHz; HCLK = SYSCLK = APB1
	FLASH->ACTLR = FLASH_ACTLR_LATENCY_1;				// 1 Cycle Latency
	RCC->INTR  = 0x009F0000;                            // Clear PLL, CSSC, HSE, HSI and LSI ready flags.

	// From SetSysClockTo_48MHZ_HSI
	while((RCC->CTLR & RCC_PLLRDY) == 0);														// Wait till PLL is ready
	RCC->CFGR0 = ( RCC->CFGR0 & ((uint32_t)~(RCC_SW))) | (uint32_t)RCC_SW_PLL;					// Select PLL as system clock source
	while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08);									// Wait till PLL is used as system clock source
}

/*********************************************************************
 * @fn      SystemInit
 *
 * @brief   Setup the microcontroller system Initialize the Embedded Flash Interface,
 *        the PLL and update the SystemCoreClock variable.
 *
 * @return  none
 */
void SystemInit (void)
{
  RCC->CTLR |= (uint32_t)0x00000001;
  RCC->CFGR0 &= (uint32_t)0xFCFF0000;
  RCC->CTLR &= (uint32_t)0xFEF6FFFF;
  RCC->CTLR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR0 &= (uint32_t)0xFFFEFFFF;
  RCC->INTR = 0x009F0000;
}

/** entry point */
int main(void)
{
	SystemInit();
	SystemInit48HSI();

	// Enable GPIOC.
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;

	// GPIO C1 Push-Pull, 10MHz Slew Rate Setting
	GPIOC->CFGLR &= ~(0xf<<(4*1));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*1);

	while(1)
	{
		GPIOC->BSHR = 1 << 1;	 // Turn on GPIOC1
		GPIOC->BCR = 1 << 1;    // Turn off GPIOC1
	}

  return 0;
}
// WCH-Interrupt-fast can be properly emulated in standard GCC but this is a minimal example
void NMI_Handler(void); //__attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void); //__attribute__((interrupt("WCH-Interrupt-fast")));

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

