/*
 * main.c
 *
 *  Created on: Nov 16, 2020
 *      Author: taehoonkim
 */
#include "main.h"
#include "sysclk.h"
#include "systick.h"
#include "gpio.h"
#include "adc.h"

void EnableInterrupts(void)
{
    __asm ("    CPSIE  I\n");
}

int main(void)
{
    SYSCLK_80MHz();
    SysTick_Init(2400); // 1/30 sec (30Hz).

    PortE_Init();
    ADC0_PortE_Init();

    EnableInterrupts();

    while (1)
    {
        if (SysTick_Flag)
        {

            //LCD_Dis();

            SysTick_Flag = 0;
        }
    }
    return 0;
}

void SysTick_Handler(void)
{
    /*ADC routine*/
    ADC0_InSeq2();
    if ((ADC_xy_Data[0] != /*mean value*/) || (ADC_xy_Data[1] != /*mean value*/)){
        ADC_2_LCD_x = ADC_xy_Data[0];
        ADC_2_LCD_y = ADC_xy_Data[1];
    }

    /*Switch routine*/
    //-> External interrupt로 해야하나....? 한다면
    //priority는 systick보다 낮게 해야함.

    SysTick_Flag = 1;
}
