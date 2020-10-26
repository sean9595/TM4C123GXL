/*
 * gpio.h
 *
 *  Created on: Oct 27, 2020
 *      Author: taehoonkim
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "main.h"

#define PF2   (*((volatile unsigned long *)0x40025010))

void GPIO_Init(void);

#endif /* GPIO_H_ */
