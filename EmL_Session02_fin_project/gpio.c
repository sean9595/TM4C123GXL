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
void PortA_Init(void){

}*/

void PortE_Init(void){
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
    GPIO_PORTE_AMSEL_R |= 0x0C;   // 7) enable analog functionality on PE2,3
}
