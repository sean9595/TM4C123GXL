/*
 * adc.h
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */

#ifndef ADC_H_
#define ADC_H_

#include "main.h"

void ADC0_PortE_Init(void);
void ADC0_InSeq2(void);

uint32_t ADC_xy_Data[2] = {0,0};

#endif /* ADC_H_ */
