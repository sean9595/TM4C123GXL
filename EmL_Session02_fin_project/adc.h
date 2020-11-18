/*
 * adc.h
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */

#ifndef ADC_H_
#define ADC_H_

#include "main.h"

void ADC_var_Init(void);
void ADC0_PortE_Init(void);
void ADC0_InSeq2(volatile uint32_t data[2]);

typedef struct{
    uint32_t x_to_LCD;
    uint32_t y_to_LCD;
    //volatile uint32_t xy_Data[2];
} Data;

Data ADC;


#endif /* ADC_H_ */
