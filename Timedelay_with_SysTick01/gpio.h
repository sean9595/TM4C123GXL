/*
 * gpio.h
 *
 *  Created on: Oct 9, 2020
 *      Author: taehoonkim
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <main.h>

void PortF_Init(void);

/*variables & definitions*/
//PF1의 경우에는 0x02, PF2는 0x04, PF3은 0x08을 씀으로써 toggle이 가능하다.
#define PF1   (*((volatile unsigned long *)0x40025008)) //GPIO_PCTL_PF1_M을 보면 0xF0으로 마스킹되어 있는데 따라서 4개의 비트를 한개의 비트가 차지하고 있으므로 *4한다.
#define PF2   (*((volatile unsigned long *)0x40025010))
#define PF3   (*((volatile unsigned long *)0x40025020))

#endif /* GPIO_H_ */
