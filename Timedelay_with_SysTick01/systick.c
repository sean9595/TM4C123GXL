/*
 * SysTick.c
 *
 *  Created on: Oct 9, 2020
 *      Author: taehoonkim
 */
#include <systick.h>

void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;          // disalbe SysTick during setup
    NVIC_ST_CTRL_R = 0x00000005; // enable SysTick with core clk
}

void SysTick_Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay - 1;
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R & 0x00010000) ==0){};
} //12.5ns

void SysTick_Wait_10ms (uint32_t delay){
    uint32_t i;
    for(i=0;i<delay;i++){
        SysTick_Wait(800000); //wait 10ms
    }
} //800000*12.5ns = 10ms
