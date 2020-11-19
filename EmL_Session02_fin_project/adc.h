/*
 * adc.h
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */

#ifndef ADC_H_
#define ADC_H_

#include "main.h"

typedef struct{
    uint32_t x_to_LCD;
    uint32_t y_to_LCD;
    uint32_t xy_Data[2];
} Data;

Data ADC;

void ADC_var_Init(void);
void ADC0_PortE_Init(void);
void ADC0_InSeq2(Data *data);


#endif /* ADC_H_ */
