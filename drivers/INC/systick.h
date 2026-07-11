/*
 * systick.h
 *
 *  Created on: 11-Jul-2026
 *      Author: veeresh
 */
#include<stdint.h>
#ifndef SYSTICK_H_
#define SYSTICK_H_

void SysTick_Init(uint8_t clock_source);
void SysTick_DelayMs(uint32_t ms);
void SysTick_Enable(void);
void SysTick_Disable(void);
void SysTick_SetReloadValue(uint32_t reload_value);
void SysTick_ClearCurrentValue(void);

void SysTick_SelectClock(uint8_t clock_source);

void SysTick_DelayMs(uint32_t delay_ms);

#endif /* SYSTICK_H_ */
