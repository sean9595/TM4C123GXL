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
    ADC0_SSPRI_R = 0x2310;        // 9) Sequencer 2 is Highest priority(0x3)
    //0b0010.0011.0001.0000       //    Sequencer 3 is lowest priority(0x0)

    /*Setting sample sequence 2*/
    ADC0_ACTSS_R &= ~0x0004;      // 10) disable sample sequencer 2
    ADC0_EMUX_R &= ~0x0F00;       // 11) seq2 is software trigger
    ADC0_SSMUX2_R = 0x01;         // 12) set channel. AIn1(first), AIn0(second)
    ADC0_SSCTL2_R = 0x0060;       // 13) no TS0 D0, yes IE0 END0 //Set flag
    //0b0110.0000->2nd sample is the last one.
                                  // Select properties(D, END, IE, TS) of each samples.
    ADC0_IM_R &= ~0x0004;         // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0004;       // 15) enable sample sequencer 3
}

//----------ADC0 InSeq2-----------
// Input: void
// Output: void / Two 12 bit result of ADC
// Samples: AIN1->PE2, AIN0->PE3
// 125k max sampling
// software trigger, busy-wait sampling
// data returned by reference
// data[0] is AIn1 (PE2) 0 to 4095 -> x-axis
// data[1] is AIn0 (PE3) 0 to 4095 -> y-axis
void ADC0_InSeq2(void)
{
    ADC0_PSSI_R = 0x0004;            // 1) initiate SS2. [START]

    while ((ADC0_RIS_R & 0x04) == 0)
    {
    };   // 2) wait for conversion done [CHECK STATUS]

    ADC_xy_Data[0] = ADC0_SSFIFO2_R&0xFFF; //Read AIn1
    ADC_xy_Data[1] = ADC0_SSFIFO2_R&0xFFF; //Read AIn0
    ADC0_ISC_R = 0x0004;             // 4) acknowledge completion [CLEAR]
}
