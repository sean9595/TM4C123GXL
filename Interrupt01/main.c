/*
 * main.c
 * Topic: Interrupt01
 *  Created on: Oct 21, 2020
 *      Author: taehoonkim
 */
#include "main.h"

#define PF1   (*((volatile unsigned long *)0x40025008)) //GPIO_PCTL_PF1_M을 보면 0xF0으로 마스킹되어 있는데 따라서 4개의 비트를 한개의 비트가 차지하고 있으므로 *4한다.
#define PF2   (*((volatile unsigned long *)0x40025010))
#define PF3   (*((volatile unsigned long *)0x40025020))

void PortF_Init(void)
{
    volatile uint32_t delay;
    //SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
    SYSCTL_RCGC2_R |= 0x20;
    delay = SYSCTL_RCGC2_R;        // (b) initialize count and wait for clock
    GPIO_PORTF_CR_R = 0x0E;           // allow changes to PF3-1

    GPIO_PORTF_DIR_R = 0x0E;    // (c) make PF4 in (built-in button), PF3-1 out

    GPIO_PORTF_DEN_R |= 0x1E;     //     enable digital I/O on PF4, PF3-1

    GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
    PF1 = 0x00;
    PF2 = 0x00;
    PF3 = 0x00;
}

void GPIOInterrupt_Init(void)
{
    GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive

    GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges

    GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event

    GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4

    GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4

    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // (g) priority 5

    NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC

    EnableInterrupts();           // (i) Enable global Interrupt flag (I)
}

void EnableInterrupts(void)
{
    __asm ("    CPSIE  I\n");
}

/*void WaitForInterrupt(void)
 {
 __asm ("    WFI\n");
 }*/

int main(void)
{
    PortF_Init();
    GPIOInterrupt_Init(); // initialize GPIO Port F interrupt

    while (1)
    {
        //WaitForInterrupt();
        if (Int_Flag)
        {
            if (count == 0)
            {
                PF1 = 0x02;
                PF2 = 0x00;
                PF3 = 0x00;
            }
            else if (count == 1)
            {
                PF1 = 0x00;
                PF2 = 0x04;
                PF3 = 0x00;
            }
            else if (count == 2)
            {
                PF1 = 0x00;
                PF2 = 0x00;
                PF3 = 0x08;
            }

            count++;
            if (count == 3)
            {
                count = 0;
            }
            Int_Flag = 0;
        }
    }
}

void GPIOPortF_Handler(void)
{
    GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
    Int_Flag = !(Int_Flag);
}
