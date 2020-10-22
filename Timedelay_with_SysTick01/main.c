

/**
 * main.c
 */
#include <main.h>

#include <sysclk.h>
#include <systick.h>
#include <gpio.h>

int main(void)
{
    SYSCLK_80MHz();
    SysTick_Init();
    PortF_Init();

    while(1)
    {
        PF1 = 0x02;
        SysTick_Wait_10ms (100);
        PF1 = 0;
        SysTick_Wait_10ms (100);
    }
}
