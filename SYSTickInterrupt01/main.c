/**
 * main.c
 */
#include "main.h"
#include "gpio.h"
#include "systick.h"

volatile uint32_t counts = 0;

void EnableInterrupts(void)
{
    __asm ("    CPSIE  I\n");
}

void main(void)
{
    GPIO_Init();
    SysTick_Init(16000000); //0.5s
    EnableInterrupts();
    while (1)
    {

    }
}

void SysTick_Handler(void)
{
    PF2 ^= 0x04;
    counts += 1;
}
