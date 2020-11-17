/*
 * adc.c
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */
#include "adc.h"

void ADC0_PortE_Init(void){
    while ((SYSCTL_PRADC_R & 0x0001) != 0x0001){};
    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;             // 8) configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123;        // 9) Sequencer 3 is highest priority

    /*Setting sequence*/
    ADC0_ACTSS_R &= ~0x0008;      // 10) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;       // 11) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 9;           // 12) set channel
    ADC0_SSCTL3_R = 0x0006;       // 13) no TS0 D0, yes IE0 END0 //Set flag
    ADC0_IM_R &= ~0x0008;         // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;       // 15) enable sample sequencer 3
}

//----------ADC0 InSeq3-----------
//Busy-wait analog->digital conversion
//Input: void
//Output: 12 bit result of ADC
uint32_t ADC0_InSeq3(void)
{
    uint32_t result;

    ADC0_PSSI_R = 0x0008;            // 1) initiate SS3. [START]

    while ((ADC0_RIS_R & 0x08) == 0)
    {
    };   // 2) wait for conversion done [CHECK STATUS]

    result = ADC0_SSFIFO3_R & 0xFFF;   // 3) read result [READ]

    ADC0_ISC_R = 0x0008;             // 4) acknowledge completion [CLEAR]

    return result;
}
