/*
 * gpio.c
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 *
 * Switch->PE1, ADC -> PE2,3, LCD -> PA
 */
#include "gpio.h"
/*
//PA4,5->debugging
void PortA_Init(void){
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x01;           // 1) activate clock for Port A
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F -> GPIOCR을 unlock.
    GPIO_PORTA_CR_R = 0x30;           // allow changes to PF4-0
                                      // only PF0 needs to be unlocked, other bits can't be locked
    GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog on PF
    GPIO_PORTA_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIO_PORTA_DIR_R = 0x30;          // 5) PA4,5 out //0b0000,1110
    GPIO_PORTA_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
    //GPIO_PORTA_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
    GPIO_PORTA_DEN_R = 0x30;          // 7) enable digital I/O on PF4-0

}*/

void PortE_Init(void){
    //Pull-up 혹은 Pull-down 설정 추
    SYSCTL_RCGCADC_R |= 0x0001;   // 1) activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x10;    // 2) activate clock for Port E
    while ((SYSCTL_PRGPIO_R & 0x10) != 0x10)
    {
    };  // 3) for stabilization
    GPIO_PORTE_DIR_R &= ~0x0E;    // 4) make PE1,2,3 input
    GPIO_PORTE_AFSEL_R |= 0x0C;   // 5) enable alternate function on PE2,3
    GPIO_PORTE_DEN_R = 0x02;      // 6) disable digital I/O on PE2,3
                                  // enable digital I/O on PE1.
                                  // since we are using 'analog input'
    GPIO_PORTE_PCTL_R = 0x00000000; //ADC는 이것이 필요없고 GPIO기능에만 필요.
    GPIO_PORTE_AMSEL_R |= 0x0C;   // 7) enable analog functionality on PE2,3

}
