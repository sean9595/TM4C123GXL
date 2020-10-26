/*
 * gpio.c
 *
 *  Created on: Oct 26, 2020
 *      Author: taehoonkim
 */
#include "gpio.h"

void GPIO_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x20;           // 1) activate clock for Port F
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTF_CR_R = 0x04;           // allow changes to PF2
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIO_PORTF_DIR_R = 0x04;          // 5) PF2 out
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
    GPIO_PORTF_DEN_R = 0x04;          // 7) enable digital I/O on PF4-0
}

