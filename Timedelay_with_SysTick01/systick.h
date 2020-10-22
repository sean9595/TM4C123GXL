/*
 * SysTick.h
 *
 *  Created on: Oct 9, 2020
 *      Author: taehoonkim
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

/*includes*/
#include <main.h>

void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Wait_10ms (uint32_t delay);

#endif /* SYSTICK_H_ */
