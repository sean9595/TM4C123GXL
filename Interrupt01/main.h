/*
 * main.h
 *
 *  Created on: Oct 21, 2020
 *      Author: taehoonkim
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

volatile uint8_t Int_Flag = 0;
volatile uint8_t count = 0;

void PortF_Init(void);
void EnableInterrupts(void);
//void WaitForInterrupt(void);
void GPIOInterrupt_Init(void);

#endif /* MAIN_H_ */
