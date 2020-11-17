/*
 * systick.c
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */
#include "systick.h"

void SysTick_Init(uint32_t period)
{
    NVIC_ST_CTRL_R = 0x00; //disable systick during setup
    NVIC_ST_RELOAD_R = period - 1; //reload value. SysTick(us) = (Bus frequency) /
    NVIC_ST_CURRENT_R = 0x00; //any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; //Priority 2
    NVIC_ST_CTRL_R = 0x07; //enable systick with the core clock and interrupts
}
