/*
 * systick.c
 *
 *  Created on: 11-Jul-2026
 *      Author: veere
 */


#include "systick.h"
#include "STM32F401CCUx.h"

#include <stdint.h>

static uint32_t g_systick_clock_hz;
void SysTick_Enable(void)
{
	SYSTICK->CTRL |= (1 << 0);
}


void SysTick_Disable(void)
{
	SYSTICK->CTRL &= ~(1 << 0);
}

void SysTick_SetReloadValue(uint32_t reload_value)
{

	SYSTICK->LOAD = reload_value & 0x00FFFFFFU;  /// since LOAD Is 24 bit lets just mask it
}

void SysTick_ClearCurrentValue(void)
{
	 SYSTICK->VAL = 0; // ARM SAYS WRITE ANYTHING IN VAL TO CLEAR CURRENT VALUE
}

void SysTick_SelectClock(uint8_t clock_source)
{
    if(clock_source == SYSTICK_CLKSOURCE_INTERNAL){
    	SYSTICK->CTRL |= (1 << 2);
    }
    else if(clock_source == SYSTICK_CLKSOURCE_EXTERNAL){
    	SYSTICK->CTRL&= ~(1 << 2);
    }

}




void SysTick_Init(uint8_t clock_source)
{
    if(clock_source == SYSTICK_CLKSOURCE_INTERNAL)
    {
        g_systick_clock_hz = SYSTICK_CLOCK_HZ;
    }
    else
    {
        g_systick_clock_hz = SYSTICK_CLOCK_HZ / 8U;
    }

    SysTick_SelectClock(clock_source);
}


void SysTick_DelayMs(uint32_t delay_ms)
{
    uint32_t reload = ((g_systick_clock_hz / 1000U) * delay_ms) - 1U;

    SysTick_SetReloadValue(reload);

    SysTick_ClearCurrentValue();

    SysTick_Enable();

    while(!(SYSTICK->CTRL & (1U << 16)));

    SysTick_Disable();
}
