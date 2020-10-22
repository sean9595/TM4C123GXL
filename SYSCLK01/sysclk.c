/*
 * sysclk.c
 *
 *  Created on: Sep 28, 2020
 *      Author: taehoonkim
 */
#include <sysclk.h>

/*
 * Caution
 * Sysclk is usually configured by 'Software driven model' which is more safe and stable.
 * This program is just for study how to set sysclk.
 */

void SYSCLK_80MHz(void)
{

    // 0) Use RCC2
    SYSCTL_RCC2_R |= 0x80000000;  // USERCC2 (bit31)

    // 1) bypass PLL while initializing
    SYSCTL_RCC2_R |= 0x00000800;  // BYPASS2, PLL bypass

    // 2) select the crystal value and oscillator source
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0)   // clear XTAL field, bits 10-6
    + 0x00000540;           // 10101, configure for 16 MHz crystal extern
    // 3) activate PLL by clearing PWRDN
    SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source

    // 4) set the desired system divider
    SYSCTL_RCC2_R &= ~0x00002000;
    SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL
    //RCC2[28:21]=SYSDIV2 400MHz/(4+1) = 80MHz

    // 5) wait for the PLL to lock by polling PLLLRIS
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000)  // clear system clock divider
    + (4 << 22);      // configure for 80 MHz clock

    // 6) enable use of PLL by clearing BYPASS
    while ((SYSCTL_RIS_R & 0x00000040) == 0)
    {};// wait for PLLRIS bit

    // 7) Set BYPASS to 0, select PLL as the source of system clock
    SYSCTL_RCC2_R &= ~0x00000800;
}
