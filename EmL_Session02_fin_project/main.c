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

uint8_t SysTick_Flag = 0;
//volatile uint32_t ADC_xy_Data[2] = { 0, 0 };

//#define PA4   (*((volatile unsigned long *)0x40004040))
//#define PA5   (*((volatile unsigned long *)0x40004080))

void EnableInterrupts(void)
{
    __asm ("    CPSIE  I\n");
}

int main(void)
{
    SYSCLK_80MHz();
    SysTick_Init(2400); // 1/30 sec (30Hz).

    PortE_Init();
    //PortA_Init();
    //PA4 = 0x00;
    //PA5 = 0x00;
    ADC0_PortE_Init();
    ADC_var_Init();

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
    //Neutral value -> 2047 is an example.
    //should be checked by experiment.
    //volatile uint32_t ADC_xy_Data[2] = {0,0};
    //ADC0_InSeq2(ADC_xy_Data);
    ADC0_InSeq2(&ADC);

    if (ADC.xy_Data[0] <= 2047)
    {
        //ADC.x_to_LCD = ADC_xy_Data[0];
        //PA4 = 0x00;
    }
    else
    {
        //PA4 = 0x10;
    }

    if (ADC.xy_Data[1] <= 2047)
    {
        //ADC.y_to_LCD = ADC_xy_Data[1];
        //PA5 = 0x00;
    }
    else
    {
        //PA5 = 0x20;
    }

    /*Switch routine*/
    //-> External interrupt로 해야하나....? 한다면 -> interrupt로 해야함.
    //priority는 systick보다 낮게 해야함.
    SysTick_Flag = 1;
}

